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
    'date' => t('Date'),
  );
  $rows = array();
    $rows[] = array(
              array('data' => "toto"),
              );
  return theme('table', array('rows' => $rows));
}

