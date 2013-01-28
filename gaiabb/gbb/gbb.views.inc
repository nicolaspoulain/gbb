<?php

/**
* Implement hook_views_data().
*
* Describe table "gbb_session" so that it can be used by views
*
* Pour les handlers (filed, sort, filter..)
* http://api.drupal.org/api/views/views.api.php/group/views_handlers/7
*/

function gbb_views_data() {

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
  $data['gbb_gmodu']['co_degre'] = array(
            'title' => t('co_degre'),
            'help' => t('Code degré du module'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
            ),
            'sort' => array('handler' => 'views_handler_sort'),
            'filter' => array('handler' => 'views_handler_filter_numeric'),
            'argument' => array('handler' => 'views_handler_argument_numeric'),
  );
   $data['gbb_gmodu']['co_anmo'] = array(
            'title' => t('co_anmo'),
            'help' => t('Code du type d annulation des modules'),
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
  $data['gbb_gmodu']['cout_p_fonc'] = array(
            'title' => t('cout_p_fonc'),
            'help' => t('Coût prévisionnel de fonctionnement par groupe'),
            'field' => array(
              'handler' => 'views_handler_field_numeric',
              'click sortable' => TRUE,
             ),
            'sort' => array('handler' => 'views_handler_sort'),
            'filter' => array('handler' => 'views_handler_filter_numeric'),
            'argument' => array('handler' => 'views_handler_argument_numeric'),
  );
  $data['gbb_gmodu']['cout_p_prest'] = array(
            'title' => t('cout_p_prest'),
            'help' => t('Coût prévisionnel de prestation par groupe'),
            'field' => array(
              'handler' => 'views_handler_field_numeric',
              'click sortable' => TRUE,
             ),
            'sort' => array('handler' => 'views_handler_sort'),
            'filter' => array('handler' => 'views_handler_filter_numeric'),
            'argument' => array('handler' => 'views_handler_argument_numeric'),
  );
  $data['gbb_gmodu']['cout_p_excep'] = array(
            'title' => t('cout_p_excep'),
            'help' => t('Coût prévisionnel des frais exceptionnels'),
            'field' => array(
              'handler' => 'views_handler_field_numeric',
              'click sortable' => TRUE,
             ),
            'sort' => array('handler' => 'views_handler_sort'),
            'filter' => array('handler' => 'views_handler_filter_numeric'),
            'argument' => array('handler' => 'views_handler_argument_numeric'),
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
  $data['gbb_session']['date_ts'] = array(
            'title' => t('date_ts'),
            'help' => t('Date TS'),
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
   $data['gbb_session']['type_paiement'] = array(
            'title' => t('type_paiement'),
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
    $data['gbb_session']['duree_a_payer'] = array(
            'title' => t('duree_a_payer'),
            'help' => t('Duree à payer'),
            'field' => array(
              'handler' => 'views_handler_field_numeric',
              'click sortable' => TRUE,
             ),
            'sort' => array('handler' => 'views_handler_sort'),
            'filter' => array('handler' => 'views_handler_filter_numeric'),
            'argument' => array('handler' => 'views_handler_argument_numeric'),
  );
  $data['gbb_session']['duree_prevue'] = array(
            'title' => t('duree_prevue'),
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
  $data['gbb_session']['co_lieu'] = array(
            'title' => t('co_lieu'),
            'help' => t('Code lieu (RNE)'),
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
  $data['gbb_gdire']['co_tres'] = array(
            'title' => t('co_tres'),
            'help' => t('Code trésorerie'),
            'field' => array(
              'handler' => 'views_handler_field_numeric',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_numeric'),
           'argument' => array('handler' => 'views_handler_argument_numeric'),
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
  $data['gbb_gresp_dafor']['co_resp'] = array(
            'title' => t('co_resp'),
            'help' => t('Code du formateur'),
            'field' => array(
              'handler' => 'views_handler_field_numeric',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_numeric'),
           'argument' => array('handler' => 'views_handler_argument_numeric'),
  );
  $data['gbb_gresp_dafor']['nomu'] = array(
            'title' => t('nomu'),
            'help' => t('Nom du formateur'),
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
            'help' => t('Prénom du formateur'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_string'),
           'argument' => array('handler' => 'views_handler_argument_string'),
  );
  $data['gbb_gresp_dafor']['tel'] = array(
            'title' => t('tel'),
            'help' => t('Télephone du formateur'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_string'),
           'argument' => array('handler' => 'views_handler_argument_string'),
  );
  $data['gbb_gresp_dafor']['mel'] = array(
            'title' => t('mel'),
            'help' => t('Mail du formateur'),
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
  $data['gbb_gdisp']['co_camp'] = array(
           'title' => t('co_camp'),
           'help' => t('Code de la campagne'),
           'field' => array(
             'handler' => 'views_handler_field',
             'click sortable' => TRUE,
           ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_string'),
           'argument' => array('handler' => 'views_handler_argument_string'),
  );

  // --------------------------------------------------------
  // gbb_netab_dafor
  // --------------------------------------------------------

  $data['gbb_netab_dafor']['table'] = array(
            'group' => t('gbb_netab_dafor'),
            'join' => array(
              'gbb_gmodu' => array(
                'left_table' => 'gbb_session',
                'left_field' => 'co_lieu',
                'field' => 'co_lieu',
              ),
            ),
  );
  $data['gbb_netab_dafor']['denom_comp'] = array(
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
   $data['gbb_netab_dafor']['sigle'] = array(
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

  // --------------------------------------------------------
  // gbb_evaluation
  // --------------------------------------------------------

  $data['gbb_evaluation']['table'] = array(
            'group' => t('gbb_evaluation'),
            'join' => array(
              'gbb_gmodu' => array(
                'left_field' => 'co_modu',
                'field' => 'co_modu',
                'extra' => 'gbb_evaluation.co_degre = gbb_gmodu.co_degre',
              ),
            ),
  );
  $data['gbb_evaluation']['groupe'] = array(
            'title' => t('groupe'),
            'help' => t('groupe'),
            'field' => array(
              'handler' => 'views_handler_field_numeric',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_numeric'),
           'argument' => array('handler' => 'views_handler_argument_numeric'),
  );
   $data['gbb_evaluation']['satisAccueil'] = array(
            'title' => t('satisAccueil'),
            'help' => t('satisAccueil'),
            'field' => array(
              'handler' => 'views_handler_field_numeric',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_numeric'),
           'argument' => array('handler' => 'views_handler_argument_numeric'),
  );
   $data['gbb_evaluation']['commAccueil'] = array(
            'title' => t('commAccueil'),
            'help' => t('commAccueil'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_string'),
           'argument' => array('handler' => 'views_handler_argument_string'),
  );
   $data['gbb_evaluation']['satisSalle'] = array(
            'title' => t('satisSalle'),
            'help' => t('satisSalle'),
            'field' => array(
              'handler' => 'views_handler_field_numeric',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_numeric'),
           'argument' => array('handler' => 'views_handler_argument_numeric'),
  );
   $data['gbb_evaluation']['commSalle'] = array(
            'title' => t('commSalle'),
            'help' => t('commSalle'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_string'),
           'argument' => array('handler' => 'views_handler_argument_string'),
  );
    $data['gbb_evaluation']['satisMatos'] = array(
            'title' => t('satisMatos'),
            'help' => t('satisMatos'),
            'field' => array(
              'handler' => 'views_handler_field_numeric',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_numeric'),
           'argument' => array('handler' => 'views_handler_argument_numeric'),
  );
   $data['gbb_evaluation']['commMatos'] = array(
            'title' => t('commMatos'),
            'help' => t('commMatos'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_string'),
           'argument' => array('handler' => 'views_handler_argument_string'),
  );
    $data['gbb_evaluation']['rencontrePrep'] = array(
            'title' => t('rencontrePrep'),
            'help' => t('rencontrePrep'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_string'),
           'argument' => array('handler' => 'views_handler_argument_string'),
  );
    $data['gbb_evaluation']['satisEffectif'] = array(
            'title' => t('satisEffectif'),
            'help' => t('satisEffectif'),
            'field' => array(
              'handler' => 'views_handler_field_numeric',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_numeric'),
           'argument' => array('handler' => 'views_handler_argument_numeric'),
  );
   $data['gbb_evaluation']['commEffectif'] = array(
            'title' => t('commEffectif'),
            'help' => t('commEffectif'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_string'),
           'argument' => array('handler' => 'views_handler_argument_string'),
  );
   $data['gbb_evaluation']['satisAssiduite'] = array(
            'title' => t('satisAssiduite'),
            'help' => t('satisAssiduite'),
            'field' => array(
              'handler' => 'views_handler_field_numeric',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_numeric'),
           'argument' => array('handler' => 'views_handler_argument_numeric'),
  );
   $data['gbb_evaluation']['commAssiduite'] = array(
            'title' => t('commAssiduite'),
            'help' => t('commAssiduite'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_string'),
           'argument' => array('handler' => 'views_handler_argument_string'),
  );
    $data['gbb_evaluation']['satisCooper'] = array(
            'title' => t('satisCooper'),
            'help' => t('satisCooper'),
            'field' => array(
              'handler' => 'views_handler_field_numeric',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_numeric'),
           'argument' => array('handler' => 'views_handler_argument_numeric'),
  );
   $data['gbb_evaluation']['commCooper'] = array(
            'title' => t('commCooper'),
            'help' => t('commCooper'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_string'),
           'argument' => array('handler' => 'views_handler_argument_string'),
  );
  $data['gbb_evaluation']['satisPrerequis'] = array(
            'title' => t('satisPrerequis'),
            'help' => t('satisPrerequis'),
            'field' => array(
              'handler' => 'views_handler_field_numeric',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_numeric'),
           'argument' => array('handler' => 'views_handler_argument_numeric'),
  );
   $data['gbb_evaluation']['commPrerequis'] = array(
            'title' => t('commPrerequis'),
            'help' => t('commPrerequis'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_string'),
           'argument' => array('handler' => 'views_handler_argument_string'),
  );
  $data['gbb_evaluation']['satisObjectifs'] = array(
            'title' => t('satisObjectifs'),
            'help' => t('satisObjectifs'),
            'field' => array(
              'handler' => 'views_handler_field_numeric',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_numeric'),
           'argument' => array('handler' => 'views_handler_argument_numeric'),
  );
   $data['gbb_evaluation']['questionsTraitees'] = array(
            'title' => t('questionsTraitees'),
            'help' => t('questionsTraitees'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_string'),
           'argument' => array('handler' => 'views_handler_argument_string'),
  );
   $data['gbb_evaluation']['pointsForts'] = array(
            'title' => t('pointsForts'),
            'help' => t('pointsForts'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_string'),
           'argument' => array('handler' => 'views_handler_argument_string'),
  );
    $data['gbb_evaluation']['ameliorations'] = array(
            'title' => t('ameliorations'),
            'help' => t('ameliorations'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_string'),
           'argument' => array('handler' => 'views_handler_argument_string'),
  );
     $data['gbb_evaluation']['genants'] = array(
            'title' => t('genants'),
            'help' => t('genants'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_string'),
           'argument' => array('handler' => 'views_handler_argument_string'),
  );
     $data['gbb_evaluation']['besoinsParticip'] = array(
            'title' => t('besoinsParticip'),
            'help' => t('besoinsParticip'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_string'),
           'argument' => array('handler' => 'views_handler_argument_string'),
  );
   $data['gbb_evaluation']['besoinsForm'] = array(
            'title' => t('besoinsForm'),
            'help' => t('besoinsForm'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_string'),
           'argument' => array('handler' => 'views_handler_argument_string'),
  );
  $data['gbb_evaluation']['nbConvoques'] = array(
            'title' => t('nbConvoques'),
            'help' => t('nbConvoques'),
            'field' => array(
              'handler' => 'views_handler_field_numeric',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_numeric'),
           'argument' => array('handler' => 'views_handler_argument_numeric'),
  );
   $data['gbb_evaluation']['moyConvoques'] = array(
            'title' => t('moyConvoques'),
            'help' => t('moyConvoques'),
            'field' => array(
              'handler' => 'views_handler_field_numeric',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_numeric'),
           'argument' => array('handler' => 'views_handler_argument_numeric'),
  );
   $data['gbb_evaluation']['nbEvalStag'] = array(
            'title' => t('nbEvalStag'),
            'help' => t('nbEvalStag'),
            'field' => array(
              'handler' => 'views_handler_field_numeric',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_numeric'),
           'argument' => array('handler' => 'views_handler_argument_numeric'),
  );
   $data['gbb_evaluation']['syntheseEvalSta'] = array(
            'title' => t('syntheseEvalSta'),
            'help' => t('syntheseEvalSta'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_string'),
           'argument' => array('handler' => 'views_handler_argument_string'),
  );
    $data['gbb_evaluation']['syntheseEvalForm'] = array(
            'title' => t('syntheseEvalForm'),
            'help' => t('syntheseEvalForm'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_string'),
           'argument' => array('handler' => 'views_handler_argument_string'),
  );
    $data['gbb_evaluation']['condMateriellesAccueil'] = array(
            'title' => t('condMateriellesAccueil'),
            'help' => t('condMateriellesAccueil'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_string'),
           'argument' => array('handler' => 'views_handler_argument_string'),
  );
    $data['gbb_evaluation']['remqAnalysesRD'] = array(
            'title' => t('remqAnalysesRD'),
            'help' => t('remqAnalysesRD'),
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
