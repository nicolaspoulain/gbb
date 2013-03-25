<?php
/**
 * @file
 *
 * Composition de la page de gestion des évaluations
 *
 */

/**
 * Fonction d'appel aux differents elements composant la page
 * affichant une liste de responsables et les stages qui leur
 * associes
 */
function gbb_offres_view($args = 'NaN') {
  // Charge la feuille des style et les fichiers javascript pour le module
  drupal_add_css(drupal_get_path('module', 'gbb') . '/css/gbb.css',
                 array('group' => CSS_SYSTEM -1 , 'preprocess' => FALSE));
  drupal_add_library('system', 'drupal.collapse');

  // Construction de la page
  $build = array();
  $build['search'] = render(drupal_get_form('gbb_offres_filter',$args));
  $build['tab'] = gbb_offres_tab($args);

  return implode('', $build);
}

function gbb_offres_filter($form, &$form_state, $args) {
  $form['#method'] = 'get';
  $form['f'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filtrer'),
    '#attributes' => array('class' => array('collapsible', 
                                            'collapsed')),
  );

  $form['f']['co_orie'] = array(
    '#type' => 'textfield',
    '#title' => t('Orientation'),
    '#default_value' => (isset($_GET['co_orie']))? $_GET['co_orie'] : '',
    '#description' => t('p.ex.:PR70'),
    '#size' => 20, 
  );
   $form['f']['no_offre'] = array(
    '#type' => 'textfield',
    '#title' => t('NoOffre'),
    '#default_value' => (isset($_GET['no_offre']))? $_GET['no_offre'] : '',
    '#description' => t('p.ex.:20130020'),
    '#size' => 20, 
  );
   $form['f']['nomu'] = array(
    '#type' => 'textfield',
    '#title' => t('Resp. (dispo)'),
    '#default_value' => (isset($_GET['nomu']))? $_GET['nomu'] : '',
    '#description' => t('p.ex.:DENISE'),
    '#size' => 20, 
  );
   $form['f']['nomu2'] = array(
    '#type' => 'textfield',
    '#title' => t('Resp. (module)'),
    '#default_value' => (isset($_GET['nomu2']))? $_GET['nomu2'] : '',
    '#description' => t('p.ex.:POULAIN'),
    '#size' => 20, 
  );
   $form['f']['disc'] = array(
    '#type' => 'textfield',
    '#title' => t('Disc/Domaine'),
    '#default_value' => (isset($_GET['disc']))? $_GET['disc'] : '',
    '#description' => t('p.ex.:TICE'),
    '#size' => 20, 
  );
   $form['f']['co_tpla'] = array(
    '#type' => 'textfield',
    '#title' => t('Type de plan'),
    '#default_value' => (isset($_GET['co_tpla']))? $_GET['co_tpla'] : '',
    '#description' => t('p.ex.:S'),
    '#size' => 20, 
  );
  $form['f']['submit_co_modu'] = array(
    '#type' => 'submit',
    '#value' => 'Envoyer',
  );

  return $form;
}




function gbb_offres_tab($args) {

  $header = array(
    'nomu' => t('Interlocuteur dispo'),
    'co_tpla' => t('Type de Plan'),
    'co_orie' => t('Code Orientation'),
    array('data' => t('Disc/Domaine'), 'class' => 'jaune'),
    'no_offre' => t('No Offre/ Code Module'),
    'libl' => t('Titre'),
    'nomu2' => t('Resp. péda.'),
    'lmoda' => t('Modalités'),
    'lcomm' => t('Commenatires (GAIA)'),
    'nb_groupe' => t('Nbre groupe'),
    'duree_prev' => t('Duree prév. (h)'),
    array('data' => t('Nbre HP'), 'class' => 'jaune'),
    array('data' => t('Nbre vac.'), 'class' => 'jaune'),
    array('data' => t('Hors CDC'), 'class' => 'jaune'),
    array('data' => t('Priorité Nat.'), 'class' => 'jaune'),
    array('data' => t('Priorité Aca.'), 'class' => 'jaune'),
    array('data' => t('Offre nouvelle'), 'class' => 'jaune'),
    array('data' => t('&nbsp;&nbsp;&nbsp;Observations&nbsp;&nbsp;&nbsp;'), 'class' => 'jaune'),
    array('data' => t('Position'), 'CLASS' => 'jaune'),
  );
  // Number of records (or nodes, in our case) to show per page.
  $count = 20;

  $query = db_select('gbb_gdiof', 'd');
  $query ->leftjoin('gbb_gmoof', 'm',
                    'd.co_offre = m.co_offre');
  $query ->leftjoin('gbb_gdiof_dafor', 'dd',
                    'm.co_omodu = dd.co_omodu');
  $query ->leftjoin('gbb_gresp', 'r',
                    'd.co_resp = r.co_resp AND r.co_degre = 2');
  $query ->leftjoin('gbb_gresp', 'r2',
                    'm.co_resp = r2.co_resp AND r2.co_degre = 2');
  $query ->condition('d.no_offre', '20130000', '>')
         ->fields('d', array('no_offre', 'co_tpla', 'co_orie'))
         ->fields('m', array('libl', 'co_omodu', 'lmoda', 'lcomm','nb_groupe','duree_prev'))
         ->fields('r', array('nomu'));
  $query ->addField('r2', 'nomu', 'nomu2');
  $query ->fields('dd', array('disc','nb_hp','nb_vac','nb_gr','hors_cdc',
                              'prio_nat','prio_aca','offre_new','comment','position' ));
  $query = $query->extend('PagerDefault')->limit($count);
  $query ->distinct();

  if (isset($_GET['co_orie']) && $_GET['co_orie']!='') $query ->condition('d.co_orie', $_GET['co_orie'], 'LIKE');
  if (isset($_GET['no_offre']) && $_GET['no_offre']!='') $query ->condition('d.no_offre', $_GET['no_offre'], '=');
  if (isset($_GET['nomu']) && $_GET['nomu']!='') $query ->condition('r.nomu', $_GET['nomu'], 'LIKE');
  if (isset($_GET['nomu2']) && $_GET['nomu2']!='') $query ->condition('r2.nomu', $_GET['nomu2'], 'LIKE');
  if (isset($_GET['co_tpla']) && $_GET['co_tpla']!='') $query ->condition('d.co_tpla', $_GET['co_tpla'], 'LIKE');
  if (isset($_GET['disc']) && $_GET['disc']!='') $query ->condition('dd.disc', $_GET['disc'], 'LIKE');

  $result = $query->execute();
  $nb_off = $result->rowCount();

  $rows = array();
  foreach ($result as $r) {
    $d = render(drupal_get_form('gbb_offres_disc_form',
                                $r->disc, $r->co_omodu)) ;
    $hp = render(drupal_get_form('gbb_offres_nb_hp_form',
                                $r->nb_hp, $r->co_omodu)) ;
    $vac = render(drupal_get_form('gbb_offres_nb_vac_form',
                                $r->nb_vac, $r->co_omodu)) ;
    $hors_cdc = render(drupal_get_form('gbb_offres_hors_cdc_form',
                                $r->hors_cdc, $r->co_omodu)) ;
    $prio_nat = render(drupal_get_form('gbb_offres_prio_nat_form',
                                $r->prio_nat, $r->co_omodu)) ;
    $prio_aca = render(drupal_get_form('gbb_offres_prio_aca_form',
                                $r->prio_aca, $r->co_omodu)) ;
    $offre_new = render(drupal_get_form('gbb_offres_offre_new_form',
                                $r->offre_new, $r->co_omodu)) ;
    $comment = render(drupal_get_form('gbb_offres_comment_form',
                                $r->comment, $r->co_omodu)) ;
    $position = render(drupal_get_form('gbb_offres_position_form',
                                $r->position, $r->co_omodu)) ;

    $rows[] = array(
                array('data' => $r->nomu),
                array('data' => $r->co_tpla),
                array('data' => $r->co_orie),
                array('data' => $d),
                array('data' => $r->no_offre.' / '.$r->co_omodu),
                array('data' => $r->libl),
                array('data' => $r->nomu2),
                array('data' => $r->lmoda),
                array('data' => $r->lcomm),
                array('data' => $r->nb_groupe),
                array('data' => $r->duree_prev),
                array('data' => $hp),
                array('data' => $vac),
                array('data' => $hors_cdc),
                array('data' => $prio_nat),
                array('data' => $prio_aca),
                array('data' => $offre_new),
                array('data' => $comment),
                array('data' => $position),
              );
  }
  $output = theme('table', 
                  array('header' => $header,
                        'rows'   => $rows
                  ));
  return $output . theme('pager', array('tags' => array()));
}

function gbb_offres_disc_form($form, &$form_state, $disc, $co_omodu) {
  $form['f']['co_omodu']  = array('#type' => 'hidden','#value' => $co_omodu );
  $form['f']['disc'] = array(
    '#type' => 'textfield', '#size' => 15,
    '#default_value' => $disc,
    '#autocomplete_path' => 'autocomp/disc',
    '#prefix' => '<fieldset class="ajaxform inline">',
    '#suffix' => '</fieldset>',
    '#ajax' => array(
      'callback' => 'gbb_offres_disc_form_submit',
      'progress' => array('type' => 'throbber', 'message' => '')),);
  return $form;
}
function gbb_offres_disc_form_submit($form, &$form_state) {
  db_merge('gbb_gdiof_dafor')
    ->key(array('co_omodu' => $form_state['values']['co_omodu']))
    ->fields(array('disc'  => $form_state['values']['disc'],
  ))->execute();
}

function gbb_offres_nb_hp_form($form, &$form_state, $nb_hp, $co_omodu) {
  $form['f']['co_omodu']  = array('#type' => 'hidden','#value' => $co_omodu );
  $form['f']['nb_hp'] = array(
    '#type' => 'textfield', '#size' => 1,
    '#default_value' => $nb_hp,
    '#ajax' => array(
      'callback' => 'gbb_offres_nb_hp_form_submit',
      'progress' => array('type' => 'throbber', 'message' => '')),);
  return $form;
}
function gbb_offres_nb_hp_form_submit($form, &$form_state) {
  db_merge('gbb_gdiof_dafor')
    ->key(array('co_omodu' => $form_state['values']['co_omodu']))
    ->fields(array('nb_hp'  => $form_state['values']['nb_hp'],
  ))->execute();
}

function gbb_offres_nb_vac_form($form, &$form_state, $nb_vac, $co_omodu) {
  $form['f']['co_omodu']  = array('#type' => 'hidden','#value' => $co_omodu );
  $form['f']['nb_vac'] = array(
    '#type' => 'textfield', '#size' => 1,
    '#default_value' => $nb_vac,
    '#ajax' => array(
      'callback' => 'gbb_offres_nb_vac_form_submit',
      'progress' => array('type' => 'throbber', 'message' => '')),);
  return $form;
}
function gbb_offres_nb_vac_form_submit($form, &$form_state) {
  db_merge('gbb_gdiof_dafor')
    ->key(array('co_omodu' => $form_state['values']['co_omodu']))
    ->fields(array('nb_vac'  => $form_state['values']['nb_vac'],
  ))->execute();
}

function gbb_offres_hors_cdc_form($form, &$form_state, $hors_cdc, $co_omodu) {
  $form['f']['co_omodu']  = array('#type' => 'hidden','#value' => $co_omodu );
  $form['f']['hors_cdc'] = array(
    '#type' => 'checkbox', '#size' => 1,
    '#default_value' => $hors_cdc,
    '#ajax' => array(
      'callback' => 'gbb_offres_hors_cdc_form_submit',
      'progress' => array('type' => 'throbber', 'message' => '')),);
  return $form;
}
function gbb_offres_hors_cdc_form_submit($form, &$form_state) {
  db_merge('gbb_gdiof_dafor')
    ->key(array('co_omodu' => $form_state['values']['co_omodu']))
    ->fields(array('hors_cdc'  => $form_state['values']['hors_cdc'],
  ))->execute();
}

function gbb_offres_prio_nat_form($form, &$form_state, $prio_nat, $co_omodu) {
  $form['f']['co_omodu']  = array('#type' => 'hidden','#value' => $co_omodu );
  $form['f']['prio_nat'] = array(
    '#type' => 'select', 
    '#options' => array('Q0','Q1','Q2','Q3','Q4','Q5','Q6','Q7','Q8','Q9','0'),
    '#default_value' => $prio_nat,
    '#ajax' => array(
      'callback' => 'gbb_offres_prio_nat_form_submit',
      'progress' => array('type' => 'throbber', 'message' => '')),);
  return $form;
}
function gbb_offres_prio_nat_form_submit($form, &$form_state) {
  db_merge('gbb_gdiof_dafor')
    ->key(array('co_omodu' => $form_state['values']['co_omodu']))
    ->fields(array('prio_nat'  => $form_state['values']['prio_nat'],
  ))->execute();
}

function gbb_offres_prio_aca_form($form, &$form_state, $prio_aca, $co_omodu) {
  $form['f']['co_omodu']  = array('#type' => 'hidden','#value' => $co_omodu );
  $form['f']['prio_aca'] = array(
    '#type' => 'select',
    '#options' => array(0,1,2,3,4,5),
    '#default_value' => $prio_aca,
    '#ajax' => array(
      'callback' => 'gbb_offres_prio_aca_form_submit',
      'progress' => array('type' => 'throbber', 'message' => '')),);
  return $form;
}
function gbb_offres_prio_aca_form_submit($form, &$form_state) {
  db_merge('gbb_gdiof_dafor')
    ->key(array('co_omodu' => $form_state['values']['co_omodu']))
    ->fields(array('prio_aca'  => $form_state['values']['prio_aca'],
  ))->execute();
}

function gbb_offres_offre_new_form($form, &$form_state, $offre_new, $co_omodu) {
  $form['f']['co_omodu']  = array('#type' => 'hidden','#value' => $co_omodu );
  $form['f']['offre_new'] = array(
    '#type' => 'checkbox', '#size' => 1,
    '#default_value' => $offre_new,
    '#ajax' => array(
      'callback' => 'gbb_offres_offre_new_form_submit',
      'progress' => array('type' => 'throbber', 'message' => '')),);
  return $form;
}
function gbb_offres_offre_new_form_submit($form, &$form_state) {
  db_merge('gbb_gdiof_dafor')
    ->key(array('co_omodu' => $form_state['values']['co_omodu']))
    ->fields(array('offre_new'  => $form_state['values']['offre_new'],
  ))->execute();
}

function gbb_offres_comment_form($form, &$form_state, $comment, $co_omodu) {
  $form['f']['co_omodu']  = array('#type' => 'hidden','#value' => $co_omodu );
  $form['f']['comment'] = array(
    '#type' => 'textarea', 
    '#cols' => 20, '#rows' => 2,
    '#default_value' => $comment,
    '#ajax' => array(
      'callback' => 'gbb_offres_comment_form_submit',
      'progress' => array('type' => 'throbber', 'message' => '')),);
  return $form;
}
function gbb_offres_comment_form_submit($form, &$form_state) {
  db_merge('gbb_gdiof_dafor')
    ->key(array('co_omodu' => $form_state['values']['co_omodu']))
    ->fields(array('comment' => $form_state['values']['comment'],
  ))->execute();
}

function gbb_offres_position_form($form, &$form_state, $position, $co_omodu) {
  $form['f']['co_omodu']  = array('#type' => 'hidden','#value' => $co_omodu );
  $form['f']['position'] = array(
    '#type' => 'textfield', '#size' => 4,
    '#default_value' => $position,
    '#ajax' => array(
      'callback' => 'gbb_offres_position_form_submit',
      'progress' => array('type' => 'throbber', 'message' => '')),);
  return $form;
}
function gbb_offres_position_form_submit($form, &$form_state) {
  db_merge('gbb_gdiof_dafor')
    ->key(array('co_omodu' => $form_state['values']['co_omodu']))
    ->fields(array('position' => $form_state['values']['position'],
  ))->execute();
}



/**
 * Fonctions d' ** AUTOCOMPLETE AJAX RESPONSABLES **
 */
function _gbb_autocompdisc($string) {

  $matches = array();
  $result = db_select('gbb_gdiof_dafor', 'c')
    ->fields('c', array('disc'))
    ->condition('disc', '%' . db_like($string) . '%', 'LIKE')
    ->distinct()
    ->range(0, 10)
    ->execute();
  foreach ($result as $r) {
    $matches[$r->disc] = $r->disc;
  }
  $count = $result->rowCount();
  if ($count > 9) $matches[' '] = '..... Et bien d\'autres .....';
  drupal_json_output($matches);
}

