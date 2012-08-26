<?php
/**
 * @file
 *
 * Composition de la page de gestion des formateurs
 *
 */

/**
 * Fonction d'appel aux differents elements composant la page
 * affichant une liste de responsables et les stages qui leur
 * associes
 */
function formateur($args = 'NaN') {
  // Construction de la page
  $build = array();
  #$build['form_people'] = render(drupal_get_form('form_people', $args));

  return implode('', $build);
}

