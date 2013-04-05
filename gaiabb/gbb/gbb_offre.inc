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

CREATE TABLE `gbbdr_exp`.`gbb_gmoof_taxonomy` (
`co_omodu` INT( 11 ) NOT NULL ,
`tid` INT( 10 ) NOT NULL
) ENGINE = MYISAM ;

INSERT INTO gbb_gmoof_taxonomy (co_omodu,tid)
SELECT co_omodu,tid FROM gbb_gdiof_dafor
JOIN taxonomy_term_data
ON taxonomy_term_data.name=gbb_gdiof_dafor.disc

 */
function gbb_offre_view($args = 'NaN') {
  // Charge la feuille des style et les fichiers javascript pour le module
  drupal_add_css(drupal_get_path('module', 'gbb') . '/css/gbb.css',
                 array('group' => CSS_SYSTEM -1 , 'preprocess' => FALSE));
  drupal_add_library('system', 'drupal.collapse');

  // Construction de la page
  $build = array();
  //$build['search'] = render(drupal_get_form('gbb_offres_filter',$args));
  $build['tab'] = gbb_offre_tab($args);

  return implode('', $build);
}

function gbb_offre_tab($args) {
  //print_r(taxonomy_get_vocabularies());
  foreach (taxonomy_get_vocabularies() as $k=>$v) {
    $header[] = array('data' => t($v->name.' ').l('('.t('edit').')',
                            'admin/structure/taxonomy/'.$v->machine_name), 'class' => 'gris');
    $rows[0][] = array('data' => render(drupal_get_form('gbb_offre_taxonomy_form', arg(1), $k)),
                       'class'=>'topAlign');
  }
  $output = theme('table',array('header' => $header, 'rows'   => $rows ));
  return $output;
}

function gbb_offre_taxonomy_form($form, &$form_state, $co_omodu,$vid) {
  $form['f']['co_omodu']  = array('#type' => 'hidden','#value' => $co_omodu );

  $terms = taxonomy_get_tree($vid);
  $terms_select = array();
  foreach ($terms as $term) {
    $terms_select[$term->tid] = str_repeat('— ', $term->depth) . $term->name;
  };
  $checked = db_query("SELECT tid FROM {gbb_gmoof_taxonomy}
                                  WHERE co_omodu = :co_omodu",
                              array(':co_omodu' => $co_omodu,))->fetchCol();
  $form['f']['disc'] = array(
    '#type' => 'checkboxes',
    '#options' => $terms_select,
    '#default_value' => $checked,
    '#ajax' => array(
      'callback' => 'gbb_offre_taxonomy_form_submit',
      'progress' => array('type' => 'throbber', 'message' => '')),);
  return $form;
}
function gbb_offre_taxonomy_form_submit($form, &$form_state) {
  //dpm($form_state['values']);
  foreach ($form_state['values']['disc'] as $key => $value) {
    if ($value == 0) { // $form_state['values']['disc'][$key] not checked
      $num_deleted = db_delete('gbb_gmoof_taxonomy')
          ->condition('co_omodu', $form_state['values']['co_omodu'] )
          ->condition('tid', $key )
          ->execute();
    } else {           // $form_state['values']['disc'][$key] was checked
      $nb = db_select('gbb_gmoof_taxonomy','m')
          ->fields('m',array('tid'))
          ->condition('co_omodu', $form_state['values']['co_omodu'] )
          ->condition('tid', $key )
          ->execute()->rowCount();
      if ($nb==0)  db_insert('gbb_gmoof_taxonomy')
                     ->fields(array('co_omodu' => $form_state['values']['co_omodu'] ,
                                        'tid' => $key ))
                     ->execute();
    }
  }
}


