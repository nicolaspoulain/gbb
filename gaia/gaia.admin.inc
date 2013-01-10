<?php

/**
 * Just provide some basic info for the entity administration page.
 * This can be expanded to add more information
 */
function gmodu_admin_page() {
  $output = 'Page d\'administration de l\'entite gmodu.<br/>';
  $output .= 'gerer ici les champs additionnels';
  return $output;
}
function session_admin_page() {
  $output = 'Page d\'administration de l\'entite session.<br/>';
  $output .= 'gerer ici les champs additionnels';
  return $output;
}

