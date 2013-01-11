<?php

class GdispViewsController extends EntityDefaultViewsController {

/**
 * Override views_data().
 * Ajoute les liaisons des autres tables à la table gmodu
 */
  public function views_data() {
    $data = parent::views_data();

    // la classe EntityDefaultViewsController a créé les éléments
    // pour views à partir du hook_views du .install
    // On définit ici les liaisons entre les tables
    $data['g_gdisp']['table'] = array(
      'group' => t('g_gdisp'),
      'join' => array(
        'g_gmodu' => array(
          'left_table' => 'g_gdire',
          'left_field' => 'co_disp',
          'field' => 'co_disp',
          'extra' => 'g_gdisp.co_degre = g_gmodu.co_degre',
        ),
      ),
    );
    // gbb_gdire table de liaison, pas de champ à extraire
    $data['g_gdire']['table'] = array(
            'group' => t('g_gdire'),
            'join' => array(
              'g_gmodu' => array(
                'left_field' => 'co_modu',
                'field' => 'co_modu',
                'extra' => 'g_gdire.co_degre = g_gmodu.co_degre',
              ),
            ),
    );
    $data['g_gdire']['co_tres'] = array(
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

    $data['g_gdire']['co_resp'] = array(
            'title' => t('co_resp'),
            'help' => t('Code responsable'),
            'field' => array(
              'handler' => 'views_handler_field_numeric',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_numeric'),
           'argument' => array('handler' => 'views_handler_argument_numeric'),
    );


return $data;
  }
}

class SessionViewsController extends EntityDefaultViewsController {

/**
 * Override views_data().
 * Ajoute les liaisons des autres tables à la table gmodu
 */
  public function views_data() {
    $data = parent::views_data();
    $data['g_session']['table'] = array(
            'group' => t('g_session'),
            'join' => array(
              'g_gmodu' => array(
                'left_field' => 'co_modu',
                'field' => 'co_modu',
                'extra' => 'g_session.co_degre = g_gmodu.co_degre',
              ),
            ),
    );
    return $data;
  }
}

class GrespViewsController extends EntityDefaultViewsController {

/**
 * Override views_data().
 * Ajoute les liaisons des autres tables à la table gmodu
 */
  public function views_data() {
    $data = parent::views_data();
    $data['g_gresp']['table'] = array(
            'group' => t('g_gresp'),
            'join' => array(
              'g_gmodu' => array(
                'left_table' => 'g_session',
                'left_field' => 'co_resp',
                'field' => 'co_resp',
                'extra' => 'g_gresp.co_degre = g_gmodu.co_degre',
              ),
            ),
    );
    return $data;
  }
}

class NetabViewsController extends EntityDefaultViewsController {

/**
 * Override views_data().
 * Ajoute les liaisons des autres tables à la table gmodu
 */
  public function views_data() {
    $data = parent::views_data();
    $data['g_netab']['table'] = array(
            'group' => t('g_netab'),
            'join' => array(
              'g_gmodu' => array(
                'left_table' => 'g_session',
                'left_field' => 'co_lieu',
                'field' => 'co_lieu',
              ),
            ),
    );
    return $data;
  }
}

class NcontViewsController extends EntityDefaultViewsController {

/**
 * Override views_data().
 * Ajoute les liaisons des autres tables à la table gmodu
 */
  public function views_data() {
    $data = parent::views_data();
    $data['g_ncont']['table'] = array(
            'group' => t('g_ncont'),
            'join' => array(
              'g_gmodu' => array(
                'left_field' => 'co_cont',
                'field' => 'co_modu',
              ),
            ),
    );
    return $data;
  }
}



