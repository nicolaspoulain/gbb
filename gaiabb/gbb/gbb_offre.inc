<?php
/**
 * @file
 *
 * Comweight de la page de gestion des évaluations
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

INSERT INTO gbb_gmoof_taxonomy (co_omodu,tid,weight)
SELECT co_omodu,tid,weight FROM gbb_gdiof_dafor
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

/**
 * Construit le tableau avec un vocabulaire par colonne
 */
function gbb_offre_tab($args) {

  //print_r(taxonomy_get_vocabularies());
  foreach (taxonomy_get_vocabularies() as $k=>$v) {
    $header[] = array('data' => t($v->name.' ').l('('.t('edit').')',
                            'admin/structure/taxonomy/'.$v->machine_name), 'class' => 'gris');
    $rows[0][] = array('data' => gbb_offre_vocab($k),
                       'class'=>'topAlign');
  }
  $output = theme('table',array('header' => $header, 'rows'   => $rows ));
  return $output;
}

/**
 * Construit le tableau avec une colonne contant les mots du vocabulaire $vid
 */
  function gbb_offre_vocab($vid) {

  $terms = taxonomy_get_tree($vid);
  $terms_select = array();
  foreach ($terms as $term) {
    $terms_select[$term->tid] = str_repeat('— ', $term->depth) . $term->name;
  };
  $check = db_query("SELECT tid,weight FROM {gbb_gmoof_taxonomy}
                                  WHERE co_omodu = :co_omodu",
                                  array(':co_omodu' => arg(1),))
                                  ->fetchAllkeyed();
  foreach ($terms_select as $key=>$val) {
    $pos= array_key_exists($key,$check)? $check[$key] : 500;
    $ischk= array_key_exists($key,$check);
    $rows[] = array(array('data'=>render(drupal_get_form('gbb_offre_taxonomy_form',$key,$val,$pos,$ischk,arg(1)))));
  }
  $output = theme('table',array('rows'   => $rows,'attributes' => array('id' => 'offre-table'.$key) ));
  return $output;
  }

/**
 * Construit un formualire pour chaque mot de vocabulaire
 */
function gbb_offre_taxonomy_form($form, &$form_state, $tid,$voc,$pos,$ischk,$co_omodu) {

    $form['f']['co_omodu']  = array('#type' => 'hidden','#value' => $co_omodu );
    $form['f']['tid']  = array('#type' => 'hidden','#value' => $tid );
    $form['f'][$tid] = array(
      '#type' => 'checkbox',
      '#title' => $voc,
      '#default_value' => $ischk,
      '#prefix' => '<div class="inline">',
      '#suffix' => '</div>',
      '#ajax' => array(
        'callback' => 'gbb_offre_taxonomy_form_submit',
        'progress' => array('type' => 'throbber', 'message' => '')),
      );
    $form['f']['weight'] = array(
      '#type' => 'textfield',
      '#size' => 2,
      '#default_value' => $pos,
      '#prefix' => '&nbsp;<div class="inline">',
      '#attributes' => array('class' => array('weight')),
      '#suffix' => '</div>',
      '#states' => array(    // This #states rule limits visibility
        'visible' => array(  // action to take.
          ':input[name='.$tid.']' => array('checked' => TRUE),),
      ),
      '#ajax' => array(
        'callback' => 'gbb_offre_taxonomy_form_submit',
        'progress' => array('type' => 'throbber', 'message' => '')),
      );
    return $form;
}

function gbb_offre_taxonomy_form_submit($form, &$form_state) {
  $tid = $form_state['values']['tid'];
  if ($form_state['values'][$tid] == 0) { // $form_state['values']['disc'][$key] not checked
    db_delete('gbb_gmoof_taxonomy')
        ->condition('co_omodu', $form_state['values']['co_omodu'] )
        ->condition('tid', $form_state['values']['tid'] )
        ->execute();
  } else {           // $form_state['values']['disc'][$key] was checked
      $nb = db_select('gbb_gmoof_taxonomy','m')
          ->fields('m',array('tid'))
          ->condition('co_omodu', $form_state['values']['co_omodu'] )
          ->condition('tid', $form_state['values']['tid']  )
          ->execute()->rowCount();
      if ($nb==0) {
        db_insert('gbb_gmoof_taxonomy')
          ->fields(array('co_omodu' => $form_state['values']['co_omodu'] ,
                              'tid' => $form_state['values']['tid'] ,
                         'weight' => $form_state['values']['weight']
            ))
            ->execute();
      } else {
        db_update('gbb_gmoof_taxonomy')
          ->fields(array('weight'=>$form_state['values']['weight']))
          ->condition('co_omodu', $form_state['values']['co_omodu'] )
          ->condition('tid', $form_state['values']['tid'] )
          ->execute();
      }
  }
}


/**
 * Affiche le contenu à compiler pour le CTP
 */
function gbb_offre_ctp_view($args = 'NaN') {

  // Construction de la page
  $build = array();
  $build['tab'] = gbb_offre_ctp($args);

  return implode('', $build);
}

/**
 * Construit le tableau avec un vocabulaire par colonne
 */
function gbb_offre_ctp($args) {

  //print_r(taxonomy_get_vocabularies());
  foreach (taxonomy_get_vocabularies() as $k=>$v) {
    $header[] = array('data' => t($v->name.' ').l('('.t('edit').')',
                            'admin/structure/taxonomy/'.$v->machine_name), 'class' => 'gris');
    $rows[0][] = array('data' => gbb_offre_ctp_vocab($k),
                       'class'=>'topAlign');
  }
  $output = theme('table',array('header' => $header, 'rows'   => $rows ));
  return $output;
}

/**
 * Construit le tableau avec une colonne contant les mots du vocabulaire $vid
 */
  function gbb_offre_ctp_vocab($vid) {

  $terms = taxonomy_get_tree($vid);
  $terms_select = array();
  foreach ($terms as $term) {
    $depth = $term->depth +1;
    $terms_select[$term->tid] = '((H' . $depth .'))' . $term->name . '<br/>';
  };
  foreach ($terms_select as $key=>$val) {
    $query = db_select('gbb_gmoof_taxonomy','t')
          ->condition('tid', $key )
          ->condition('weight', 500, '<');
    $query->leftjoin('gbb_gmoof', 'm',
                    't.co_omodu = m.co_omodu');
    $query->leftjoin('gbb_gdiof', 'd',
                    'm.co_offre = d.co_offre');
    $query->leftjoin('gbb_gresp', 'rm',
                    'm.co_resp = rm.co_resp');
    $query->fields('m',array('co_omodu','duree_prev','lpeda','lcont','lmoda','lform'));
    $query->addField('rm', 'nomu', 'nomu_modu');
    $query->addField('rm', 'prenom', 'prenom_modu');
    $query->addField('rm', 'qualite', 'qualite');
    $query->addField('m', 'libl', 'libmodu');
    $query->fields('d',array('no_offre','libl','lobjt'));
    $query->orderBy('weight', 'ASC');
    $list= $query->execute()->fetchAll();
    $str = "";
    foreach ($list as $f) {
      $str .= '((1))' . $f->no_offre;
      $str .= '((2))' . $f->libl;
      $str .= '((3))' . $f->lobjt;
      $str .= '((4))' . $f->co_omodu;
      $str .= '((5))' . $f->libmodu;
      $str .= '((6))' . $f->duree_prev;
      $str .= '((7))' . $f->prenom_modu.' '.$f->nomu_modu;
      $str .= '((8))' . $f->qualite;
      $str .= '((9))' . $f->lpeda;
      $str .= '((10))' . $f->lcont;
      $str .= '((11))' . $f->lmoda;
      $str .= '((12))' . $f->lform;
      $str .= '((13))';
      $str .= '<br/>';
    };
    $rows[] = $val." ".$str;
  }
  $output = implode('<br/>',$rows);
  return $output;
  }


