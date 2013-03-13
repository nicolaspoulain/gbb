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
    'no_offre' => t('No Offre'),
    'libl' => t('Titre'),
    'disc' => t('Disc/Domaine'),
    'nb_hp' => t('Nbre HP'),
    'nb_vac' => t('Nbre vac.'),
    'nb_gr' => t('Nbre gr.'),
  );
  // Number of records (or nodes, in our case) to show per page.
  $count = 10;

  $query = db_select('gbb_gdiof', 'd');
  $query ->leftjoin('gbb_gdiof_dafor', 'dd',
                    'd.no_offre = dd.no_offre');
  $query ->leftjoin('gbb_gmoof', 'm',
                    'd.co_offre = m.co_offre');
  $query ->condition('d.no_offre', '20130000', '>')
         ->fields('d', array('no_offre'))
         ->fields('m', array('libl'));
  $query ->fields('dd', array('disc','nb_hp','nb_vac','nb_gr'
    ));
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

    $rows[] = array(
                array('data' => $r->no_offre),
                array('data' => $r->libl),
                array('data' => $d),
                array('data' => $hp),
                array('data' => $vac),
                array('data' => $gr),
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
    '#type' => 'textfield', '#size' => 8,
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
    '#type' => 'textfield', '#size' => 8,
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
    '#type' => 'textfield', '#size' => 8,
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



