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
  drupal_add_library('system', 'drupal.collapse');

  // Construction de la page
  $build = array();
  drupal_add_js(drupal_get_path('module', 'gaiabb') . '/jquery.dataTables.js', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
  drupal_add_js(drupal_get_path('module', 'gaiabb') . '/jquery.dataTables.load.js', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
  //if (user_access('unstable gaiabb')) {
  $build['recherche'] = render(drupal_get_form('recherche_form',$args));
  $build['form_choose'] = render(drupal_get_form('form_chooseBIS', $args));
  $build['liste_stages'] = liste_stagesBIS($args);
  return implode('', $build);
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

  $form['f'] = array(
    '#type' => 'fieldset',
    '#title' => t('Recherche par conseiller'),
    '#attributes' => array('class' => array('collapsible',)),
  );


  $active = array(0 => t('Tous («Vue Conseiller»)'), 1 => t('En demande de traitement («Vue Administratif»)'));
  $form['f']['voir_encours'] = array(
    '#type' => 'radios',
    '#options' => $active,
    '#default_value' => (isset($_GET['voir_encours']))? $_GET['voir_encours'] : ((user_access('gest_sessions gaiabb'))? 0 : 1) ,
    '#prefix' => '<p><div class="flotteDroite">',
    '#suffix' => '</div></p>',
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
    '#suffix' => '</div>&nbsp;&nbsp;',
  );

  $form['f']['voir_ferme'] = array(
    '#type' => 'checkbox',
    '#title' => t('Voir les modules fermés'),
    '#default_value' => (isset($_GET['voir_ferme']))? $_GET['voir_ferme'] : 0,
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
    '#element_validate' => array('_choix_conseiller_validate'),
    '#autocomplete_path' => 'autocomp/responsable',
    '#prefix' => '<br/><div class="ajaxform inline">',
    '#suffix' => '</div>&nbsp;',
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

  $form['if']['infos']['#markup']= '<div class="infos">' . t('Saisissez quelques lettres ou chiffres du stage pour réduire la liste.<br />Vous pouvez aussi utiliser les jokers : « paf* » ou « fil* » pour réduire à un type de stage, ou bien « ! » pour réduire aux stages prioritaires.') . '</div>';


  return $form;
}

function _choix_conseiller_validate($element, &$form_state) {
  $query = db_select('gbb_gresp', 'r')
    ->fields('r', array('nomu'))
    ->condition('nomu', $element['#value'], '=')
    ->distinct();
  $num_rows = $query->countQuery()->execute()->fetchField();
  if ($num_rows == 0) {
    form_error($element, t('Le champ Conseiller ne peut être qu\'une des valeurs proposées, et que vous aurez cliquée.'));
  }
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
 * Fonction de ** LISTE DES STAGES ** pour un conseiller donne en argument
 */
function liste_stagesBIS($args) {
  // ------------

  $query = '
      SELECT DISTINCT m.co_modu,
                      mp.module_alert,
                      mp.module_printed,
                      mp.prioritaire,
                      mp.organisation,
                      m.lib AS libm,
                      d.co_disp,
                      id_disp,
                      d.lib AS libd,
                      co_camp,
                      co_andi,
                      co_anmo,
                      (select MIN(date) from {gbb_session} WHERE co_modu=m.co_modu AND co_degre=m.co_degre AND en_attente=0) as dateD,
                      (select MAX(date) from {gbb_session} WHERE co_modu=m.co_modu AND co_degre=m.co_degre AND en_attente=0) as dateF,
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
        'path' => drupal_get_path('module', 'gaiabb') . '/images/convoc_printed.gif',
        'alt' => 'module_printed',
        'title' => t('Imprimé'),
        'attributes' => array('class' => 'some-img', 'id' => 'my-img'));
      $module_printed = ($r->module_printed && $voirEnCours)? theme('image', $variables) . ' ' : '';

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

      $variables = array(
        'path' => drupal_get_path('module', 'gaiabb') . '/images/comments.png',
        'alt' => 'Organisation',
        'title' => t('Organisation'),
        'attributes' => array('class' => 'some-img', 'id' => 'my-img'));
      $plus = ($r->organisation == '')? '' : '&nbsp;' . theme('image', $variables);

      $variables = array(
        'path' => drupal_get_path('module', 'gaiabb') . '/images/timer_green.png',
        'alt' => 'Timer',
        'title' => t('Première date dans plus de 15 jours'),
        'attributes' => array('class' => 'some-img', 'id' => 'my-img'));
      if (strtotime($r->dateD) < time()) { // le premier jour dépassé
          $variables['path']=drupal_get_path('module', 'gaiabb') . '/images/timer_red.png';
          $variables['title']=t('Première date dépassée');
      } elseif(strtotime($r->dateD) < time() + 1296000) { // le premier jour est dans moins de 15 jours.
          $variables['path']=drupal_get_path('module', 'gaiabb') . '/images/timer_orange.png';
          $variables['title']=t('Première date dans moins de 15 jours');
      };
      if (strtotime($r->dateF) < time()) { // le dernier jour jour dépassé donc formation finie
          $variables['path']=drupal_get_path('module', 'gaiabb') . '/images/timer_end.png';
          $variables['title']=t('Formation terminée');
      }
      $timer = theme('image', $variables);

      if (isset($r->dateD)) {
        if ($r->dateD==$r->dateF) {
            $dates = $timer . t("&nbsp;" . strftime('%d/%m/%y', strtotime($r->dateD))) ; 
        } else {
            $dates = $timer . t("&nbsp;" . strftime('%d/%m/%y', strtotime($r->dateD)) . "&rarr;" . strftime('%d/%m/%y', strtotime($r->dateF)));
        };
      } else {
          $dates ='';
      };

      $rows[] = array( 
                  'col0' => array('data' => 
                                 '<span class="invisible">' . $paf_ou_fil . '*' . '</span> ' .
                                 '<span class="' .$paf_ou_fil . ' ' . $d_ouvert_ou_ferme . '">[' . strtoupper($paf_ou_fil) . "] " . $r->id_disp . ' ' . t($r->libd) . '</span> '
                  ),
                  'col1' => array('data' => 
                                  $attachement .
                                  $module_printed .
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
                                 '<span class="' . $m_ouvert_ou_ferme . '">' . t($r->libm) . $plus . '</span> ',
                       ),
                  'col3' => array('data' => nl2br($r->organisation),
                       ),
                  'col4' => array('data' => $dates,
                       ),
               );
      $libd_precedent = $r->libd;
    }
  $attributes = array('id' => 'table-id', 'class' => array('display'));
  return theme('table', array('rows' => $rows, 'attributes' => $attributes));
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

/**
 * Formulaire de ** FORMULAIRE DE RECHERCHE TOUS AZIMUTS **
 */
function recherche_form($form, &$form_state, $args) {
  $form['f'] = array(
    '#type' => 'fieldset',
    '#title' => t('Recherche tous azimuts'),
    '#attributes' => array('class' => array('sessions','recherchestage', 'collapsible', 'collapsed')),
  );

  $form['f']['comodu'] = array(
             '#type' => 'textfield',
             '#title' => t('Rechercher un STAGE'),
             '#description' => t('Saisissez quelques lettres du titre (ou chiffres du numéro) pour rechercher un stage'),
             '#autocomplete_path' => 'autocomp/stage',
             '#prefix' => '<div class="ajaxform inline">',
             '#suffix' => '</div>',
             '#size' => 80, 
  );
  $form['f']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Envoyer,',
    '#submit' => array('recherche_stage_form_submit'),
  );

  $form['f']['coresp'] = array(
             '#type' => 'textfield',
             '#title' => t('Rechercher un FORMATEUR'),
             '#description' => t('Saisissez quelques lettres du nom pour rechercher un formateur'),
             '#autocomplete_path' => 'autocomp/formateur2',
             '#prefix' => '<div class="ajaxform inline">',
             '#suffix' => '</div>',
             '#size' => 80, 
  );
  $form['f']['submit2'] = array(
    '#type' => 'submit',
    '#value' => 'Envoyer.',
    '#submit' => array('recherche_formateur_form_submit'),
  );

  if (user_access('gest_etablissements gaiabb')) {
    $etab_add_icon = array(
        'path' => drupal_get_path('module', 'gaiabb') . '/images/building_add.png',
        'alt' => 'etab_add',
        'title' => t('Ajouter un établissement manquant.'),
        'attributes' => array('class' => 'some-img', 'id' => 'my-img'));
    $form['f']['lieu'] = array(
       '#markup' => t('<p>Ajouter un lieu&nbsp;').l(theme('image',$etab_add_icon),'etablissement/',
                                      array('attributes' =>  array('class' => 'anchor-class'), 'html' => TRUE)) . '</p>',
        );
  };
  return $form;
}


/**
 * ----> Submit
 */
function recherche_stage_form_submit($form, &$form_state) {
  $url = url('gest_module/2/' . $form_state['values']['comodu'],
                            array(
                                'absolute' => TRUE));
  $form_state['redirect'] = $url;
}


/**
 * ----> Submit
 */
function recherche_formateur_form_submit($form, &$form_state) {
  $url = url('formateur/' . $form_state['values']['coresp'],
                            array(
                                'absolute' => TRUE));
  $form_state['redirect'] = $url;
}

/**
 * Fonctions d' ** AUTOCOMPLETE AJAX STAGES **
 */
function _gaiabb_autocompstage($string) {

  $matches = array();
  $query = db_select('gbb_gdisp', 'd');
  $query  ->leftjoin('gbb_gmodu', 'm', 'm.co_disp = d.co_disp');
  $query  ->addField('d', 'id_disp', 'id_disp');
  $query  ->addField('d', 'lib', 'lib');
  $query  ->addField('m', 'lib', 'libmodu');
  $query  ->addField('m', 'co_modu', 'co_modu');
  $query  ->condition('id_disp', db_like('12') . '%', 'LIKE')
          ->condition(db_or()->condition('id_disp', '%' . db_like($string) . '%', 'LIKE')
                             ->condition('m.lib', '%' . db_like($string) . '%', 'LIKE')
                             ->condition('m.co_modu', '%' . db_like($string) . '%', 'LIKE')
                     )
    ->distinct()
    ->range(0, 10);
  $result = $query->execute();
  foreach ($result as $r) {
    $matches[$r->co_modu] = $r->id_disp."/".$r->co_modu." ".$r->libmodu;
  }
  $count = $result->rowCount();
  if ($count > 9) $matches[' '] = '..... Et bien d\'autres .....';
  drupal_json_output($matches);
}



/**
 * Fonctions d' ** AUTOCOMPLETE AJAX FORMATEURS **
 */
function _gaiabb_autocompform2($string) {

  $matches = array();
  $result = db_select('gbb_gresp_dafor', 'c')
    ->fields('c', array('co_resp', 'nomu', 'prenom'))
    ->condition('nomu', '%' . db_like($string) . '%', 'LIKE')
    ->range(0, 10)
    ->execute();
  foreach ($result as $r) {
    $matches[$r->co_resp] = $r->prenom . ' ' . $r->nomu;
  }
  $count = $result->rowCount();
  if ($count > 9) $matches[' '] = '..... Et bien d\'autres .....';
  drupal_json_output($matches);
}
