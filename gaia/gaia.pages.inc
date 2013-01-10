<?php

/**
 * Task view callback.
 */
function gmodu_view($gmodu) {
  drupal_set_title(t('Module n°') . entity_id('gmodu', $gmodu));
  $header = array('Champ','Valeur');
  $wrapper = entity_metadata_wrapper('gmodu', $gmodu);
  //drupal_set_message(print_r($wrapper->getPropertyInfo()));
  foreach ($wrapper->getPropertyInfo() as $field) {
    $f = strtolower($field['label']);
    $f = ($f == 'gmodu entity id')? 'co_modu' : $f;
    $v = $wrapper->$f->value(array('sanitize' => TRUE));
    $rows[] = array(
                array('data' => $field['label']),
                array('data' => $v),
              );
  };
  return theme('table',array('rows' => $rows,'header' => $header));
  //return entity_view('gmodu', array(entity_id('gmodu', $gmodu) => $gmodu), 'full');
}

/**
 * Form callback: allows us to edit a gmodu entity.
 */
function gmodu_edit_form($form, &$form_state, $gmodu) {
  $form_state['gmodu'] = $gmodu;
  //$form['lib'] = array(
  //  '#type' => 'textfield',
  //  '#title' => t('Lib'),
  //  '#required' => TRUE,
  //  '#default_value' => $gmodu->lib,
  //);
  field_attach_form('gmodu', $gmodu, $form, $form_state);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Modif'),
    '#weight' => 100,
  );

  return $form;
}
/**
 * Form callback: Validates gmodu_add_form form.
 */
function gmodu_edit_form_validate($form, &$form_state) {
  $gmodu_submission = (object) $form_state['values'];
  field_attach_form_validate('gmodu', $gmodu_submission, $form, $form_state);
}

/**
 * Form callback: submits gmodu_edit_form information
 */
function gmodu_edit_form_submit($form, &$form_state) {
  $gmodu_submission = (object) $form_state['values'];
  field_attach_submit('gmodu', $gmodu_submission, $form, $form_state);
  $gmodu = gmodu_save($gmodu_submission);
  $form_state['redirect'] = $gmodu['path'];
  drupal_set_message(t('Modification enregistrée'));
}

/**
 * We save the gmodu entity by calling the controller.
 */
function gmodu_save(&$gmodu) {
  entity_get_controller('gmodu')->save($gmodu);
  return entity_uri('gmodu',$gmodu);
}

function gdisp_view($gdisp) {
  drupal_set_title(t('Dispositif ') . entity_id('gdisp', $gdisp));
  $header = array('Champ','Valeur');
  $wrapper = entity_metadata_wrapper('gdisp', $gdisp);
  //drupal_set_message(print_r($wrapper->getPropertyInfo()));
  foreach ($wrapper->getPropertyInfo() as $field) {
    $f = strtolower($field['label']);
    $f = ($f == 'gdisp entity id')? 'id_disp' : $f;
    $v = $wrapper->$f->value(array('sanitize' => TRUE));
    $rows[] = array(
                array('data' => $field['label']),
                array('data' => $v),
              );
  };
  return theme('table',array('rows' => $rows,'header' => $header));
}

function session_view($session) {
  drupal_set_title(t('Session ') . entity_id('session', $session));
  $header = array('Champ','Valeur');
  $wrapper = entity_metadata_wrapper('session', $session);
  //drupal_set_message(print_r($wrapper->getPropertyInfo()));
  foreach ($wrapper->getPropertyInfo() as $field) {
    $f = strtolower($field['label']);
    $f = ($f == 'session entity id')? 'sess_id' : $f;
    $v = $wrapper->$f->value(array('sanitize' => TRUE));
    $rows[] = array(
                array('data' => $field['label']),
                array('data' => $v),
              );
  };
  return theme('table',array('rows' => $rows,'header' => $header));
}

function netab_view($netab) {
  drupal_set_title(t('Etablissement ') . entity_id('netab', $netab));
  $header = array('Champ','Valeur');
  $wrapper = entity_metadata_wrapper('netab', $netab);
  //drupal_set_message(print_r($wrapper->getPropertyInfo()));
  foreach ($wrapper->getPropertyInfo() as $field) {
    $f = strtolower($field['label']);
    $f = ($f == 'netab entity id')? 'co_lieu' : $f;
    $v = $wrapper->$f->value(array('sanitize' => TRUE));
    $rows[] = array(
                array('data' => $field['label']),
                array('data' => $v),
              );
  };
  return theme('table',array('rows' => $rows,'header' => $header));
}

function gresp_view($gresp) {
  drupal_set_title(t('Responsable ') . entity_id('gresp', $gresp));
  $header = array('Champ','Valeur');
  $wrapper = entity_metadata_wrapper('gresp', $gresp);
  //drupal_set_message(print_r($wrapper->getPropertyInfo()));
  foreach ($wrapper->getPropertyInfo() as $field) {
    $f = strtolower($field['label']);
    $f = ($f == 'gresp entity id')? 'co_resp' : $f;
    $v = $wrapper->$f->value(array('sanitize' => TRUE));
    $rows[] = array(
                array('data' => $field['label']),
                array('data' => $v),
              );
  };
  return theme('table',array('rows' => $rows,'header' => $header));
}

function ncont_view($ncont) {
  drupal_set_title(t('Ncont ') . entity_id('ncont', $ncont));
  $header = array('Champ','Valeur');
  $wrapper = entity_metadata_wrapper('ncont', $ncont);
  //drupal_set_message(print_r($wrapper->getPropertyInfo()));
  foreach ($wrapper->getPropertyInfo() as $field) {
    $f = strtolower($field['label']);
    $f = ($f == 'ncont entity id')? 'co_cont' : $f;
    $v = $wrapper->$f->value(array('sanitize' => TRUE));
    $rows[] = array(
                array('data' => $field['label']),
                array('data' => $v),
              );
  };
  return theme('table',array('rows' => $rows,'header' => $header));
}
