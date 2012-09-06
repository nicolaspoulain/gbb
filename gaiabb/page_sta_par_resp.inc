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
  drupal_add_js(drupal_get_path('module', 'gaiabb') . '/jquery.js', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
  drupal_add_js(drupal_get_path('module', 'gaiabb') . '/jquery.ui.tablefilter.js', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
  drupal_add_js(drupal_get_path('module', 'gaiabb') . '/my_food_plan_pick_foods.js', array('group' => CSS_DEFAULT, 'every_page' => TRUE));

  // Construction de la page
  $build = array();
  if (user_access('unstable gaiabb')) {
    $build['form_choose'] = render(drupal_get_form('form_choose', $args));
  } else { 
    $build['form_people'] = render(drupal_get_form('form_people', $args));
  };
  $build['filter_form'] = render(drupal_get_form('filter_form', $args));
  if (user_access('unstable gaiabb')) {
    $build['liste_stages'] = liste_stagesBIS($args);
  } else {
    $build['liste_stages'] = liste_stages($args);
  }; 
  return implode('', $build);
}

/**
 * Formulaire de ** CHOIX D'UN CONSEILLER ** 
 */
function form_people($form, &$form_state, $args) {

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
  asort($rows);

  $query = db_select('field_data_field_id_resp', 'u')
            ->condition('u.entity_id', $user->uid, '=')
            ->fields('u', array('field_id_resp_value'));
  $co_resp= ( arg(4)=='' )? $query->execute()->fetchField() : arg(4);


  $form['f']['voir_ferme'] = array(
    '#type' => 'checkbox',
    '#title' => t('Voir les modules fermés'),
    '#default_value' => (isset($_GET['voir_ferme']))? $_GET['voir_ferme'] : 0,
  );

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

  $form['f']['people'] = array(
    '#type' => 'select',
    '#title' => t('Conseiller'),
    '#options' => $rows,
    '#default_value' => $co_resp,
    '#attributes' => array(//'onChange' => "this.form.submit();",
                           //'class' => array("flottant")
                          ),
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

  $form['f']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Envoyer',
  );
  return $form;
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
 * ----> Submit
 */
function form_people_submit($form, &$form_state) {

  $url = url('sta_par_resp/' . $form_state['values']['year']
                                       . '/' . $form_state['values']['codegre']
                                       . '/' . $form_state['values']['cotres']
                                       . '/' . $form_state['values']['people'],
                            array('query' => array('voir_ferme' => $form_state['values']['voir_ferme']), 'absolute' => TRUE));
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
      WHERE j.co_degre = :codegre
        AND j.co_resp =  :coresp
        AND id_disp LIKE :year
        ';
  if (user_access('gest_convocs gaiabb') and !user_access('co_resp gaiabb')) {
  //  les non conseillers càd les administratifs
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
 /*    $query .= '
         AND (
           (select count(*) from {gbb_session} WHERE co_modu=m.co_modu AND co_degre=m.co_degre AND en_attente=0) > 0
           OR
           (select count(*) from {gbb_session} WHERE co_modu=m.co_modu AND co_degre=m.co_degre AND session_alert=1) > 0
         ) 
         AND (
           (select count(*) from {gbb_session} WHERE co_modu=m.co_modu AND co_degre=m.co_degre AND convoc_sent=1) <
           (select count(*) from {gbb_session} WHERE co_modu=m.co_modu AND co_degre=m.co_degre)
           OR
           mp.module_alert = 1
        )
        ';
*/
  }

  if ( !isset($_GET['voir_ferme']) || $_GET['voir_ferme']==0 ) {
    $query .= 'AND co_anmo = \'\'';
  }

  if (user_access('co_resp gaiabb') && arg(3) != 0) {
      $query .= 'AND j.co_tres = :cotres';
      $result = db_query($query, 
                     array(':year'    => arg(1) . '%',
                           ':codegre' => arg(2),
                           ':cotres' => arg(3),
                           ':coresp'  => $co_resp)
                );
  }
  else {
    $result = db_query($query, 
                     array(':year'    => arg(1) . '%',
                           ':codegre' => arg(2),
                           ':coresp'  => $co_resp)
                );
  }
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
         AND d.co_resp = :coresp
         AND d.co_tres = 2
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

    if (arg(4)!=0) {
      $query .= 'AND j.co_resp = :coresp ';
      $variables[':coresp'] = arg(4);
      if (arg(3)!=0) {
        $query .= 'AND j.co_tres = :cotres ';
        $variables[':cotres'] = arg(3);
      };
    };
    if (arg(5)!='0') {
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

