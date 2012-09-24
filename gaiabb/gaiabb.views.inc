<?php

/**
* Implement hook_views_data().
*
* Describe table "gbb_session" so that it can be used by views
*/

function gaiabb_views_data() {
  $tableName = 'gbb_session';
  $data = array();
  $data[$tableName]['table']['group'] = t('gaiabbSessions');

  $data[$tableName]['table']['base'] = array(
          'field' => 'co_resp', 
          'title' => t('gaiabbSessions'), 
          'help' => t("Sessions de Gaiabb.")
          );
  $data[$tableName]['co_resp'] = array(
          'title' => t('co_resp'), 
          'help' => t('Numero de rep péda'), 
          'field' => array(
              'handler' => 'views_handler_field', 
              'click sortable' => TRUE,
              ), 
          'sort'     => array('handler' => 'views_handler_sort'), 
          'filter'   => array('handler' => 'views_handler_filter_string'), 
          'argument' => array('handler' => 'views_handler_argument_string'),
          ); 
  $data[$tableName]['co_modu'] = array(
          'title' => t('co_modu'), 
          'help' => t('Numero de module : co_modu'), 
          'field' => array(
              'handler' => 'views_handler_field', 
              'click sortable' => TRUE,
              ), 
          'sort'     => array('handler' => 'views_handler_sort'), 
          'filter'   => array('handler' => 'views_handler_filter_string'), 
          'argument' => array('handler' => 'views_handler_argument_string'),
          );
  $data[$tableName]['date'] = array(
          'title' => t('date'), 
          'help' => t('date de création'), 
          'field' => array(
              'handler' => 'views_handler_field_date', //'handler' => 'views_handler_field', 
              'click sortable' => TRUE,
              ), 
          'sort' => array('handler' => 'views_handler_sort_date'), 
          'filter' => array('handler' => 'date_views_filter_handler_simple'), 
          );


  $data['gbb_gresp_dafor']['table']['default_relationship'] = array(
          $tableName => array(
              'left_field' => 'co_resp',
              'field' => 'co_resp',
              ),
          );
  $data['gbb_gresp_dafor']['nomu'] = array(
          'title' => t('nomu'), 
          'help' => t('Nom'), 
          'field' => array(
              'handler' => 'views_handler_field', 
              'click sortable' => TRUE,
              ), 
          'sort'     => array('handler' => 'views_handler_sort'), 
          'filter'   => array('handler' => 'views_handler_filter_string'), 
          'argument' => array('handler' => 'views_handler_argument_string'),
          );

 return $data;
}


function gaiabb_views_data() {
  $tableName = 'gbb_session';
  $data = array();
  $data[$tableName]['table']['group'] = t('gaiabbSessions');

  $data[$tableName]['table']['base'] = array(
          'field' => 'sess_id', 
          'title' => t('gaiabbSessions'), 
          'help' => t("Sessions de Gaiabb.")
          );

  $data[$tableName]['co_resp'] = array(
          'title' => t('co_resp'), 
          'help' => t('Numero de rep péda'), 
          'field' => array(
              'handler' => 'views_handler_field', 
              'click sortable' => TRUE,
              ), 
          'sort'     => array('handler' => 'views_handler_sort'), 
          'filter'   => array('handler' => 'views_handler_filter_string'), 
          'argument' => array('handler' => 'views_handler_argument_string'),
          );
 
  $data[$tableName]['co_modu'] = array(
          'title' => t('co_modu'), 
          'help' => t('Numero de module : co_modu'), 
          'field' => array(
              'handler' => 'views_handler_field', 
              'click sortable' => TRUE,
              ), 
          'sort'     => array('handler' => 'views_handler_sort'), 
          'filter'   => array('handler' => 'views_handler_filter_string'), 
          'argument' => array('handler' => 'views_handler_argument_string'),
          );
  $data[$tableName]['date'] = array(
          'title' => t('date'), 
          'help' => t('date de création'), 
          'field' => array(
              'handler' => 'views_handler_field_date', //'handler' => 'views_handler_field', 
              'click sortable' => TRUE,
              ), 
          'sort' => array('handler' => 'views_handler_sort_date'), 
          'filter' => array('handler' => 'date_views_filter_handler_simple'), 
          );

  $data[$tableName]['sess_id'] = array(
          'title' => t('Unique identifier'),
          'help' => t('Primary key for table'),
          'field' => array('handler' => 'views_handler_field'),
          'argument' => array('handler' => 'views_handler_argument_numeric'),
          'filter' => array('handler' => 'views_handler_filter_numeric'),
          'sort' => array('handler' => 'views_handler_sort_numeric'));

  $data[$tableName]['table']['join'] = array(
          'gbb_gresp_dafor' => array(
              'left_field' => 'co_resp',
              'field' => 'co_resp',
              ),
          );

  return $data;
}


