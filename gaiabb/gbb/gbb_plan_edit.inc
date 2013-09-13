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
function gbb_plan_edit_view($args = 'NaN') {
  // Charge la feuille des style et les fichiers javascript pour le module
  drupal_add_css(drupal_get_path('module', 'gbb') . '/css/gbb.css',
                 array('group' => CSS_SYSTEM -1 , 'preprocess' => FALSE));
  drupal_add_library('system', 'drupal.collapse');

  // Construction de la page
  $build = array();
  $build['search'] = render(drupal_get_form('gbb_plan_filter',$args));
  $build['tab'] = gbb_plan_tab($args);

  return implode('', $build);
}

function gbb_plan_filter($form, &$form_state, $args) {
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




function gbb_plan_tab($args) {

  $header = array(
    'nomu' => t('Interlocuteur dispo'),
    'co_tpla' => t('Type de Plan'),
    'co_orie' => t('Code Orientation'),
    'no_offre' => t('No Offre/ Code Module'),
    'libl' => t('Titre'),
    'nomu2' => t('Resp. péda.'),
    'lmoda' => t('Modalités'),
    'lcomm' => t('Commentaires (GAIA)'),
    'nb_groupe' => t('Nbre groupe'),
    'duree_prev' => t('Duree prév. (h)'),
    array('data' => t('&nbsp;&nbsp;&nbsp;Observations&nbsp;&nbsp;&nbsp;'), 'class' => 'jaune'),
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
  $query ->fields('dd', array('nb_hp','nb_vac','hors_cdc','iufm',
                              'nouv_offreur','prio_nat','prio_aca','offre_new',
                              'pub_des','comment' ));
  $query = $query->extend('PagerDefault')->limit($count);
  $query ->distinct();

  if (isset($_GET['co_orie']) && $_GET['co_orie']!='') $query ->condition('d.co_orie', $_GET['co_orie'], 'LIKE');
  if (isset($_GET['no_offre']) && $_GET['no_offre']!='') $query ->condition('d.no_offre', $_GET['no_offre'], '=');
  if (isset($_GET['nomu']) && $_GET['nomu']!='') $query ->condition('r.nomu', $_GET['nomu'], 'LIKE');
  if (isset($_GET['nomu2']) && $_GET['nomu2']!='') $query ->condition('r2.nomu', $_GET['nomu2'], 'LIKE');
  if (isset($_GET['co_tpla']) && $_GET['co_tpla']!='') $query ->condition('d.co_tpla', $_GET['co_tpla'], 'LIKE');

  $result = $query->execute();
  $nb_off = $result->rowCount();

  $rows = array();
  foreach ($result as $r) {
    $comment = render(drupal_get_form('gbb_plan_comment_form',
                                $r->comment, $r->co_omodu)) ;

    $rows[] = array(
                array('data' => $r->nomu),
                array('data' => $r->co_tpla),
                array('data' => $r->co_orie),
                array('data' => $r->no_offre.' / '.$r->co_omodu),
                array('data' => $r->libl),
                array('data' => $r->nomu2),
                array('data' => $r->lmoda),
                array('data' => $r->lcomm),
                array('data' => $r->nb_groupe),
                array('data' => $r->duree_prev),
                array('data' => $comment),
              );
  }
  $output = theme('table', 
                  array('header' => $header,
                        'rows'   => $rows
                  ));
  return $output . theme('pager', array('tags' => array()));
}

function gbb_plan_comment_form($form, &$form_state, $comment, $co_omodu) {
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
function gbb_plan_comment_form_submit($form, &$form_state) {
  db_merge('gbb_gdiof_dafor')
    ->key(array('co_omodu' => $form_state['values']['co_omodu']))
    ->fields(array('comment' => $form_state['values']['comment'],
  ))->execute();
}

