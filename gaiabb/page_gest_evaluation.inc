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
function evaluation($args = 'NaN') {
  // Charge la feuille des style et les fichiers javascript pour le module
  drupal_add_css(drupal_get_path('module', 'gaiabb') . '/gaiabb.css', array('group' => CSS_SYSTEM -1 , 'preprocess' => FALSE));
  drupal_add_css(drupal_get_path('module', 'gaiabb') . '/gaiabb.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
  // Construction de la page
  $build = array();
  //$build['infos'] = render(drupal_get_form('eval_infos' , $args));
  $build['environnement'] = render(drupal_get_form('eval_AA_form' , $args));

  echo "wizzz";
  return implode('', $build);
}

/**
 * Formulaire sur l'** ENVIONNEMENT ** du stage
 */
function eval_AA_form($form, &$form_state) {

  $form['f'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fiche formateur'),
  );
  return $form;
}


/**
 * Formulaire sur l'** ENVIONNEMENT ** du stage
 */
function eval_environnement_form($form, &$form_state) {

  if (arg(1)=='') return '';

  $query = db_select('gbb_gresp_dafor', 'r');
  $query   ->leftjoin('gbb_gresp_plus', 'rp', 'r.co_resp=rp.co_resp');
  $query   ->condition('r.co_resp', arg(1), '=')
           ->fields('r', array('co_resp', 'nomu', 'prenom', 'mel', 'tel'))
           ->fields('rp', array('discipline','resp_dafor', 'decharge', 'grade', 'divers'));
  $record = $query->execute()->fetchObject();

  if (arg(1)=='add'){
    $record->co_resp = db_query("SELECT MAX(co_resp)+1 as max_id FROM gbb_gresp_dafor")->fetchField();
    $record->nomu = '';
    $record->prenom = '';
    $record->mel = '';
    $record->tel = '';
    $record->discipline = '';
    $record->resp_dafor = '';
    $record->decharge = '';
    $record->grade = '';
    $record->divers = '';
  }
  $record->co_degre = '2';

  $form['f'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fiche formateur'),
  );
  
$form['f']['coresp'] = array(
    '#type' => 'hidden',
    '#value' => $record->co_resp,
  );
$form['f']['codegre'] = array(
    '#type' => 'hidden',
    '#value' => $record->co_degre,
  );
$form['f']['nomu'] = array(
    '#type' => 'textfield',
    '#title' => t('Nom'),
    '#default_value' => $record->nomu,
    '#size' => 40,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>',
  );
$form['f']['prenom'] = array(
    '#type' => 'textfield',
    '#title' => t('Prénom'),
    '#default_value' => $record->prenom,
    '#size' => 40,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>',
  );
$form['f']['mel'] = array(
    '#type' => 'textfield',
    '#title' => t('email'),
    '#default_value' => $record->mel,
    '#size' => 40,
    '#maxlength' => 128,
  );
$form['f']['tel'] = array(
    '#type' => 'textfield',
    '#title' => t('Téléphone'),
    '#default_value' => $record->tel,
    '#size' => 40,
    '#maxlength' => 128,
  );
$form['f']['discipline'] = array(
    '#type' => 'textfield',
    '#title' => t('Discipline'),
    '#default_value' => $record->discipline,
    '#size' => 40,
    '#maxlength' => 128,
  );
$form['f']['grade'] = array(
    '#type' => 'select',
    '#title' => t('Grade'),
    '#default_value' => $record->grade,
    '#options' => drupal_map_assoc(array(
'','NOMENCLATURE','VACATAIRE','CONTRACTUEL','MA','PEGC','PLP','CERTIFIE','AGREGE','INST','PE','COP','CPE','INF','MED','ING','ASU','CASU','AS','PERDIR','IA-IPR','IEN-EG','IEN-ET','IEN-CC','CC','PREC','PRCE','PRAG','MC','PR','ASSOCIATION','ENTREPRISE','UNIVERSITE','INTERVENANT-XT'
    )),
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>&nbsp;',
  );
$form['f']['decharge'] = array(
    '#type' => 'textfield',
    '#title' => t('Décharge (h)'),
    '#default_value' => $record->decharge,
    '#size' => 10,
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>&nbsp;',
  );
$form['f']['resp_dafor'] = array(
    '#type' => 'textfield',
    '#title' => t('Responsable DAFOR'),
    '#default_value' => $record->resp_dafor,
    '#size' => 10,
    '#maxlength' => 128,
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>',
  );
$form['f']['divers'] = array(
    '#type' => 'textarea',
    '#title' => t('Divers'),
    '#default_value' => $record->divers,
  );


  $form['f']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Enregistrer',
  );

  return $form;
}

/**
 * ----> Submit
 */
function form_modif_formateur_submit($form, &$form_state) {

// Merge Queries : http://drupal.org/node/310085
  db_merge('gbb_gresp_dafor')
    ->key(array('co_resp'  => $form_state['values']['coresp'],
             ))
    ->fields(array('nomu'   => $form_state['values']['nomu'],
                   'prenom' => $form_state['values']['prenom'],
                   'mel'    => $form_state['values']['mel'],
                   'tel'    => $form_state['values']['tel'],
                   'co_degre'    => $form_state['values']['codegre'],
             ))
    ->execute();
  if ($form_state['values']['decharge']=='') $form_state['values']['decharge']='0.0';
  db_merge('gbb_gresp_plus')
    ->key(array('co_resp'  => $form_state['values']['coresp'],
             ))
    ->fields(array('discipline' => $form_state['values']['discipline'],
                   'resp_dafor' => $form_state['values']['resp_dafor'],
                   'decharge'   => $form_state['values']['decharge'],
                   'grade'      => $form_state['values']['grade'],
                   'divers'     => $form_state['values']['divers'],
             ))
    ->execute();

  $url = url('formateur/' . $form_state['values']['coresp'],
                            array('absolute' => TRUE));
  $form_state['redirect'] = $url;
}

