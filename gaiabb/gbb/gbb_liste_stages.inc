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
function gbb_liste_view($args = 'NaN') {
  // Charge la feuille des style et les fichiers javascript pour le module
  drupal_add_css(drupal_get_path('module', 'gbb') . '/css/gbb.css', 
                 array('group' => CSS_SYSTEM -1 , 'preprocess' => FALSE));
  drupal_add_library('system', 'drupal.collapse');

  // Construction de la page
  $build = array();
  drupal_add_js(drupal_get_path('module', 'gbb') . '/js/jquery.dataTables.js', 
                array('group' => CSS_DEFAULT, 'every_page' => TRUE));
  drupal_add_js(drupal_get_path('module', 'gbb') . '/js/jquery.dataTables.load.js',
                array('group' => CSS_DEFAULT, 'every_page' => TRUE));

  $build['search'] = render(drupal_get_form('gbb_liste_stages_search',$args));
  $build['choix']  = render(drupal_get_form('gbb_liste_stages_choix', $args));
  $build['liste']  = gbb_liste_stages_liste($args);
  return implode('', $build);
  return "Redirigez-vous en Cliquant sur <a href=\"/bb/liste/14/2/0\">nouveau B&B</a>";
}

/**
 * Fonction d'appel aux deux views d'affichage des stages à
 * traiter
 */
function gbb_atraiter_view($args = 'NaN') {
  // Charge la feuille des style et les fichiers javascript pour le module
  drupal_add_css(drupal_get_path('module', 'gbb') . '/css/gbb.css', 
                 array('group' => CSS_SYSTEM -1 , 'preprocess' => FALSE));
  drupal_add_library('system', 'drupal.collapse');

  // Construction de la page
  $build = array();
  drupal_add_js(drupal_get_path('module', 'gbb') . '/js/jquery.dataTables.js', 
                array('group' => CSS_DEFAULT, 'every_page' => TRUE));
  drupal_add_js(drupal_get_path('module', 'gbb') . '/js/jquery.dataTables.load.js',
                array('group' => CSS_DEFAULT, 'every_page' => TRUE));

  $build['choix']  = render(drupal_get_form('gbb_atraiter_choix', $args));
  return implode('', $build);
}

/**
 * Formulaire de ** FORMULAIRE DE RECHERCHE TOUS AZIMUTS **
 */
function gbb_liste_stages_search($form, &$form_state, $args) {
  $form['f'] = array(
    '#type' => 'fieldset',
    '#title' => t('Recherche tous azimuts'),
    '#attributes' => array('class' => array('sessions',
                                            'recherchestage', 
                                            'collapsible', 
                                            'collapsed')),
  );

  $form['f']['co_modu'] = array(
    '#type' => 'textfield',
    '#title' => t('Rechercher un STAGE'),
    '#description' => t('Saisissez quelques lettres du titre 
                         (ou chiffres du numéro) pour rechercher un stage'),
    '#autocomplete_path' => 'autocomp/stage',
    '#element_validate' => array('element_validate_integer_positive'),
    '#prefix' => '<div class="ajaxform inline">',
    '#suffix' => '</div>',
    '#size' => 80, 
  );
  $form['f']['submit_co_modu'] = array(
    '#type' => 'submit',
    '#value' => 'Envoyer,',
    '#submit' => array('recherche_stage_submit'),
  );

  $form['f']['co_resp'] = array(
    '#type' => 'textfield',
    '#title' => t('Rechercher un FORMATEUR'),
    '#description' => t('Saisissez quelques lettres du nom 
                         pour rechercher un formateur'),
    '#autocomplete_path' => 'autocomp/formateur2',
    '#element_validate' => array('element_validate_integer_positive'),
    '#prefix' => '<div class="ajaxform inline">',
    '#suffix' => '</div>',
    '#size' => 80, 
  );
  $form['f']['submit_co_resp'] = array(
    '#type' => 'submit',
    '#value' => 'Envoyer.',
    '#submit' => array('recherche_formatr_submit'),
  );

  if (user_access('gest_etablissements gaiabb')) {
    $form['f']['lieu'] = array(
      '#markup' => t('<p>Ajouter un lieu&nbsp;') .
                     l( theme('image',variable_get('_gbb_etab_add_icon')),
                        'etablissement/',
                        array('attributes' =>  array('class' => 'anchor-class'),
                                                     'html' => TRUE)) . '</p>'.
                   t('<p>Ajouter un formateur&nbsp;') .
                     l( theme('image',variable_get('_gbb_user_add_icon')),
                     'formateur/add',
                     array('attributes' => array('class' => 'anchor-class'),
                                                 'html' => TRUE))
    );
  } else {
    $form['f']['lieu'] = array(
      '#markup' => 
                   t('<p>Ajouter un formateur&nbsp;') .
                     l( theme('image',variable_get('_gbb_user_add_icon')),
                     'formateur/add',
                     array('attributes' => array('class' => 'anchor-class'),
                                                 'html' => TRUE))
    );

  };
  return $form;
}

/**
 * ----> Submit
 */
function recherche_stage_submit($form, &$form_state) {
  $form_state['redirect'] = url('module/2/' . $form_state['values']['co_modu'],
                                array('absolute' => TRUE));
}
function recherche_formatr_submit($form, &$form_state) {
  $form_state['redirect'] = url('formateur/' . $form_state['values']['co_resp'],
                                array('absolute' => TRUE));
}

/**
 * Formulaire de ** CHOIX D'UN FORMATEUR OU D'UNE ORIENTATION ** 
 */
function gbb_atraiter_choix($form, &$form_state, $args) {

  $query = db_select('gbb_gdire', 'j');
  $query ->rightjoin('gbb_gdisp', 's', 
                     'j.co_disp=s.co_disp AND j.co_degre=s.co_degre');
  $query ->join('gbb_gresp', 'r', 
                'j.co_resp=r.co_resp AND j.co_degre=r.co_degre');
  $query ->condition('j.co_degre', 2, '=')
         ->condition('s.id_disp' , '14' . '%', 'LIKE')
         ->fields('j', array('co_resp'))
         ->fields('r', array('nomu'));
  if (arg(3) != 0) { 
    // si un co_tres est spécifié : on trie dessus
    $query->condition('j.co_tres' , 2, '=');
  }
  elseif (!user_access('gest_sessions gbb')  && arg(3) == 0) {
    // si pas de co_tres : resp orga par défaut pour les administratifs
    $query->condition('j.co_tres' , 2, '=');
  }
  else {
    // si pas de co_tres : tout par défaut pour les non administratifs
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
    '#title' => t('Service Administratif : Recherche des stages à traiter'),
    '#attributes' => array('class' => array('collapsible',)),
  );

  // ------------
  // Ici on récupère le conseiller par défaut si arg(4) est vide
  // cela pour caler le select sur le bon conseiller
  global $user;
  $query = db_select('field_data_field_nomu', 'u')
         ->condition('u.entity_id', $user->uid, '=')
         ->fields('u', array('field_nomu_value'));
  $nomu = ( isset($_GET['nomu']) )? $_GET['nomu'] : 'TOUS';
  if ($nomu=="") { $nomu = "TOUS";};


  $form['f']['people'] = array(
    '#type' => 'textfield',
    '#title' => t('Responsable organisationnel'),
    '#default_value' => $nomu,
    '#size' => 20,
    #'#element_validate' => array('_choix_conseiller_validate'),
    '#autocomplete_path' => 'autocomp/responsable',
    '#prefix' => '<br/><div class="ajaxform inline">',
    '#suffix' => '</div>&nbsp;',
  );

  $form['f']['submit'] = array('#type' => 'submit',
                               '#value' => 'Envoyer',);

  return $form;
}
/**
 * ----> Submit
 */
function gbb_atraiter_choix_submit($form, &$form_state) {
  $nomu = $form_state['values']['people'];
  if ($nomu=="TOUS") { $nomu = "";};
  $url = url('atraiter',
             array('query' => array(
                     'nomu' => $nomu), 
                     'absolute' => TRUE)
  );
  $form_state['redirect'] = $url;
}


/**
 * Formulaire de ** CHOIX D'UN FORMATEUR OU D'UNE ORIENTATION ** 
 */
function gbb_liste_stages_choix($form, &$form_state, $args) {

  $query = db_select('gbb_gdire', 'j');
  $query ->rightjoin('gbb_gdisp', 's', 
                     'j.co_disp=s.co_disp AND j.co_degre=s.co_degre');
  $query ->join('gbb_gresp', 'r', 
                'j.co_resp=r.co_resp AND j.co_degre=r.co_degre');
  $query ->condition('j.co_degre', arg(2), '=')
         ->condition('s.id_disp' , db_like(arg(1)) . '%', 'LIKE')
         ->fields('j', array('co_resp'))
         ->fields('r', array('nomu'));
  if (arg(3) != 0) { 
    // si un co_tres est spécifié : on trie dessus
    $query->condition('j.co_tres' , arg(3), '=');
  }
  elseif (!user_access('gest_sessions gbb')  && arg(3) == 0) {
    // si pas de co_tres : resp orga par défaut pour les administratifs
    $query->condition('j.co_tres' , 2, '=');
  }
  else {
    // si pas de co_tres : tout par défaut pour les non administratifs
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

  $options = array(0 => t('Tous («Vue Conseiller»)'), 
                   1 => t('En demande de traitement («Vue Administratif»)'));
  // le bouton radio coché par défaut est le choix dans le get s'il existe, 
  // sinon c'est selon les droits (conseiler ou administratif)
  $default = (isset($_GET['voir_encours']))? 
               $_GET['voir_encours'] : (!user_access('gest_convocs gbb')? 
                                         0 : 1);
  $form['f']['voir_encours'] = array(
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => $default,
    '#prefix' => '<p><div class="flotteDroite">',
    '#suffix' => '</div></p>',
  );

  $degre = array(1 => 'Premier',
                 2 => 'Second'  );
  $form['f']['codegre'] = array(
    '#type' => 'select',
    '#title' => t('Degré'),
    '#default_value' => arg(2),
    '#options' => $degre,
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>',
  );

  $periode = array( 10 => '2010-2011',
                    11 => '2011-2012',
                    12 => '2012-2013',
                    13 => '2013-2014',
                    14 => '2014-2015'  );
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
  // Ici on récupère le conseiller par défaut si arg(4) est vide
  // cela pour caler le select sur le bon conseiller
  global $user;
  $query = db_select('field_data_field_nomu', 'u')
         ->condition('u.entity_id', $user->uid, '=')
         ->fields('u', array('field_nomu_value'));
  $nomu = ( arg(4)=='' )? $query->execute()->fetchField() : arg(4);

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
  $query ->rightjoin('gbb_gdisp', 'd', 'o.co_orie=d.co_orie');
  $query ->condition('d.co_degre', arg(2), '=')
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

  if (user_access('gest_sessions gbb')) {
    $tres = array( 0 => 'TOUS',
                // 1 => 'Interlocuteur',
                   2 => 'Resp. orga.',
                   3 => 'Resp. péda.'  );
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
  } else {
    $form['f']['cotres'] = array('#type' => 'hidden',
                                 '#value' => 2,      );
    $form['f']['coorie'] = array('#type' => 'hidden',
                                 '#value' => 0,      );
  };

  $form['f']['submit'] = array('#type' => 'submit',
                               '#value' => 'Envoyer',);

  $form['if']['infos']['#markup']= '<div class="infos">' . t('
    Saisissez quelques lettres ou chiffres 
    du stage pour réduire la liste.<br />
    Vous pouvez aussi utiliser les jokers : « paf* » ou « fil* » 
    pour réduire à un type de stage, 
    ou bien « ! » pour réduire aux stages prioritaires.<br/>
    <font color="red">NOUVEAU</font> : Entrez le nom d\'un formateur 
    pour réduire aux seuls stages où il intervient.
    ') . '</div>';

  return $form;
}

function _choix_conseiller_validate($element, &$form_state) {
  $query = db_select('gbb_gresp', 'r')
         ->fields('r', array('nomu'))
         ->condition('nomu', $element['#value'], '=')
         ->distinct();
  $num_rows = $query->countQuery()->execute()->fetchField();
  if ($num_rows == 0 && $element['#value'] != 'TOUS') {
    form_error($element, t('Le champ Conseiller ne peut être 
                qu\'une des valeurs proposées, et que vous aurez cliquée.'));
  }
} 

/**
 * ----> Submit
 */
function gbb_liste_stages_choix_submit($form, &$form_state) {
  $url = url('liste/' . $form_state['values']['year']
                . '/' . $form_state['values']['codegre']
                . '/' . $form_state['values']['cotres']
                . '/' . $form_state['values']['people']
                . '/' . $form_state['values']['coorie'],
             array('query' => array(
                     'voir_ferme' => $form_state['values']['voir_ferme'], 
                     'voir_encours' => $form_state['values']['voir_encours']), 
                     'absolute' => TRUE)
  );
  $form_state['redirect'] = $url;
}

/**
 * Fonction de ** LISTE DES STAGES ** pour un conseiller donné en argument
 */
function gbb_liste_stages_liste($args) {

  $query = db_select('gbb_gmodu', 'm');
  $query ->leftjoin('gbb_gmodu_plus', 'mp',
                    'mp.co_modu=m.co_modu AND mp.co_degre=m.co_degre');
  $query ->join('gbb_gdisp', 'd', 
                'd.co_disp = m.co_disp AND d.co_degre = m.co_degre');
  $query ->join('gbb_gdire', 'j', 
                'j.co_modu = m.co_modu AND j.co_degre = m.co_degre');
  $query ->fields('m',  array('co_modu', 'co_anmo'))
         ->fields('mp', array('module_alert', 'module_printed',
                              'prioritaire', 'organisation','color' ))
         ->fields('d',  array('co_disp', 'id_disp', 'co_tcan',
                              'co_camp', 'co_andi'));
  $query ->addField('m', 'lib', 'libm');
  $query ->addField('d', 'lib', 'libd');

  // -----------------------------------------------------------------
  // prise en compte de l'année, du degré, du orga/péda et orientation
  // -----------------------------------------------------------------
  $query  ->condition('id_disp',    db_like(arg(1)) . '%', 'LIKE')
          ->condition('m.co_degre', arg(2), '=');
  if (arg(3) && arg(3)!="0") {
    $query->condition('j.co_tres',  arg(3), '='); };
  if (arg(5) && arg(5)!="0") {
    $query->condition('d.co_orie',  arg(5), '='); };

  // -------------------------------------------
  // prise en compte du nom du conseiller
  // -------------------------------------------
  if ( arg(4)=='' ) { // On récupère le nomu par défaut.dans le profil
    global $user;
    $qu = db_select('field_data_field_nomu', 'u')
        ->condition('u.entity_id', $user->uid, '=')
        ->fields('u', array('field_nomu_value'));
    $nomu = $qu->execute()->fetchField();
  } else { $nomu = arg(4); }; // L'URL donne le nomu : on le prend

  if( $nomu != 'TOUS' ) { // On récupère les co_resp correspondants à *nomu
    $tab_resp = db_select('gbb_gresp', 'c')
              ->fields('c', array('co_resp'))
              ->condition('nomu', '' . db_like($nomu) . '', 'LIKE')
              ->condition('co_degre', arg(2), '=')
              ->distinct()
              ->execute()->fetchCol();
    $query->condition('j.co_resp', $tab_resp, 'IN');  // print_r($tab_resp);
 };
  // ----------------------------------------i---
  // prise en compte du voir les stages fermés
  // -------------------------------------------
  if ( !isset($_GET['voir_ferme']) || $_GET['voir_ferme']==0 ) {
    $query->condition('m.co_anmo', '', '=');
    //$query .= 'AND co_anmo = \'\'';
  }

  $query->orderBy('id_disp', 'ASC');
  $query->orderBy('co_modu', 'ASC');
  $query->distinct();
  $result = $query->execute();
  if ($result->rowCount() == 0) return 'Hum !';

  $libd_precedent= '';
  $rows = array();

  foreach ($result as $r) {
/*
(select count({gbb_file}.fid) from {gbb_file} JOIN {file_managed} ON {file_managed}.fid={gbb_file}.fid  Where co_modu=mp.co_modu AND co_degre=mp.co_degre AND status=1) as nb_pj
*/ 
    if (!isset($_GET['voir_encours']) OR $_GET['voir_encours'] == 0) {
      $qdt = db_select('gbb_session', 's');
      $qdt ->fields('s', array('sess_id'))
           ->condition('s.co_degre', arg(2), '=')
           ->condition('s.co_modu', $r->co_modu, '=')
           ->condition('s.en_attente', '0', '=');
      $qdt ->addExpression('MIN(date)','dateD');
      $qdt ->addExpression('MAX(date)','dateF');
      $dt = $qdt->execute()->fetchObject();
      $dateD = $dt->dateD;
      $dateF = $dt->dateF;
    } else {
      $qdt = db_select('gbb_session', 's');
      $qdt ->fields('s', array('sess_id'))
           ->condition('s.co_degre', arg(2), '=')
           ->condition('s.co_modu', $r->co_modu, '=')
           ->condition('s.en_attente', '0', '=')
           ->condition('s.convoc_sent', '0', '=');
      $qdt ->addExpression('MIN(date)','dateD');
      $dt = $qdt->execute()->fetchObject();
      $dateD = $dt->dateD;
      $dateF = '2061-07-16';
    };

    $qnea = db_select('gbb_session', 's');
    $qnea ->fields('s', array('sess_id'))
          ->condition('s.co_degre', arg(2), '=')
          ->condition('s.co_modu', $r->co_modu, '=')
          ->condition('s.en_attente', '0', '=')
          ->condition('s.convoc_sent', '0', '=');
    $nb_nea = $qnea->execute()->rowCount();

    $qea = db_select('gbb_session', 's');
    $qea ->fields('s', array('sess_id'))
         ->condition('s.co_degre', arg(2), '=')
         ->condition('s.co_modu', $r->co_modu, '=')
         ->condition('s.en_attente', '1', '=');
    $nb_ea = $qea->execute()->rowCount();

    $qs = db_select('gbb_session', 's');
    $qs ->fields('s', array('sess_id'))
        ->condition('s.co_degre', arg(2), '=')
        ->condition('s.co_modu', $r->co_modu, '=')
        ->condition('s.convoc_sent', '1', '=');
    $nb_s = $qs->execute()->rowCount();

    $qsa = db_select('gbb_session', 's');
    $qsa ->fields('s', array('sess_id'))
         ->condition('s.co_degre', arg(2), '=')
         ->condition('s.co_modu', $r->co_modu, '=')
         ->condition('s.session_alert', '1', '=');
    $nb_sa = $qsa->execute()->rowCount();

    $qpj = db_select('gbb_file', 'f');
    $qpj ->join('file_managed', 'fm', 'fm.fid=f.fid');
    $qpj ->fields('f', array('fid'))
         ->condition('f.co_degre', arg(2), '=')
         ->condition('f.co_modu', $r->co_modu, '=')
         ->condition('fm.status', '1', '=');
    $nb_pj = $qpj->execute()->rowCount();

    $dda = db_select('gbb_gmodu_plus', 'mp');
    $dda ->fields('mp', array('DA_date_depot'))
         ->condition('mp.co_degre', arg(2), '=')
         ->condition('mp.co_modu', $r->co_modu, '=');
    $ddt = $dda->execute()->fetchObject();
    $DAdd = ($ddt) ? $ddt->DA_date_depot : "NaN";

    $m_ouvert_ou_ferme = ($r->co_anmo == '')? '' : 'Ferme';
    $options_co_modu = array('attributes' =>
                                array('class' => array($m_ouvert_ou_ferme),
                                      'style' => 'background-color:'.$r->color,
                                      'id' => 'cool-id'         ),
                             'html' => FALSE,
    );
    $d_ouvert_ou_ferme = ($r->co_andi == '')? '' : 'Ferme';
    $can = array(1 => "", 2 => "", 3 => theme('image', variable_get('_gbb_pub_des')));
    $candidature = $can[$r->co_tcan];
    $paf_ou_fil = ($r->co_camp == 'BS')? 'fil' : 'paf';
    $libd_class = ($r->co_camp == 'BS')? 'fil' : 'paf';
    $libd_class = ($r->libd == $libd_precedent)? "gris" : $libd_class;
    // Les icônes à afficher
    $session_alert = ($nb_sa == 0)? '' :
        theme('image', variable_get('_gbb_session_alert_icon')) .
                       '<span class="nb_sa">' . $nb_sa . '</span>'.
                       '<span class="invisible">></span> ';
    $module_alert = ($r->module_alert==0)? '' :
        theme('image', variable_get('_gbb_module_alert_icon')) .
                       '<span class="nb_sa">*</span>'.
                       '<span class="invisible">></span> ';
    $NOT_en_attente = ($nb_nea == 0)? '' :
        theme('image', variable_get('_gbb_not_en_attente_icon')) .
                       '<span class="nb_sa">' . $nb_nea . '</span>'.
                       '<span class="invisible">></span> ';
    $en_attente = ($nb_ea == 0)? '' :
        theme('image', variable_get('_gbb_en_attente_icon')) .
                       '<span class="nb_sa">' . $nb_ea . '</span>';
    $voirEnCours = (isset($_GET['voir_encours']))?
                       (($_GET['voir_encours']==1)? TRUE : FALSE) : FALSE;
    $module_printed = ($r->module_printed)?
        theme('image', variable_get('_gbb_print_icon')) . ' ' : '';
    $prioritaire = (!$r->prioritaire)? '' :
        theme('image', variable_get('_gbb_prioritaire_icon')) .
                       '<span class="invisible">!</span> ';
    $convoc_sent = ($nb_s == 0)? '' :
        theme('image', variable_get('_gbb_convoc_sent_icon')) .
                       '<span class="nb_sa">' . $nb_s . '</span>';
    $attachement = ($nb_pj ==0)? '' :
        theme('image', variable_get('_gbb_attachement_icon')) .
                       '<span class="nb_pj">' . $nb_pj . '</span>';
    $plus  = ($r->organisation == '')? '' :
        '&nbsp;' .
        theme('image', variable_get('_gbb_comment_icon'));
    $rsp = db_select('gbb_gdire', 'd');
    $rsp ->fields('d', array('co_resp', 'co_tres'))
         ->condition('d.co_degre', arg(2), '=')
         ->condition('d.co_tres', 3, '=')
         ->condition('d.co_modu', $r->co_modu, '=');
    $rsp ->leftjoin('gbb_gresp', 'r',  
                    'r.co_resp  = d.co_resp AND r.co_degre = d.co_degre');
    $rsp ->fields('r', array('nomu', 'prenom'));
    $respo = $rsp->execute()->fetchObject();
    if ($paf_ou_fil=='paf') {
      $isnomu = $rsp->execute()->rowCount();
      if ($isnomu > 0) $plus .= " <i>(Resp.péda : ".$respo->nomu.")</i>";
    };
    $timer = theme('image', variable_get('_gbb_timer_green_icon'));
    if (strtotime($dateD) < time()) { // le premier jour dépassé
      $timer = theme('image', variable_get('_gbb_timer_red_icon'));
    } elseif(strtotime($dateD) < time() + 1296000) { // le premier jour est dans moins de 15 jours.
      $timer = theme('image', variable_get('_gbb_timer_orange_icon'));
    };
    if ($dateF != '2061-07-16' && strtotime($dateF) < time()) { // le dernier jour jour dépassé donc formation finie
      $timer = theme('image', variable_get('_gbb_timer_end_icon'));
    }

    if ($dateD > 0) {
      if ($dateD==$dateF || $dateF=='2061-07-16') {
        $dates = $timer . 
                 t( "&nbsp;" . 
                    strftime('%d/%m/%y', strtotime($dateD))) ; 
      } else {
        $dates = $timer . 
                 t( "&nbsp;" . 
                    strftime('%d/%m/%y', strtotime($dateD)) . 
                    "&rarr;" . 
                    strftime('%d/%m/%y', strtotime($dateF)));
      };
    } else { $dates =''; };
    $DA = "";
    if ($DAdd !="NaN" && $DAdd !="1970-01-01 00:00:00") {
      $DA = "&nbsp;" . theme('image', variable_get('_gbb_DA_rendu'))
                     . " " .strftime('%d/%m/%y', strtotime($DAdd));
    };
  $queryev = db_select('gbb_evaluation', 'e');
  $queryev ->condition('e.co_modu', $r->co_modu, '=')
           ->condition('e.co_degre', arg(2), '=')
           #->fields('e',  array('groupe','syntheseEvalForm'));
           ->fields('e',  array('groupe','remqAnalysesRD'));
  $resultev = $queryev->execute();
  $evalbilan = '';
  if (($resultev->rowCount() != 0)) {
    $evalbilan = "&nbsp;&nbsp;".theme('image', variable_get('_gbb_evals'));
    foreach ($resultev as $rrv) {
      #if ($rrv->syntheseEvalForm !='') {
      if ($rrv->remqAnalysesRD !='') {
        $evalbilan = "&nbsp;&nbsp;".theme('image', variable_get('_gbb_bilans'));
      };
    };
  };
    // La liste des sessions pour le module
    $qf = db_select('gbb_session', 's');
    $qf ->leftjoin('gbb_gresp_dafor', 'r',
                   'r.co_resp = s.co_resp AND r.co_degre=s.co_degre');
    $qf ->condition('s.co_degre', arg(2), '=')
        ->condition('s.co_modu', $r->co_modu, '=')
        ->fields('r', array('nomu', 'prenom'))
        ->fields('s', array('date', 'en_attente'));
    $fo = $qf->execute();

    $items = array();
    foreach ($fo as $f) {
      $items[] = ((!$f->en_attente)? '' :
                     theme('image', variable_get('_gbb_en_attente_icon')) ) .
                 ' ' . $f->date . ' - ' . $f->prenom . ' ' . $f->nomu;
    };
    $formateurs = theme('item_list', array('items' => $items));
  // ------------------------------------------------------------
  // prise en compte du choix radio Tous/EndemandeDeTraitement
  // ------------------------------------------------------------
/*         mp.module_alert = 1
           OR
           (select count(*) from {gbb_session} WHERE co_modu=m.co_modu AND co_degre=m.co_degre AND session_alert=1) > 0
           OR
           (select count(*) from {gbb_session} WHERE co_modu=m.co_modu AND co_degre=m.co_degre AND convoc_sent=1) <
           (select count(*) from {gbb_session} WHERE co_modu=m.co_modu AND co_degre=m.co_degre AND en_attente=0)
*/
   if (   ($r->module_alert==1 OR  $nb_sa >0 OR $nb_nea > 0) 
       OR $voirEnCours == 0 ) {

      $rows[] = array( 
                'col0' => array(
                  'data' => "<span class='invisible'>$paf_ou_fil*</span>" .
                            "<span class='$paf_ou_fil $d_ouvert_ou_ferme'>" .
                            "[" . strtoupper($paf_ou_fil) . "] $candidature ".
                            $r->id_disp . ' ' . t($r->libd) . '</span> '
                ),
                'col1' => array(
                  'data' => //$attachement .
                            $module_printed .
                            $convoc_sent .
                            $session_alert .
                            $NOT_en_attente .
                            $en_attente .
                            //$nb_nea.' '. $nb_s.
                            $module_alert .
                            $prioritaire,
                            'class' => 'nowrap',
                ),
                'col2' => array(
                  'data' => l( $r->co_modu, 
                              'module/' . arg(2) . '/' . $r->co_modu, 
                              $options_co_modu) . 
                            " <span class='$m_ouvert_ou_ferme'>" . 
                            t($r->libm) . $plus . '</span> ',
                ),
                'col3' => array(
                  'data' => nl2br($r->organisation). $formateurs ,
                ),
                'col4' => array(
                  'data' => $dates . $evalbilan . $DA,
                ),
    );
    $libd_precedent = $r->libd;
   }
  }
  $attributes = array('id' => 'table-id', 
                      'class' => array('display'));
  return theme('table', array('rows' => $rows, 
                              'attributes' => $attributes));
}


/**
 * Fonctions d' ** AUTOCOMPLETE AJAX RESPONSABLES **
 */
function _gbb_autocompresp($string) {

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
 * Fonctions d' ** AUTOCOMPLETE AJAX STAGES **
 */
function _gbb_autocompstage($string) {

  $matches = array();
  $query = db_select('gbb_gdisp', 'd');
  $query  ->leftjoin('gbb_gmodu', 'm', 'm.co_disp = d.co_disp');
  $query  ->addField('d', 'id_disp', 'id_disp');
  $query  ->addField('d', 'lib', 'lib');
  $query  ->addField('m', 'lib', 'libmodu');
  $query  ->addField('m', 'co_modu', 'co_modu');
  $query  ->condition('id_disp', db_like('14') . '%', 'LIKE')
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
function _gbb_autocompformatr2($string) {

  $matches = array();
  $result = db_select('gbb_gresp_dafor', 'c')
    ->fields('c', array('co_resp', 'nomu', 'prenom'))
    ->condition('nomu', '%' . db_like($string) . '%', 'LIKE')
    ->range(0, 10)
    ->execute();
  if (strpos($string,'++')) {
    $result = db_select('gbb_gresp_dafor', 'c')
      ->fields('c', array('co_resp', 'nomu', 'prenom'))
      ->condition('nomu', '%' . db_like(str_replace("++","",$string)) . '%', 'LIKE')
      ->execute();
  }
  foreach ($result as $r) {
    $matches[$r->co_resp] = $r->prenom . ' ' . $r->nomu;
  }
  $count = $result->rowCount();
  if ($count > 9 && !strpos($string,'++')) $matches[' '] = '..... Et bien d\'autres .....';
  drupal_json_output($matches);
}
