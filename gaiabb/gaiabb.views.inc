<?php

/**
* Implement hook_views_data().
*
* Describe table "gbb_session" so that it can be used by views
*
* Pour les handlers (filed, sort, filter..)
* http://api.drupal.org/api/views/views.api.php/group/views_handlers/7
*/

function gaiabb_views_data() {

  // --------------------------------------------------------
  // gbb_gmodu
  // --------------------------------------------------------

  $data['gbb_gmodu']['table'] = array(
            'group' => t('gbb_gmodu'),
              'base' => array(
                'field' => 'co_modu', 
                'title' => t('GaiaBB'), 
                'help' => t('Tables du module GaiaBB.'),
        ),
  );
  $data['gbb_gmodu']['co_modu'] = array(
            'title' => t('co_modu'),
            'help' => t('Code du module'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
            ),
            'sort' => array('handler' => 'views_handler_sort'),
            'filter' => array('handler' => 'views_handler_filter_numeric'),
            'argument' => array('handler' => 'views_handler_argument_numeric'),
  );
  $data['gbb_gmodu']['lib'] = array(
            'title' => t('lib'),
            'help' => t('Libellé du module'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
            ),
            'sort' => array('handler' => 'views_handler_sort'),
            'filter' => array('handler' => 'views_handler_filter_string'),
            'argument' => array('handler' => 'views_handler_argument_string'),
  );

  // --------------------------------------------------------
  // gbb_session
  // --------------------------------------------------------

  $data['gbb_session']['table'] = array(
            'group' => t('gbb_session'),
            'join' => array(
              'gbb_gmodu' => array(
                'left_field' => 'co_modu',
                'field' => 'co_modu',
                'extra' => 'gbb_session.co_degre = gbb_gmodu.co_degre',
              ),
            ),
  );
  $data['gbb_session']['co_resp'] = array(
            'title' => t('co_resp'),
            'help' => t('Code du responsable'),
            'field' => array(
              'handler' => 'views_handler_field_numeric',
              'click sortable' => TRUE,
             ),
            'sort' => array('handler' => 'views_handler_sort'),
            'filter' => array('handler' => 'views_handler_filter_numeric'),
            'argument' => array('handler' => 'views_handler_argument_numeric'),
  );
  $data['gbb_session']['date_timestamp'] = array(
            'title' => t('date_timestamp'),
            'help' => t('Date (TS)'),
            'field' => array(
              'handler' => 'views_handler_field_date',
              'click sortable' => TRUE,
             ),
            'sort' => array('handler' => 'views_handler_sort_date'),
            'filter' => array('handler' => 'date_views_filter_handler_simple'), 
  );
  $data['gbb_session']['horaires'] = array(
            'title' => t('horaires'),
            'help' => t('Horaires'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
             ),
            'sort' => array('handler' => 'views_handler_sort'),
            'filter' => array('handler' => 'views_handler_filter_string'),
            'argument' => array('handler' => 'views_handler_argument_string'),
  );
   $data['gbb_session']['typePaiement'] = array(
            'title' => t('typePaiement'),
            'help' => t('Type de paiement'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
             ),
            'sort' => array('handler' => 'views_handler_sort'),
            'filter' => array('handler' => 'views_handler_filter_string'),
            'argument' => array('handler' => 'views_handler_argument_string'),
  );
   $data['gbb_session']['groupe'] = array(
            'title' => t('groupe'),
            'help' => t('Groupe'),
            'field' => array(
              'handler' => 'views_handler_field_numeric',
              'click sortable' => TRUE,
             ),
            'sort' => array('handler' => 'views_handler_sort'),
            'filter' => array('handler' => 'views_handler_filter_numeric'),
            'argument' => array('handler' => 'views_handler_argument_numeric'),
  );
    $data['gbb_session']['dureeapayer'] = array(
            'title' => t('dureeapayer'),
            'help' => t('Duree à payer'),
            'field' => array(
              'handler' => 'views_handler_field_numeric',
              'click sortable' => TRUE,
             ),
            'sort' => array('handler' => 'views_handler_sort'),
            'filter' => array('handler' => 'views_handler_filter_numeric'),
            'argument' => array('handler' => 'views_handler_argument_numeric'),
  );
    $data['gbb_session']['dureeprevue'] = array(
            'title' => t('dureeprevue'),
            'help' => t('Durée prévue'),
            'field' => array(
              'handler' => 'views_handler_field_numeric',
              'click sortable' => TRUE,
             ),
            'sort' => array('handler' => 'views_handler_sort'),
            'filter' => array('handler' => 'views_handler_filter_numeric'),
            'argument' => array('handler' => 'views_handler_argument_numeric'),
  );
  $data['gbb_session']['convoc_sent'] = array(
            'title' => t('convoc_sent'),
            'help' => t('La convocation a été envoyée (0/1)'),
            'field' => array(
              'handler' => 'views_handler_field_numeric',
              'click sortable' => TRUE,
             ),
            'sort' => array('handler' => 'views_handler_sort'),
            'filter' => array('handler' => 'views_handler_filter_numeric'),
            'argument' => array('handler' => 'views_handler_argument_numeric'),
  );
  $data['gbb_session']['en_attente'] = array(
            'title' => t('en_attente'),
            'help' => t('La session est en attente (0/1)'),
            'field' => array(
              'handler' => 'views_handler_field_numeric',
              'click sortable' => TRUE,
             ),
            'sort' => array('handler' => 'views_handler_sort'),
            'filter' => array('handler' => 'views_handler_filter_numeric'),
            'argument' => array('handler' => 'views_handler_argument_numeric'),
  );
  

  // --------------------------------------------------------
  // gbb_gmodu_plus
  // --------------------------------------------------------

  $data['gbb_gmodu_plus']['table'] = array(
            'group' => t('gbb_gmodu_plus'),
            'join' => array(
              'gbb_gmodu' => array(
                'left_field' => 'co_modu',
                'field' => 'co_modu',
                'extra' => 'gbb_gmodu_plus.co_degre = gbb_gmodu.co_degre',
              ),
            ),
  );
  $data['gbb_gmodu_plus']['organisation'] = array(
            'title' => t('organisation'),
            'help' => t('Organisation du module'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
             ),
            'sort' => array('handler' => 'views_handler_sort'),
            'filter' => array('handler' => 'views_handler_filter_string'),
            'argument' => array('handler' => 'views_handler_argument_string'),
  );
   // --------------------------------------------------------
  // gbb_gdire table de liaison, pas de champ à extraire
  // --------------------------------------------------------

  $data['gbb_gdire']['table'] = array(
            'group' => t('gdire'),
            'join' => array(
              'gbb_gmodu' => array(
                'left_field' => 'co_modu',
                'field' => 'co_modu',
                'extra' => 'gbb_gdire.co_degre = gbb_gmodu.co_degre',
              ),
            ),
  );

  // --------------------------------------------------------
  // gbb_gresp
  // --------------------------------------------------------

  $data['gbb_gresp']['table'] = array(
            'group' => t('gbb_gresp'),
            'join' => array(
              'gbb_gmodu' => array(
                'left_table' => 'gbb_gdire',
                'left_field' => 'co_resp',
                'field' => 'co_resp',
                'extra' => 'gbb_gresp.co_degre = gbb_gmodu.co_degre',
              ),
            ),
  );

  $data['gbb_gresp']['nomu'] = array(
            'title' => t('nomu'),
            'help' => t('Nom du responsable'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_string'),
           'argument' => array('handler' => 'views_handler_argument_string'),
  );

  // --------------------------------------------------------
  // gbb_gresp_dafor
  // --------------------------------------------------------

  $data['gbb_gresp_dafor']['table'] = array(
            'group' => t('gbb_gresp_dafor'),
            'join' => array(
              'gbb_gmodu' => array(
                'left_table' => 'gbb_session',
                'left_field' => 'co_resp',
                'field' => 'co_resp',
                'extra' => 'gbb_gresp_dafor.co_degre = gbb_gmodu.co_degre',
              ),
            ),
  );
  $data['gbb_gresp_dafor']['nomu'] = array(
            'title' => t('nomu'),
            'help' => t('Nom du responsable'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_string'),
           'argument' => array('handler' => 'views_handler_argument_string'),
  );
  $data['gbb_gresp_dafor']['prenom'] = array(
            'title' => t('prenom'),
            'help' => t('Prénom du responsable'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_string'),
           'argument' => array('handler' => 'views_handler_argument_string'),
  );

  // --------------------------------------------------------
  // gbb_gdisp
  // --------------------------------------------------------

  $data['gbb_gdisp']['table'] = array(
           'group' => t('gbb_gdisp'),
           'join' => array(
             'gbb_gmodu' => array(
               'left_table' => 'gbb_gdire',
               'left_field' => 'co_disp',
               'field' => 'co_disp',
                'extra' => 'gbb_gdisp.co_degre = gbb_gmodu.co_degre',
             ),
           ),
  );
  $data['gbb_gdisp']['id_disp'] = array(
           'title' => t('id_disp'),
           'help' => t('Identifiant du dispositif'),
           'field' => array(
             'handler' => 'views_handler_field',
             'click sortable' => TRUE,
           ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_string'),
           'argument' => array('handler' => 'views_handler_argument_string'),
  );
  $data['gbb_gdisp']['lib'] = array(
           'title' => t('lib'),
           'help' => t('Libellé du dispositif'),
           'field' => array(
             'handler' => 'views_handler_field',
             'click sortable' => TRUE,
           ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_string'),
           'argument' => array('handler' => 'views_handler_argument_string'),
  );

  // --------------------------------------------------------
  // gbb_netab
  // --------------------------------------------------------

  $data['gbb_netab']['table'] = array(
            'group' => t('gbb_netab'),
            'join' => array(
              'gbb_gmodu' => array(
                'left_table' => 'gbb_session',
                'left_field' => 'co_lieu',
                'field' => 'co_lieu',
              ),
            ),
  );
  $data['gbb_netab']['denom_comp'] = array(
            'title' => t('denom_comp'),
            'help' => t('Dénomination complémentaire'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_string'),
           'argument' => array('handler' => 'views_handler_argument_string'),
  );
   $data['gbb_netab']['sigle'] = array(
            'title' => t('sigle'),
            'help' => t('Sigle (CLG, LP, LGT,...)'),
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
