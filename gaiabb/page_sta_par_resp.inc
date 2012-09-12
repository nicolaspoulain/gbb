<?php
/**
 * @file
 *
 * Composition de la page de gestion de module
 *
 */

/**
 * Fonction d'appel aux differents elements composant la page
 * affichant une liste de responsables et les stages qui leur
 * associes
 */
function sta_par_resp($args = 'NaN') {

  // Charge la feuille des style et les fichiers javascript pour le module
  drupal_add_css(drupal_get_path('module', 'gaiabb') . '/gaiabb.css', array('group' => CSS_SYSTEM -1 , 'preprocess' => FALSE));
  drupal_add_css(drupal_get_path('module', 'gaiabb') . '/gaiabb.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));

  // Construction de la page
  $build = array();
  if (TRUE) {
    drupal_add_js(drupal_get_path('module', 'gaiabb') . '/jquery.dataTables.js', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
    drupal_add_js(drupal_get_path('module', 'gaiabb') . '/jquery.dataTables.load.js', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
    $build['form_choose'] = render(drupal_get_form('form_chooseBIS', $args));
    $build['liste_stages'] = liste_stagesBIS($args);
  } else {
    drupal_add_js(drupal_get_path('module', 'gaiabb') . '/jquery122.js', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
    drupal_add_js(drupal_get_path('module', 'gaiabb') . '/jquery.ui.tablefilter.js', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
    drupal_add_js(drupal_get_path('module', 'gaiabb') . '/my_food_plan_pick_foods.js', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
    $build['form_choose'] = render(drupal_get_form('form_choose', $args));
    $build['filter_form'] = render(drupal_get_form('filter_form', $args));
    $build['liste_stages'] = liste_stages($args);
  };
  return implode('', $build);
}

/**
 * Formulaire de ** CHOIX D'UN FORMATEUR OU D'UNE ORIENTATION ** 
 */
function form_choose($form, &$form_state, $args) {

  global $user;
  $query = db_select('gbb_gdire', 'j');
  $query  ->rightjoin('gbb_gdisp', 's', 'j.co_disp=s.co_disp AND j.co_degre=s.co_degre');
  $query  ->join('gbb_gresp', 'r', 'j.co_resp=r.co_resp AND j.co_degre=r.co_degre');
  $query  ->condition('j.co_degre', arg(2), '=')
          ->condition('s.id_disp' , db_like(arg(1)) . '%', 'LIKE')
          ->fields('j', array('co_resp'))
          ->fields('r', array('nomu'));
  if (user_access('co_resp gaiabb') && arg(3) != 0) {
      $query->condition('j.co_tres' , arg(3), '=');
  }
  elseif (user_access('co_resp gaiabb')  && arg(3) == 0) {
  }
  else {
      $query->condition('j.co_tres' , 2, '=');
  };
  $result = $query->execute();

  foreach ($result as $r) {
    $rows[$r->co_resp] = $r->nomu;
  }
  unset($rows['']);
  $rows['0'] = ' ';
  asort($rows);

  $query = db_select('field_data_field_id_resp', 'u')
            ->condition('u.entity_id', $user->uid, '=')
            ->fields('u', array('field_id_resp_value'));
  $co_resp= ( arg(4)=='' )? $query->execute()->fetchField() : arg(4);

  $degre = array(1 => 'Premier',
                 2 => 'Second' ,
                  );
  $form['f']['codegre'] = array(
    '#type' => 'select',
    '#title' => t('Degré'),
    '#default_value' => arg(2),
    '#options' => $degre,
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>',
  );

  $periode = array(
                  10 => '2010-2011',
                  11 => '2011-2012',
                  12 => '2012-2013'
                  );
  $form['f']['year'] = array(
    '#type' => 'select',
    '#title' => t('Année'),
    '#default_value' => arg(1),
    '#options' => $periode,
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>',
  );

  $form['f']['voir_ferme'] = array(
    '#type' => 'checkbox',
    '#title' => t('Voir les modules fermés'),
    '#default_value' => (isset($_GET['voir_ferme']))? $_GET['voir_ferme'] : 0,
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div><br />',
  );

  $form['f']['people'] = array(
    '#type' => 'select',
    '#title' => t('Conseiller'),
    '#options' => $rows,
    '#default_value' => $co_resp,
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>',
  );
  
  if (user_access('co_resp gaiabb')) {
      $tres = array(
                  0 => 'Tous',
                  1 => 'Interlocuteur',
                  2 => 'Resp. orga.',
                  3 => 'Resp. péda.'
                  );
      $form['f']['cotres'] = array(
           '#type' => 'select',
           '#title' => t('Rôle'),
           '#default_value' => arg(3),
           '#options' => $tres,
           '#prefix' => '<div class="inline">',
           '#suffix' => '</div>',
           );
  }
  else {
      $form['f']['cotres'] = array(
           '#type' => 'hidden',
           '#value' => arg(3),
           );
  };

  $query = db_select('gbb_norie', 'o');
  $query  ->rightjoin('gbb_gdisp', 'd', 'o.co_orie=d.co_orie');
  $query  ->condition('d.co_degre', arg(2), '=')
          ->condition('d.id_disp' , db_like(arg(1)) . '%', 'LIKE')
          ->fields('o', array('co_orie', 'lib_long'));
  $orientations = $query->execute();

  foreach ($orientations as $r) {
    $ories[$r->co_orie] = $r->lib_long;
  }
  unset($ories['']);
  $ories['0']=' ';
  asort($ories);

  $co_orie = ( arg(5)=='' )? 0 : arg(5);
  $form['f']['coorie'] = array(
    '#type' => 'select',
    '#title' => t('Orientation'),
    '#options' => $ories,
    '#default_value' => $co_orie,
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>',
  );
  
  $form['f']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Envoyer',
  );


  return $form;
}

/**
 * Formulaire de ** CHOIX D'UN FORMATEUR OU D'UNE ORIENTATION ** 
 */
function form_chooseBIS($form, &$form_state, $args) {

  global $user;
  $query = db_select('gbb_gdire', 'j');
  $query  ->rightjoin('gbb_gdisp', 's', 'j.co_disp=s.co_disp AND j.co_degre=s.co_degre');
  $query  ->join('gbb_gresp', 'r', 'j.co_resp=r.co_resp AND j.co_degre=r.co_degre');
  $query  ->condition('j.co_degre', arg(2), '=')
          ->condition('s.id_disp' , db_like(arg(1)) . '%', 'LIKE')
          ->fields('j', array('co_resp'))
          ->fields('r', array('nomu'));
  if (user_access('co_resp gaiabb') && arg(3) != 0) {
      $query->condition('j.co_tres' , arg(3), '=');
  }
  elseif (user_access('co_resp gaiabb')  && arg(3) == 0) {
  }
  else {
      $query->condition('j.co_tres' , 2, '=');
  };
  $result = $query->execute();

  foreach ($result as $r) {
    $rows[$r->co_resp] = $r->nomu;
  }
  unset($rows['']);
  $rows['0'] = ' ** TOUS ** ';
  asort($rows);

  $degre = array(1 => 'Premier',
                 2 => 'Second' ,
                  );
  $form['f']['codegre'] = array(
    '#type' => 'select',
    '#title' => t('Degré'),
    '#default_value' => arg(2),
    '#options' => $degre,
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>',
  );

  $periode = array(
                  10 => '2010-2011',
                  11 => '2011-2012',
                  12 => '2012-2013'
                  );
  $form['f']['year'] = array(
    '#type' => 'select',
    '#title' => t('Année'),
    '#default_value' => arg(1),
    '#options' => $periode,
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>',
  );

  $form['f']['voir_ferme'] = array(
    '#type' => 'checkbox',
    '#title' => t('Voir les modules fermés'),
    '#default_value' => (isset($_GET['voir_ferme']))? $_GET['voir_ferme'] : 0,
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div><br />',
  );

  $active = array(0 => t('Tous («Vue Conseiller»)'), 1 => t('En demande de traitement («Vue Administratif»)'));
  $form['f']['voir_encours'] = array(
    '#type' => 'radios',
    '#options' => $active,
    '#default_value' => (isset($_GET['voir_encours']))? $_GET['voir_encours'] : ((user_access('gest_sessions gaiabb'))? 0 : 1) ,
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div><br />',
  );

  // ------------
  // Ici on récupère le co_resp par défaut si arg(4) est vide
  // cela pour caler le select sur le bon conseiller
  $query = db_select('field_data_field_id_resp', 'u')
            ->condition('u.entity_id', $user->uid, '=')
            ->fields('u', array('field_id_resp_value'));
  $input = ( arg(4)=='' )? $query->execute()->fetchField() : arg(4);

  // Si $input est numérique alors c'est bien un co_resp
  // Si $input est une chaine, alors c'est un nomu, il faut récupérer les co_resp correspondants
  if( preg_match('/[^0-9]/', $input) ) {
      //echo $co_resp." is String";
    $nomu = $input;
  } else {
      //echo $co_resp." is Int";
      // ------------
      // Ici on récupère le nom du conseiller correspondant à $co_resp
      $query = db_select('gbb_gresp', 'r');
      $query  ->condition('r.co_resp', $input, '=')
              ->condition('r.co_degre', arg(2), '=')
              ->fields('r', array('nomu'));
      $nomu = $query->execute()->fetchField();
  };

  $form['f']['people'] = array(
    '#type' => 'textfield',
    '#title' => t('Conseiller'),
    '#default_value' => $nomu,
    '#size' => 20,
    '#autocomplete_path' => 'autocomp/responsable',
    '#prefix' => '<br/><div class="inline"><div class="ajaxform">',
    '#suffix' => '</div></div>&nbsp;',
  );

  $query = db_select('gbb_norie', 'o');
  $query  ->rightjoin('gbb_gdisp', 'd', 'o.co_orie=d.co_orie');
  $query  ->condition('d.co_degre', arg(2), '=')
          ->condition('d.id_disp' , db_like(arg(1)) . '%', 'LIKE')
          ->fields('o', array('co_orie', 'lib_long'));
  $orientations = $query->execute();

  foreach ($orientations as $r) {
    $ories[$r->co_orie] = $r->lib_long;
  }
  unset($ories['']);
  $ories['0']='TOUS';
  asort($ories);

  $co_orie = ( arg(5)=='' )? 0 : arg(5);

  if (user_access('gest_sessions gaiabb')) {
      $tres = array(
                  0 => 'TOUS',
                 // 1 => 'Interlocuteur',
                  2 => 'Resp. orga.',
                  3 => 'Resp. péda.'
                  );
      $form['f']['cotres'] = array(
           '#type' => 'select',
           '#title' => t('Rôle'),
           '#default_value' => arg(3),
           '#options' => $tres,
           '#prefix' => '<div class="inline">',
           '#suffix' => '</div>',
           );

      $form['f']['coorie'] = array(
           '#type' => 'select',
           '#title' => t('Orientation'),
           '#options' => $ories,
           '#default_value' => $co_orie,
           '#prefix' => '<div class="inline">',
           '#suffix' => '</div>',
           );
   }
  else {
      $form['f']['cotres'] = array(
           '#type' => 'hidden',
           '#value' => 2,
           );
      $form['f']['coorie'] = array(
           '#type' => 'hidden',
           '#value' => 0,
           );
  };

  $form['f']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Envoyer',
  );

  return $form;
}

/**
 * ----> Submit
 */
function form_chooseBIS_submit($form, &$form_state) {
  $url = url('sta_par_resp/' . $form_state['values']['year']
                                       . '/' . $form_state['values']['codegre']
                                       . '/' . $form_state['values']['cotres']
                                       . '/' . $form_state['values']['people']
                                       . '/' . $form_state['values']['coorie'],
                            array('query' => array(
                                'voir_ferme' => $form_state['values']['voir_ferme'], 
                                'voir_encours' => $form_state['values']['voir_encours']), 
                                'absolute' => TRUE));
  $form_state['redirect'] = $url;
}

/**
 * ----> Submit
 */
function form_choose_submit($form, &$form_state) {
  $url = url('sta_par_resp/' . $form_state['values']['year']
                                       . '/' . $form_state['values']['codegre']
                                       . '/' . $form_state['values']['cotres']
                                       . '/' . $form_state['values']['people']
                                       . '/' . $form_state['values']['coorie'],
                            array('query' => array('voir_ferme' => $form_state['values']['voir_ferme']), 'absolute' => TRUE));
  $form_state['redirect'] = $url;
}

/**
 * Formulaire de ** FILTRE JS DU TABLEAU **
 */
function filter_form($form, &$form_state, $args) {

  $form['f']['filter'] = array(
    '#type' => 'textfield',
    '#title' => t('Filtrage en direct'),
    '#attributes' => array('id' => array('filter')),
    '#description' => t('Saisissez quelques lettres pour réduire la liste. Saisissez :<br />- « paf* » ou « fil* » pour réduire à un type de stage, <br />- « ! » pour réduire aux stages prioritaires, <br />- « > » pour réduire aux stages vus par le service administratif.'),
    '#prefix' => '<div class="directfilter">',
    '#suffix' => '</div>',
    '#size' => 40, 
  );
  return $form;
}

/**
 * Fonction de ** LISTE DES STAGES ** pour un conseiller donne en argument
 */
function liste_stages($args) {

  global $user;
  $query = db_select('field_data_field_id_resp', 'u')
            ->condition('u.entity_id', $user->uid, '=')
            ->fields('u', array('field_id_resp_value'));
  $co_resp= ( arg(4)=='' )? $query->execute()->fetchField() : arg(4);
  // TODO : FAIRE LES DEUX REQUETES EN UNE SEULE AVEC UN GROUP BY POUR OPTIMISER 
  $query = '
      SELECT DISTINCT m.co_modu,
                      mp.module_alert,
                      mp.prioritaire,
                      m.lib AS libm,
                      d.co_disp,
                      id_disp,
                      d.lib AS libd,
                      co_camp,
                      co_andi,
                      co_anmo,
                      (select count(*) from {gbb_session} WHERE co_modu=m.co_modu AND co_degre=m.co_degre AND en_attente=0 AND convoc_sent=0) as nb_nea,
                      (select count(*) from {gbb_session} WHERE co_modu=m.co_modu AND co_degre=m.co_degre AND convoc_sent=1) as nb_s,
                      (select count(*) from {gbb_session} WHERE co_modu=m.co_modu AND co_degre=m.co_degre AND en_attente=1) as nb_ea,
                      (select count(*) from {gbb_session} WHERE co_modu=m.co_modu AND co_degre=m.co_degre AND session_alert=1) as nb_sa,
                      (select count({gbb_file}.fid) from {gbb_file} JOIN {file_managed} ON {file_managed}.fid={gbb_file}.fid  Where co_modu=mp.co_modu AND co_degre=mp.co_degre AND status=1) as nb_pj
      FROM {gbb_gmodu} as m
      LEFT JOIN {gbb_gmodu_plus} as mp ON mp.co_modu=m.co_modu
                                      AND mp.co_degre=m.co_degre
      JOIN {gbb_gdisp} as d ON m.co_disp=d.co_disp
                           AND m.co_degre=d.co_degre
      JOIN {gbb_gdire} as j ON m.co_modu=j.co_modu
                           AND m.co_degre=j.co_degre
      WHERE d.co_degre = :codegre
        AND id_disp LIKE :year
        ';
  $variables = array();
  $variables[':year'] = arg(1) . '%';
  $variables[':codegre'] = arg(2);

  if (user_access('gest_convocs gaiabb') and !user_access('gest_sessions gaiabb')) {
  //  les administratifs
  //  voient seulement les stages qui ont
  //  - OU  module_alert = 1
  //  - OU  ( nb session_alert = 1 ) > 0
  //  - OU  ( nb de convoc_sent = 1 ) < (nb en_attente = 0 )
  $query .= '
         AND j.co_resp = :coresp
         AND j.co_tres = 2
         AND (
           mp.module_alert = 1
           OR
           (select count(*) from {gbb_session} WHERE co_modu=m.co_modu AND co_degre=m.co_degre AND session_alert=1) > 0
           OR
           (select count(*) from {gbb_session} WHERE co_modu=m.co_modu AND co_degre=m.co_degre AND convoc_sent=1) <
           (select count(*) from {gbb_session} WHERE co_modu=m.co_modu AND co_degre=m.co_degre AND en_attente=0)
        )
        ';
  $variables[':coresp'] = $co_resp;
  }
  else {

    if ($co_resp!=0) {
      $query .= 'AND j.co_resp = :coresp ';
      $variables[':coresp'] = arg(4);
      if (arg(3)!=0) {
        $query .= 'AND j.co_tres = :cotres ';
        $variables[':cotres'] = arg(3);
      };
    };
    if (arg(5)!='0' && arg(5)!='') {
      $query .= 'AND d.co_orie = :coorie ';
      $variables[':coorie'] = arg(5);
    };
  };

  if ( !isset($_GET['voir_ferme']) || $_GET['voir_ferme']==0 ) {
    $query .= 'AND co_anmo = \'\'';
  }
  
  $result = db_query($query, $variables);

  $liste = array();
  $libd_precedent= '';
  $rows = array();


  $rows[] = array( 
              array('data' => ""), 
              array('data' => "Le " .
                              format_date(time(), 'custom', 'l j F Y à H:i') .
                              ", " .
                              $result->rowCount() .
                              " stages." 
                   ),
              array('data' => ""), 
            );

  foreach ($result as $r) {
      $m_ouvert_ou_ferme = ($r->co_anmo == '')? '' : 'Ferme';
      $options_comodu = array('attributes' => array('class' => array($m_ouvert_ou_ferme),
                                                    'id' => 'cool-id',
                                                   ),
                              'html' => FALSE,
                             );

      $d_ouvert_ou_ferme = ($r->co_andi == '')? '' : 'Ferme';

      $paf_ou_fil = ($r->co_camp == 'BS')? 'fil' : 'paf';
      $libd_class = ($r->co_camp == 'BS')? 'fil' : 'paf';
      $libd_class = ($r->libd == $libd_precedent)? "gris" : $libd_class;

      $variables = array(
        'path' => drupal_get_path('module', 'gaiabb') . '/images/flag_red.png',
        'alt' => 'session_alert',
        'title' => t('Alerte sur session'),
        'attributes' => array('class' => 'some-img', 'id' => 'my-img'));
      $session_alert = ($r->nb_sa >0)? theme('image', $variables) . '<span class="nb_sa">' . $r->nb_sa . '</span>' : '';

      $variables = array(
        'path' => drupal_get_path('module', 'gaiabb') . '/images/control_play_blue.png',
        'alt' => 'session_alert',
        'title' => t('session en route'),
        'attributes' => array('class' => 'some-img', 'id' => 'my-img'));
      $NOT_en_attente = ($r->nb_nea >0)? theme('image', $variables) . '<span class="nb_sa">' . $r->nb_nea . '</span>' .'<span class="invisible">></span> ' : '';

      $variables = array(
        'path' => drupal_get_path('module', 'gaiabb') . '/images/control_pause.png',
        'alt' => 'en_attente',
        'title' => t('En attente'),
        'attributes' => array('class' => 'some-img', 'id' => 'my-img'));
      $en_attente = ($r->nb_ea >0)? theme('image', $variables) . '<span class="nb_sa">' . $r->nb_ea . '</span>' : '';

      $variables = array(
        'path' => drupal_get_path('module', 'gaiabb') . '/images/flag_red.png',
        'alt' => 'module_alert',
        'title' => t('Alerte entre conseillers'),
        'attributes' => array('class' => 'some-img', 'id' => 'my-img'));
      $module_alert = ($r->module_alert)? theme('image', $variables) . '<span class="invisible">></span> ' : '';

      $variables = array(
        'path' => drupal_get_path('module', 'gaiabb') . '/images/prioritaire.png',
        'alt' => 'prioritaire',
        'title' => t('Prioritaire'),
        'attributes' => array('class' => 'some-img', 'id' => 'my-img'));
      $prioritaire = ($r->prioritaire)? theme('image', $variables) . '<span class="invisible">!</span> ' : '';

      $variables = array(
        'path' => drupal_get_path('module', 'gaiabb') . '/images/convoc_sent.png',
        'alt' => 'convoc_sent',
        'title' => t('Convocations envoyées'),
        'attributes' => array('class' => 'some-img', 'id' => 'my-img'));
      $convoc_sent = ($r->nb_s >0)? theme('image', $variables) . '<span class="nb_sa">' . $r->nb_s . '</span>' : '';

      $variables = array(
        'path' => drupal_get_path('module', 'gaiabb') . '/images/attachement.gif',
        'alt' => 'convoc_sent',
        'title' => t('Fichier(s) attaché(s)'),
        'attributes' => array('class' => 'some-img', 'id' => 'my-img'));
      $attachement = ($r->nb_pj > 0)? theme('image', $variables) . '<span class="nb_pj">' . $r->nb_pj . '</span>' : '';

      $rows[] = array( 
                  array('data' => 
                                  //$attachement .
                                  $convoc_sent .
                                  $session_alert .
                                  $NOT_en_attente .
                                  $en_attente .
                                  $module_alert .
                                  $prioritaire,
                       ),
                  array('data' =>
                                 '<span class="invisible">' . $paf_ou_fil . '*' . '</span> ' .
                                 '<span class="' . $libd_class . ' ' .
                                 $d_ouvert_ou_ferme . '">' . $r->id_disp . ' ' . t($r->libd) . '</span> ' .
                                 l($r->co_modu, 'gest_module/' . arg(2) . '/' . $r->co_modu, $options_comodu) . ' ' .
                                 '<span class="' . $m_ouvert_ou_ferme . '">' . t($r->libm) . '</span> ',
                       ),
                  array('data' => "&nbsp;",
                       ),
               );
      $libd_precedent = $r->libd;
    }


  $attributes = array('id' => 'table-id', 'class' => 'food_planner');
  return theme('table', array('rows' => $rows, 'attributes' => $attributes));
}

/**
 * Fonction de ** LISTE DES STAGES ** pour un conseiller donne en argument
 */
function liste_stagesBIS($args) {
  // ------------

  $query = '
      SELECT DISTINCT m.co_modu,
                      mp.module_alert,
                      mp.prioritaire,
                      mp.organisation,
                      m.lib AS libm,
                      d.co_disp,
                      id_disp,
                      d.lib AS libd,
                      co_camp,
                      co_andi,
                      co_anmo,
                      (select count(*) from {gbb_session} WHERE co_modu=m.co_modu AND co_degre=m.co_degre AND en_attente=0 AND convoc_sent=0) as nb_nea,
                      (select count(*) from {gbb_session} WHERE co_modu=m.co_modu AND co_degre=m.co_degre AND convoc_sent=1) as nb_s,
                      (select count(*) from {gbb_session} WHERE co_modu=m.co_modu AND co_degre=m.co_degre AND en_attente=1) as nb_ea,
                      (select count(*) from {gbb_session} WHERE co_modu=m.co_modu AND co_degre=m.co_degre AND session_alert=1) as nb_sa,
                      (select count({gbb_file}.fid) from {gbb_file} JOIN {file_managed} ON {file_managed}.fid={gbb_file}.fid  Where co_modu=mp.co_modu AND co_degre=mp.co_degre AND status=1) as nb_pj
      FROM {gbb_gmodu} as m
      LEFT JOIN {gbb_gmodu_plus} as mp ON mp.co_modu=m.co_modu
                                      AND mp.co_degre=m.co_degre
      JOIN {gbb_gdisp} as d ON m.co_disp=d.co_disp
                           AND m.co_degre=d.co_degre 
      JOIN {gbb_gdire} as j ON m.co_modu=j.co_modu
                                   AND m.co_degre=j.co_degre 
      WHERE d.co_degre IN (:codegre)
        AND id_disp LIKE :year
        ';
  $variables = array();
  $variables[':year'] = arg(1) . '%';
  $variables[':codegre'] = arg(2);

  if ( arg(4)=='' ) {
    // On récupère le co_resp par défaut.dans le profil du user connecté
    global $user;
    $qu = db_select('field_data_field_id_resp', 'u')
            ->condition('u.entity_id', $user->uid, '=')
            ->fields('u', array('field_id_resp_value'));
    $input = $qu->execute()->fetchField();
  } else {
  // L'URL ne la donne pas, donc la variable $co_resp est la valeur par défaut
    $input = arg(4);
  };

  // Si $input est numérique alors c'est bien un co_resp
  // Si $input est une chaine, alors c'est un nomu, il faut récupérer les co_resp correspondants
  if( $input == 'TOUS' ) {
      //echo $input." is String";
  } elseif( preg_match('/[^0-9]/', $input) ) {
      //echo $co_resp." is String";
      //$tabResp = array("2011", "1690");
      $matches = array();
      $tabResp = db_select('gbb_gresp', 'c')
        ->fields('c', array('co_resp'))
        ->condition('nomu', '' . db_like($input) . '', 'LIKE')
        ->distinct()
        ->execute()->fetchCol();
      $variables[':coresp'] = $tabResp;
      //print_r($tabResp);
      $query .='  AND j.co_resp IN (:coresp) ';
  } else {
      //echo $co_resp." is Int";
      $tabResp = array( $input);
      $variables[':coresp'] = $tabResp;
      $query .='  AND j.co_resp IN (:coresp) ';
  };



  $voirEnCours = (isset($_GET['voir_encours']))? (($_GET['voir_encours']==1)? TRUE : FALSE) : FALSE;

  if ($voirEnCours) {
    //  les administratifs
    //  voient seulement les stages qui ont
    //  - OU  module_alert = 1
    //  - OU  ( nb session_alert = 1 ) > 0
    //  - OU  ( nb de convoc_sent = 1 ) < (nb en_attente = 0 )
    $query .= '
         AND (
           mp.module_alert = 1
           OR
           (select count(*) from {gbb_session} WHERE co_modu=m.co_modu AND co_degre=m.co_degre AND session_alert=1) > 0
           OR
           (select count(*) from {gbb_session} WHERE co_modu=m.co_modu AND co_degre=m.co_degre AND convoc_sent=1) <
           (select count(*) from {gbb_session} WHERE co_modu=m.co_modu AND co_degre=m.co_degre AND en_attente=0)
        )
        ';
  }

      if (arg(3)!=0) {
        $query .= 'AND j.co_tres = :cotres ';
        $variables[':cotres'] = arg(3);
      };
    if (arg(5)!='0' && arg(5)!='') {
      $query .= 'AND d.co_orie = :coorie ';
      $variables[':coorie'] = arg(5);
    };

  if ( !isset($_GET['voir_ferme']) || $_GET['voir_ferme']==0 ) {
    $query .= 'AND co_anmo = \'\'';
  }
  
  $result = db_query($query, $variables);

  $liste = array();
  $libd_precedent= '';
  $rows = array();

  $header = array(
               'col0' => array('data' => "dispo"), 
               'col1' => array('data' => "status"), 
               'col2' => array('data' => "Le " .
                              format_date(time(), 'custom', 'l j F Y à H:i') .
                              ", " .
                              $result->rowCount() .
                              " stages." 
                   ),
               'col3' => array('data' => ".."), 
            );

  foreach ($result as $r) {
      $m_ouvert_ou_ferme = ($r->co_anmo == '')? '' : 'Ferme';
      $options_comodu = array('attributes' => array('class' => array($m_ouvert_ou_ferme),
                                                    'id' => 'cool-id',
                                                   ),
                              'html' => FALSE,
                             );

      $d_ouvert_ou_ferme = ($r->co_andi == '')? '' : 'Ferme';

      $paf_ou_fil = ($r->co_camp == 'BS')? 'fil' : 'paf';
      $libd_class = ($r->co_camp == 'BS')? 'fil' : 'paf';
      $libd_class = ($r->libd == $libd_precedent)? "gris" : $libd_class;

      $variables = array(
        'path' => drupal_get_path('module', 'gaiabb') . '/images/flag_red.png',
        'alt' => 'session_alert',
        'title' => t('Alerte sur session'),
        'attributes' => array('class' => 'some-img', 'id' => 'my-img'));
      $session_alert = ($r->nb_sa >0)? theme('image', $variables) . '<span class="nb_sa">' . $r->nb_sa . '</span>'. '<span class="invisible">></span> '  : '';

      $variables = array(
        'path' => drupal_get_path('module', 'gaiabb') . '/images/control_play_blue.png',
        'alt' => 'session_alert',
        'title' => t('session en route'),
        'attributes' => array('class' => 'some-img', 'id' => 'my-img'));
      $NOT_en_attente = ($r->nb_nea >0)? theme('image', $variables) . '<span class="nb_sa">' . $r->nb_nea . '</span>'. '<span class="invisible">></span> '  : '';

      $variables = array(
        'path' => drupal_get_path('module', 'gaiabb') . '/images/control_pause.png',
        'alt' => 'en_attente',
        'title' => t('En attente'),
        'attributes' => array('class' => 'some-img', 'id' => 'my-img'));
      $en_attente = ($r->nb_ea >0)? theme('image', $variables) . '<span class="nb_sa">' . $r->nb_ea . '</span>' : '';

      $variables = array(
        'path' => drupal_get_path('module', 'gaiabb') . '/images/flag_red.png',
        'alt' => 'module_alert',
        'title' => t('Alerte entre conseillers'),
        'attributes' => array('class' => 'some-img', 'id' => 'my-img'));
      $module_alert = ($r->module_alert)? theme('image', $variables). '<span class="invisible">></span> '  : '';

      $variables = array(
        'path' => drupal_get_path('module', 'gaiabb') . '/images/prioritaire.png',
        'alt' => 'prioritaire',
        'title' => t('Prioritaire'),
        'attributes' => array('class' => 'some-img', 'id' => 'my-img'));
      $prioritaire = ($r->prioritaire)? theme('image', $variables) . '<span class="invisible">!</span> ' : '';

      $variables = array(
        'path' => drupal_get_path('module', 'gaiabb') . '/images/convoc_sent.png',
        'alt' => 'convoc_sent',
        'title' => t('Convocations envoyées'),
        'attributes' => array('class' => 'some-img', 'id' => 'my-img'));
      $convoc_sent = ($r->nb_s >0)? theme('image', $variables) . '<span class="nb_sa">' . $r->nb_s . '</span>' : '';

      $variables = array(
        'path' => drupal_get_path('module', 'gaiabb') . '/images/attachement.gif',
        'alt' => 'convoc_sent',
        'title' => t('Fichier(s) attaché(s)'),
        'attributes' => array('class' => 'some-img', 'id' => 'my-img'));
      $attachement = ($r->nb_pj > 0)? theme('image', $variables) . '<span class="nb_pj">' . $r->nb_pj . '</span>' : '';

      $rows[] = array( 
                  'col0' => array('data' => 
                                 '<span class="invisible">' . $paf_ou_fil . '*' . '</span> ' .
                                 '<span class="' .$paf_ou_fil . ' ' . $d_ouvert_ou_ferme . '">' . $r->id_disp . ' ' . t($r->libd) . '</span> '
                  ),
                  'col1' => array('data' => 
                                  $attachement .
                                  $convoc_sent .
                                  $session_alert .
                                  $NOT_en_attente .
                                  $en_attente .
                                  $module_alert .
                                  $prioritaire,
                                  'class' => 'nowrap',
                       ),
                  'col2' => array('data' =>
                                 l($r->co_modu, 'gest_module/' . arg(2) . '/' . $r->co_modu, $options_comodu) . ' ' .
                                 '<span class="' . $m_ouvert_ou_ferme . '">' . t($r->libm) . '</span> ',
                       ),
                  'col3' => array('data' => $r->organisation,
                       ),
                  'col4' => array('data' => ".",
                       ),
               );
      $libd_precedent = $r->libd;
    }
  $attributes = array('id' => 'table-id', 'class' => array('display'));
  return theme('table', array('rows' => $rows, 'attributes' => $attributes));

  //$header = array('Name', 'Type', 'Updated');
  //$attributes = array('id' => 'stagestable', 'class' => 'food_planner');
  //return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => $attributes));
  //return theme('table', array('rows' => $rows, 'attributes' => $attributes));

/*
$header = array('Name'    => array('data' => t('Name')), 
                'Type'    => array('data' => t('data')),
               );
 # set the database table
$query = db_select('actions', 'p')->extend('PagerDefault');
$query->fields('p', array('aid', 'type'));
$results = $query->execute();
$rows = array();
foreach ($results as $row) {
  $rows[] = array('Name' => array('data' => $row->aid),
                  'Type' => array('data' => $row->type),
                  );
}
//print_r($header);
//print_r($rows);


$attr = array( 'id'    => 'demotable1',
               'class' => array('iii'),
        );

$output = theme('table', array('header' => $header,
                               'rows' => $rows,
                               'attributes' => $attr,
               ));
//$output = theme('table', $header, $rows);
# add the pager
$output .= theme('pager');
return $output;
*/
}


/**
 * Fonctions d' ** AUTOCOMPLETE AJAX RESPONSABLES **
 */
function _gaiabb_autocompresp($string) {

  $matches = array();
  $result = db_select('gbb_gresp', 'c')
    ->fields('c', array('nomu'))
    ->condition('nomu', '%' . db_like($string) . '%', 'LIKE')
    ->distinct()
    ->range(0, 10)
    ->execute();
  foreach ($result as $r) {
    $matches[$r->nomu] = $r->nomu;
  }
  $count = $result->rowCount();
  if ($count > 9) $matches[' '] = '..... Et bien d\'autres .....';
  drupal_json_output($matches);
}
