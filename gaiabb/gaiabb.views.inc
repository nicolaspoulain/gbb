<?php

/**
* Implement hook_views_data().
*
* Describe table "gbb_session" so that it can be used by views
*/

function gaiabb_views_data() {
  $data = array(
    'gbb_session' => array(
      'table' => array(
        'group' => t('Sessions'),
        'base' => array(
          'field' => 'sess_id', 
          'title' => t('Table des sessions'), 
          'help' => t("Table des sessions de formation associées à un module."),
        ),
      ),
      'co_modu' => array(
        'title' => t('co_modu'),
        'help' => t('Code du module pour la session. (gbb_session.co_modu)'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'sort' => array('handler' => 'views_handler_sort'),
        'filter' => array('handler' => 'views_handler_filter_string'),
        'argument' => array('handler' => 'views_handler_argument_string'),
      ),
      'date_timestamp' => array(
        'title' => t('date'), 
        'help' => t('Date de la session sur le module (gbb_session.date_timestamp)'), 
        'field' => array(
          'handler' => 'views_handler_field_date', //'handler' => 'views_handler_field', 
          'click sortable' => TRUE,
        ), 
        'sort' => array('handler' => 'views_handler_sort_date'), 
        'filter' => array('handler' => 'date_views_filter_handler_simple'), 
      ),
    ),

    'gbb_gresp_dafor' => array(
      'table' => array(
        'group' => t('Resp.péda.'),
        'join' => array(
          'gbb_session' => array(
            'left_field' => 'co_resp',
            'field' => 'co_resp',
          ),
        ),
      ),
      'nomu' => array(
        'title' => t('nomu'),
        'help' => t('Non du responsable pédagogique (gbb_resp_dafor.nomu)'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'sort' => array('handler' => 'views_handler_sort'),
        'filter' => array('handler' => 'views_handler_filter_string'),
        'argument' => array('handler' => 'views_handler_argument_string'),
      ),
      'prenom' => array(
        'title' => t('prenom'),
        'help' => t('Prénom du responsable pédagogique (gbb_resp_dafor.prenom)'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'sort' => array('handler' => 'views_handler_sort'),
        'filter' => array('handler' => 'views_handler_filter_string'),
        'argument' => array('handler' => 'views_handler_argument_string'),
      ),
    ),

    'gbb_gmodu' => array(
      'table' => array(
        'group' => t('Module'),
        'join' => array(
          'gbb_session' => array(
            'left_field' => 'co_modu',
            'field' => 'co_modu',
          ),
        ),
      ),
      'lib' => array(
        'title' => t('lib'),
        'help' => t('Libellé du module (gbb_gmodu.lib)'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'sort' => array('handler' => 'views_handler_sort'),
        'filter' => array('handler' => 'views_handler_filter_string'),
        'argument' => array('handler' => 'views_handler_argument_string'),
      ),
    ),

    'gbb_gdisp' => array(
      'table' => array(
        'group' => t('Dispositif'),
        'join' => array(
          'gbb_gmodu' => array(
            'left_field' => 'co_disp',
            'field' => 'co_disp',
          ),
        ),
      ),
      'id_disp' => array(
        'title' => t('id_disp'),
        'help' => t('Numero du dispositif (gbb_gdisp.id_disp)'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'sort' => array('handler' => 'views_handler_sort'),
        'filter' => array('handler' => 'views_handler_filter_string'),
        'argument' => array('handler' => 'views_handler_argument_string'),
      ),
    ),
  );
 
  $data['gbb_gdisp']['table']['join']['gbb_session'] = array(
    'left_table' => 'gbb_gmodu',
    'left_field' => 'co_disp',
    'field' => 'co_disp',
  );
  $data['gbb_gdisp']['id_disp'] = array(
    'title' => t('id_disp'),
    'help' => t('Numero du dispositif (gbb_gdisp.id_disp)'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array('handler' => 'views_handler_sort'),
    'filter' => array('handler' => 'views_handler_filter_string'),
    'argument' => array('handler' => 'views_handler_argument_string'),
  );

  $data['gbb_gresp']['table']['join']['gbb_gmodu'] = array(
    'left_table' => 'gbb_gdire',
    'left_field' => 'co_resp',
    'field' => 'co_resp',
  );
  $data['gbb_gresp']['nomu'] = array(
    'title' => t('nomu'),
    'help' => t('Nom du conseiller (gbb_gresp.nomu)'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array('handler' => 'views_handler_sort'),
    'filter' => array('handler' => 'views_handler_filter_string'),
    'argument' => array('handler' => 'views_handler_argument_string'),
  );

  return $data;
}

