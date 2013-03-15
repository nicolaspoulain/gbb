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
  $build['tab'] = gbb_offres_tab($args);

  return implode('', $build);
}

function gbb_offres_tab($args) {

  $header = array(
    'nomu' => t('Interlocuteur dispo'),
    'co_tpla' => t('Type de Plan'),
    'co_orie' => t('Code Orientation'),
    'no_offre' => t('No Offre/ Code Module'),
    'libl' => t('Titre'),
    'nomu2' => t('Resp. péda.'),
    'disc' => t('Disc / Domaine'),
    'nb_hp' => t('Nbre HP'),
    'nb_vac' => t('Nbre vac.'),
    'nb_gr' => t('Nbre gr.'),
    'hors_cdc' => t('Hors CDC'),
    'prio_nat' => t('Priorité Nat.'),
    'prio_aca' => t('Priorité Aca.'),
    'offre_new' => t('Offre nouvelle'),
    'comment' => t('&nbsp;&nbsp;&nbsp;Commentaires&nbsp;&nbsp;&nbsp;'),
  );
  // Number of records (or nodes, in our case) to show per page.
  $count = 20;

  $query = db_select('gbb_gdiof', 'd');
  $query ->leftjoin('gbb_gdiof_dafor', 'dd',
                    'd.no_offre = dd.no_offre');
  $query ->leftjoin('gbb_gmoof', 'm',
                    'd.co_offre = m.co_offre');
  $query ->leftjoin('gbb_gresp', 'r',
                    'd.co_resp = r.co_resp AND r.co_degre = 2');
  $query ->leftjoin('gbb_gresp', 'r2',
                    'm.co_resp = r2.co_resp AND r2.co_degre = 2');
  $query ->condition('d.no_offre', '20130000', '>')
         ->fields('d', array('no_offre', 'co_tpla', 'co_orie'))
         ->fields('m', array('libl', 'co_omodu'))
         ->fields('r', array('nomu'));
  $query ->addField('r2', 'nomu', 'nomu2');
  $query ->fields('dd', array('disc','nb_hp','nb_vac','nb_gr','hors_cdc',
                              'prio_nat','prio_aca','offre_new','comment' ));
  $query = $query->extend('PagerDefault')->limit($count);
  $result = $query->execute();
  $nb_off = $result->rowCount();

  $rows = array();
  foreach ($result as $r) {
    $d = render(drupal_get_form('gbb_offres_disc_form',
                                $r->disc, $r->no_offre)) ;
    $hp = render(drupal_get_form('gbb_offres_nb_hp_form',
                                $r->nb_hp, $r->no_offre)) ;
    $vac = render(drupal_get_form('gbb_offres_nb_vac_form',
                                $r->nb_vac, $r->no_offre)) ;
    $gr = render(drupal_get_form('gbb_offres_nb_gr_form',
                                $r->nb_gr, $r->no_offre)) ;
    $hors_cdc = render(drupal_get_form('gbb_offres_hors_cdc_form',
                                $r->hors_cdc, $r->no_offre)) ;
    $prio_nat = render(drupal_get_form('gbb_offres_prio_nat_form',
                                $r->prio_nat, $r->no_offre)) ;
    $prio_aca = render(drupal_get_form('gbb_offres_prio_aca_form',
                                $r->prio_aca, $r->no_offre)) ;
    $offre_new = render(drupal_get_form('gbb_offres_offre_new_form',
                                $r->offre_new, $r->no_offre)) ;
    $comment = render(drupal_get_form('gbb_offres_comment_form',
                                $r->comment, $r->no_offre)) ;

    $rows[] = array(
                array('data' => $r->nomu),
                array('data' => $r->co_tpla),
                array('data' => $r->co_orie),
                array('data' => $r->no_offre.' / '.$r->co_omodu),
                array('data' => $r->libl),
                array('data' => $r->nomu2),
                array('data' => $d),
                array('data' => $hp),
                array('data' => $vac),
                array('data' => $gr),
                array('data' => $hors_cdc),
                array('data' => $prio_nat),
                array('data' => $prio_aca),
                array('data' => $offre_new),
                array('data' => $comment),
              );
  }
  $output = theme('table', 
                  array('header' => $header,
                        'rows'   => $rows
                  ));
  return $output . theme('pager', array('tags' => array()));
}

function gbb_offres_disc_form($form, &$form_state, $disc, $no_offre) {
  $form['f']['no_offre']  = array('#type' => 'hidden','#value' => $no_offre );
  $form['f']['disc'] = array(
    '#type' => 'textfield', '#size' => 8,
    '#default_value' => $disc,
    '#ajax' => array(
      'callback' => 'gbb_offres_disc_form_submit',
      'progress' => array('type' => 'throbber', 'message' => '')),);
  return $form;
}
function gbb_offres_disc_form_submit($form, &$form_state) {
  db_merge('gbb_gdiof_dafor')
    ->key(array('no_offre' => $form_state['values']['no_offre']))
    ->fields(array('disc'  => $form_state['values']['disc'],
  ))->execute();
}

function gbb_offres_nb_hp_form($form, &$form_state, $nb_hp, $no_offre) {
  $form['f']['no_offre']  = array('#type' => 'hidden','#value' => $no_offre );
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
    ->key(array('no_offre' => $form_state['values']['no_offre']))
    ->fields(array('nb_hp'  => $form_state['values']['nb_hp'],
  ))->execute();
}

function gbb_offres_nb_vac_form($form, &$form_state, $nb_vac, $no_offre) {
  $form['f']['no_offre']  = array('#type' => 'hidden','#value' => $no_offre );
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
    ->key(array('no_offre' => $form_state['values']['no_offre']))
    ->fields(array('nb_vac'  => $form_state['values']['nb_vac'],
  ))->execute();
}

function gbb_offres_nb_gr_form($form, &$form_state, $nb_gr, $no_offre) {
  $form['f']['no_offre']  = array('#type' => 'hidden','#value' => $no_offre );
  $form['f']['nb_gr'] = array(
    '#type' => 'textfield', '#size' => 1,
    '#default_value' => $nb_gr,
    '#ajax' => array(
      'callback' => 'gbb_offres_nb_hp_form_submit',
      'progress' => array('type' => 'throbber', 'message' => '')),);
  return $form;
}
function gbb_offres_nb_gr_form_submit($form, &$form_state) {
  db_merge('gbb_gdiof_dafor')
    ->key(array('no_offre' => $form_state['values']['no_offre']))
    ->fields(array('nb_gr'  => $form_state['values']['nb_gr'],
  ))->execute();
}

function gbb_offres_hors_cdc_form($form, &$form_state, $hors_cdc, $no_offre) {
  $form['f']['no_offre']  = array('#type' => 'hidden','#value' => $no_offre );
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
    ->key(array('no_offre' => $form_state['values']['no_offre']))
    ->fields(array('hors_cdc'  => $form_state['values']['hors_cdc'],
  ))->execute();
}

function gbb_offres_prio_nat_form($form, &$form_state, $prio_nat, $no_offre) {
  $form['f']['no_offre']  = array('#type' => 'hidden','#value' => $no_offre );
  $form['f']['prio_nat'] = array(
    '#type' => 'select', 
    '#options' => array('Q0','Q1','Q2','Q3','Q4','Q5','Q6','Q7','Q8','Q9'),
    '#default_value' => $prio_nat,
    '#ajax' => array(
      'callback' => 'gbb_offres_prio_nat_form_submit',
      'progress' => array('type' => 'throbber', 'message' => '')),);
  return $form;
}
function gbb_offres_prio_nat_form_submit($form, &$form_state) {
  db_merge('gbb_gdiof_dafor')
    ->key(array('no_offre' => $form_state['values']['no_offre']))
    ->fields(array('prio_nat'  => $form_state['values']['prio_nat'],
  ))->execute();
}

function gbb_offres_prio_aca_form($form, &$form_state, $prio_aca, $no_offre) {
  $form['f']['no_offre']  = array('#type' => 'hidden','#value' => $no_offre );
  $form['f']['prio_aca'] = array(
    '#type' => 'select',
    '#options' => array(1,2,3,4,5,6,7,8,9),
    '#default_value' => $prio_aca,
    '#ajax' => array(
      'callback' => 'gbb_offres_prio_aca_form_submit',
      'progress' => array('type' => 'throbber', 'message' => '')),);
  return $form;
}
function gbb_offres_prio_aca_form_submit($form, &$form_state) {
  db_merge('gbb_gdiof_dafor')
    ->key(array('no_offre' => $form_state['values']['no_offre']))
    ->fields(array('prio_aca'  => $form_state['values']['prio_aca'],
  ))->execute();
}

function gbb_offres_offre_new_form($form, &$form_state, $offre_new, $no_offre) {
  $form['f']['no_offre']  = array('#type' => 'hidden','#value' => $no_offre );
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
    ->key(array('no_offre' => $form_state['values']['no_offre']))
    ->fields(array('offre_new'  => $form_state['values']['offre_new'],
  ))->execute();
}

function gbb_offres_comment_form($form, &$form_state, $comment, $no_offre) {
  $form['f']['no_offre']  = array('#type' => 'hidden','#value' => $no_offre );
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
    ->key(array('no_offre' => $form_state['values']['no_offre']))
    ->fields(array('comment' => $form_state['values']['comment'],
  ))->execute();
}



