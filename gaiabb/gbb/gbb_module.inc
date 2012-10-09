<?php
/**
 * @file
 *
 * Composition de la page de gestion de module
 *
 */

/**
 * Fonction d'appel aux differents elements composant la page
 */
function gbb_module_view($args = 'NaN') {
  // Charge la feuille des style et les fichiers javascript pour le module
  drupal_add_css(drupal_get_path('module', 'gbb') . '/css/gbb.css', 
                 array('group' => CSS_SYSTEM -1 , 'preprocess' => FALSE));
  drupal_add_library('system', 'drupal.collapse');

  // Construction de la page
  $build = array();
  $build['infos_general'] = gbb_infos_general($args);
  $build['infos_detail']  = theme('fieldset', 
                                  array(
                                    'element' => array(
                                      '#attributes' => array(
                                        'class' => array('details', 
                                                         'collapsible', 
                                                         'collapsed',
                                        ),
                                      ), 
                                      '#children' => '',
                                      '#title' => t('Détails ...'),
                                      '#value' => gbb_infos_detail($args),
                                    )
                                  )
                            );
  if (user_access('gest_sessions gbb')) {
      $build['organis_form'] = render(drupal_get_form('gbb_organis_form', $args));
  }

  $collapsed = (user_access('gest_convocs gbb'))? '' : 'collapsed';
  $build['sess_stagiaire'] = theme('fieldset', 
                                     array(
                                       'element' => array(
                                         '#attributes' => array(
                                           'class' => array('organisation', 
                                                            'collapsible', 
                                                            $collapsed   )),
                                         '#children' => '',
                                         '#title' => t('Convoc stagiaire ...'),
                                         '#value' => gbb_sess_stagiaire($args),
                                       )
                                     )
                               );
  $build['attachements'] = render(drupal_get_form('gbb_attachements', $args));
  $build['sess_formatr'] = render(drupal_get_form('gbb_sess_formatr', $args));
  if (isset($_GET['sess_id']) && $_GET['sess_id'] != 0 ) {
    $build['sess_edition'] = render(drupal_get_form('gbb_sess_edition', $args));
  };
  $build['convoc_form']  = render(drupal_get_form('gbb_convoc_form', $args));
  /*
  $build['bilan_form']   = render(drupal_get_form('gbb_bilan_form',$args));
*/
  return implode('', $build);
}

/**
 * Un tableau à deux lignes qui affiche le dispo et le module
 */
function gbb_infos_general($args) {

  $query = db_select('gbb_gmodu', 'm');
  $query ->join('gbb_gdisp', 'ds', 
                'ds.co_disp = m.co_disp AND ds.co_degre = m.co_degre');
  $query ->leftjoin('gbb_norie', 'n',  'n.co_orie = ds.co_orie');
  $query ->leftjoin('gbb_gdire', 'dr', 
                    'dr.co_modu = m.co_modu AND dr.co_degre = m.co_degre');
  $query ->leftjoin('gbb_gresp', 'r',  
                    'r.co_resp  = dr.co_resp AND r.co_degre = m.co_degre');
  $query ->condition('m.co_modu', arg(2), '=')
         ->condition('m.co_degre', arg(1), '=')
         ->fields('m',  array('co_modu', 'lautre', 'duree_prev',
                              'lmoda', 'lform',))
         ->fields('ds', array('lobjt', 'id_disp',
                              'co_camp', 'co_andi',))
         ->fields('r',  array('prenom', 'nomu',))
         ->fields('dr', array('co_resp', 'co_tres', 'co_degre',));
  $query ->addField('ds', 'lib', 'libd');
  $query ->addField('m',  'lib', 'libm');
  $query ->addField('n',  'lib_long', 'ori');
  $result = $query->execute();
  if (($result->rowCount() == 0)) return 'Module inconnu';

  // --------------------------------------------------------------------
  // Construction des liens vers liste des stages resp.Orga et resp.Péda
  $TRES = array( 1 => t("Interlocuteur"),
                 2 => t("Resp. orga."),
                 3 => t("Resp. péda.")
    );
  $voirEnCours = (    user_access('gest_convocs gaiabb') 
                 and !user_access('gest_sessions gaiabb'))? 1 : 0;
  $str="";
  foreach ($result as $r) {
    if ($r->co_tres == 1 || $r->co_tres == 2 || $r->co_tres == 3 ) {
      $str .= $TRES[$r->co_tres] . " : ";
      $str .= l( $r->prenom . ' ' . $r->nomu, 
                 'liste/12/' . $r->co_degre . '/0/' . $r->nomu, 
                 array('query' => array('voir_encours' => $voirEnCours))
              ) ;
      $str .= "&nbsp;&nbsp;&nbsp;-&nbsp;&nbsp;&nbsp;";
    }
  }
  // --------------------------------------------------------------------

  // Construction du tableau à trois lignes
  $paf_ou_fil = ($r->co_camp == 'BS')? 'fil' : 'paf';
  $ouvert_ou_ferme = ($r->co_andi == 'Fe')? 'Ferme' : '';
  $rows[] = array( 
              array('data' => $r->id_disp . ' ' . $r->libd,
                    'class' => array($paf_ou_fil, $ouvert_ou_ferme),
                    'colspan' => '2')
            );
  $rows[] = array( 
              array('data' => $r->co_modu . ' ' . $r->libm,
                    'class' => array($ouvert_ou_ferme),
                    'colspan' => '2')
            );
  $rows[] = array(array('data' => $str));

  return theme('table', array('rows' => $rows));
}

/**
 * Un tableau qui affiche les détails du module
 */
function gbb_infos_detail($args) {

  if ($args == 'NaN') return "";

  $query = db_select('gbb_gmodu', 'm');
  $query ->join('gbb_gdisp', 'ds', 
                'ds.co_disp = m.co_disp AND ds.co_degre = m.co_degre');
  $query ->leftjoin('gbb_norie', 'n',  'n.co_orie = ds.co_orie');
  $query ->leftjoin('gbb_gdire', 'dr', 
                    'dr.co_modu = m.co_modu AND dr.co_degre = m.co_degre');
  $query ->leftjoin('gbb_gresp', 'r',  
                    'r.co_resp  = dr.co_resp AND r.co_degre = m.co_degre');
  $query ->condition('m.co_modu', arg(2), '=')
         ->condition('m.co_degre', arg(1), '=')
         ->fields('m',  array('lautre', 'duree_prev', 'lmoda',
                              'lform', 'lcont', 'lpeda', 'lcibl',))
          ->fields('r', array('prenom', 'nomu',))
          ->fields('dr', array('co_resp', 'co_tres',));
  $query  ->addField('ds', 'lobjt');
  $query  ->addField('n', 'lib_long', 'ori');
  $result = $query->execute();

  if ($result->rowCount() == 0) return 'Hum !';
  $r = $result->fetchAssoc();

  $rows[] = array( array('data' => "Objectif Dispositif"),
                   array('data' => $r['lobjt']),);
  $rows[] = array( array('data' => "Objectif Module"),
                   array('data' => $r['lcont']),);
  $rows[] = array( array('data' => "Objectif pédagogique"),
                   array('data' => $r['lpeda']),);
  $rows[] = array( array('data' => "Cible"),
                   array('data' => $r['lcibl']),);
  $rows[] = array( array('data' => "Orientation"),
                   array('data' => $r['ori']),);
  $rows[] = array( array('data' => "Observation"),
                   array('data' => $r['lautre']),);
  $rows[] = array( array('data' => t("Durée")),
                   array('data' => $r['duree_prev'] . "h"),);
  $rows[] = array( array('data' => t("Modalité")),
                   array('data' => $r['lmoda']),);
  $rows[] = array( array('data' => "Forme"),
                   array('data' => $r['lform']),);
  return theme('table', array('rows' => $rows));
}

/**
 * Formulaire d' ** ORGANISATION ** de la formation
 */
function gbb_organis_form($form, &$form_state) {

  // requete sur gbb_gmodu_plus. Le rightjoin sur gbb_gmodu assure un résultat.
  $query = db_select('gbb_gmodu_plus', 'mp');
  $query ->rightjoin('gbb_gmodu', 'm', 
                     'm.co_modu=mp.co_modu AND m.co_degre=mp.co_degre');
  $query ->condition('m.co_modu', arg(2), '=')
         ->condition('m.co_degre', arg(1), '=')
         ->fields('mp', array('organisation', 'module_alert', 'prioritaire'));
  $r = $query->execute()->fetchAssoc();

  $form['f'] = array( '#type' => 'fieldset',
                      '#title' => t('Organisation'),
                      '#attributes' => array('class' => array('organisation', 
                                                              'collapsible'  )),
  );

  $html = $r['organisation'];
  $txt = preg_replace('#<br\s*/?>#i', "\n", $html); //ce <? est une ASTUCE
  $form['f']['organisation'] = array(
    '#type' => 'textarea',
    '#title' => t('Journal'),
    '#default_value' => $txt,
    '#ajax' => array(
      'callback' => 'form_organisation_submit',
      'progress' => array('type'    => 'throbber', 
                          'message' => 'Enregistrement...'),
    ),
  );

  $form['f']['prioritaire'] = array(
    '#type' => 'checkbox',
    '#title' => t( theme('image', variable_get('_gbb_prioritaire_icon')) . 
                   'Prioritaire' ),
    '#default_value' => $r['prioritaire'],
    '#ajax' => array(
      'callback' => 'form_organisation_submit',
      'progress' => array('type' => 'throbber',
                          'message' => 'Enregistrement...'),
    ),
  );

  $comodu = isset($form_state['values']['comodu'])? 
               $form_state['values']['comodu'] : arg(2);
  $form['f']['comodu'] = array('#type' => 'hidden',
                               '#value' => $comodu );

  $codegre = isset($form_state['values']['codegre'])? 
                $form_state['values']['codegre'] : arg(1);
  $form['f']['codegre'] = array('#type' => 'hidden',
                                '#value' => $codegre );

 return $form;
}

/**
 * ----> Submit
 */
function gbb_organis_form_submit(&$form, &$form_state) {
  db_merge('gbb_gmodu_plus')
    ->key(array('co_modu'  => $form_state['values']['comodu'],
                'co_degre' => $form_state['values']['codegre'] ))
    ->fields(array('organisation' => $form_state['values']['organisation'],
                   'prioritaire' => $form_state['values']['prioritaire']   ))
    ->execute();
}

/**
 * Tableau ** CONVOCATIONS STAGIAIRES ** pour le service adiministratif
 */
function gbb_sess_stagiaire($args) {

  if ($args == 'NaN') return "";

  $query = db_select('gbb_session', 'm');
  $query ->condition('m.co_modu',  arg(2), '=')
         ->condition('m.co_degre', arg(1), '=')
         ->condition('m.en_attente', 1, '<>')
         ->leftJoin('gbb_netab', 'e', 'e.co_lieu = m.co_lieu');
  $query ->fields('m', array('date_ts', 'groupe',
                             'horaires', 'co_lieu' ))
         ->fields('e', array('denom_princ', 'denom_comp',
                             'sigle' ));
  $query ->groupBy('co_lieu');
  $query ->groupBy('date_ts');
  $query ->groupBy('groupe');
  $query ->groupBy('horaires');
  $query ->orderBy('groupe', 'ASC');
  $query ->orderBy('date_ts', 'ASC');
  $result = $query->execute();
  $header = array('date_ts'  => t('Date'),
                  'horaires' => t('Horaires'),
                  'groupe'   => t('Gr'),
                  'etab'     => t('Établissement'),
                  'co_lieu'  => t('RNE'),
  );
  setlocale(LC_TIME, 'fr_FR');
  foreach ($result as $r) {
    $rows[] = array( 
                array('data' => utf8_encode(strftime('%a %e %h %Y ', $r->date_ts))),
                array('data' => $r->horaires),
                array('data' => $r->groupe),
                array('data' => $r->denom_princ . ' ' . 
                                $r->denom_comp . ' (' . $r->sigle . ')'),
                array('data' => $r->co_lieu),
    );
  }
  if (!isset($rows)) return t('Pas de convocation à traiter pour des stagiaires');
  return theme('table', array('rows' => $rows, 'header' => $header));
}

/**
 * Formulaire des ** ATTACHEMENTS **
 */
function gbb_attachements($form, &$form_state, $args) {
  $form['f'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fichiers joints'),
    '#attributes' => array('class' => array('attachements', 'collapsible')),
  );

  // -------------------------------------------
  // TODO réécrire avec la couche d'abstraction
  // -------------------------------------------
  $files = db_query("
     SELECT fm.fid, filename, uri, filemime, timestamp
     FROM {gbb_file} AS f
     JOIN {file_managed} AS fm ON fm.fid = f.fid
     WHERE co_modu  = :comodu
       AND co_degre = :codegre
       AND status = 1
     ", array(':codegre' => arg(1),
              ':comodu'  => arg(2)) );
  $icon_directory = variable_get('file_icon_directory', 
                                 drupal_get_path('module', 'file') . '/icons');
  $flist = array();
  foreach ($files as $f) {
    $icon = theme_file_icon(array('file' => $f, 
                            'icon_directory' =>  $icon_directory));
    $date = strftime('%d/%m/%Y à %X', $f->timestamp);
    $flist[$f->fid] = $icon . l($f->filename, 
                                file_create_url($f->uri), 
                                array('attributes' => array('title' => $date)));
  }

  if ( count($flist) > 0 ) {
    $form['f']['files']          = array( '#type'    => 'radios',
                                          '#options' => $flist, );
    $form['f']['supprimer_file'] = array( '#type'    => 'submit',
                                          '#value'   => t('Supprimer'), );
  }

  // Seules les extensions suivantes sont autorisées :
  $extensions = 'jpg jpeg gif png txt rtf doc docx odt xls xlsx ods pdf zip';
  $form['f']['Myfile'] = array(
    '#title' => t('Joindre un fichier à cette page'),
    '#type'  => 'managed_file',
    '#upload_location' => 'public://',
    '#process' => array('gbb_attachements_process'),
    '#upload_validators' => array(
      'file_validate_extensions' => array( $extensions ) ),
  );

  $form['f']['comodu'] = array(  '#type' => 'hidden',
                                '#value' => arg(2)   );
  $form['f']['codegre'] = array( '#type' => 'hidden',
                                '#value' => arg(1), );
  $form['f']['add_file'] = array(
    '#type' => 'submit',
    '#value' => t('Joindre'),
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>',
  );
  return $form;
}

/**
 * ----> modifie le comportement du formulaire d'attachement
 */
function gbb_attachements_process($element, &$form_state, $form) {

  $element = file_managed_file_process($element, $form_state, $form);
  $element['upload_button']['#access'] = FALSE;
  return $element;
}

/**
 * ----> Submit
 */
function gbb_attachements_submit($form, &$form_state) {

  // TODO : actuellement on écrit dans public:// 
  // il faudrait écrire dans le dir ci_dessous :
  // $dir = drupal_get_path('module', 'gaiabb') . '/files';

  //  if ( !file_prepare_directory($dir) ) {
  //    drupal_set_message( t('WARNING: Server DIR not accessible.'), 'status');
  //  }
  if ( isset($form_state['values']['files']) ) {
    $file = file_load($form_state['values']['files']);
    $file->status |= 'FILE_STATUS_TEMPORARY';
    $file = file_save($file);
    drupal_set_message( t('Le fichier n°@num a été archivé.', 
                        array('@num' => $form_state['values']['files']) ),
                        'warning' );
  } 
  elseif ( $form_state['values']['Myfile'] == '' ) {
    drupal_set_message( t('Echec. Veuillez remplir les champs avant de valider.'),
                        'warning' );
  } 
  else {
    $file = file_load($form_state['values']['Myfile']); // Load via file.fid.
    $file->status = FILE_STATUS_PERMANENT; // Change status to permanent.
    file_save($file);                      // Save.
    db_insert('gbb_file')
      ->fields(array('co_modu'  => $form_state['values']['comodu'],
                     'co_degre' => $form_state['values']['codegre'],
                     'fid'      => $file->fid))
      ->execute();
    drupal_set_message( t('Le nouveau fichier a été enregistré.') );
  }
}

/**
 * Formulaire du ** TABLEAU DES SESSIONS **
 */
function gbb_sess_formatr($form, $form_state, $args) {

  $header = array(
    'date_ts' => t('Date'),
    'icone' => '',
    'horaires' => t('Horaires'),
    'lieu' => t('Lieu'),
    'groupe' => t('Gr'),
    'formateur' => t('Formateur'),
    'dureeprevue' => t('Prévues'),
    'dureeapayer' => t('À payer'),
    'TypePaiement' => t('Type pmt'),
  );

  $query = db_select('gbb_session', 's');
  $query ->leftjoin('gbb_gmodu_plus', 'mp', 
                    'mp.co_modu = s.co_modu AND mp.co_degre = s.co_degre');
  $query ->leftjoin('gbb_gresp_dafor', 'r', 
                    'r.co_resp = s.co_resp AND r.co_degre = s.co_degre');
  $query ->leftjoin('gbb_netab', 'e', 
                    'e.co_lieu = s.co_lieu');
  $query ->orderBy('groupe', 'ASC');
  $query ->orderBy('date_ts', 'ASC');
  $query ->orderBy('nomu', 'ASC');
  $query ->condition('mp.co_modu', arg(2), '=')
         ->condition('mp.co_degre', arg(1), '=')
         ->fields('s', array('sess_id', 'date_ts', 'dureeprevue', 
                             'dureeapayer', 'typePaiement',
                             'en_attente', 'convoc_sent', 'session_alert',
                             'horaires', 'co_lieu', 'co_resp', 'groupe'))
         ->fields('mp', array('date_modif_ts'))
         ->fields('r', array('prenom', 'nomu', 'co_resp'))
         ->fields('e', array('denom_princ', 'denom_comp', 'sigle'));
  // Une condition supplémentaire pour les administratifs
  // afin que les stages en attente ne leur apparaissent pas.
  if (user_access('gest_convocs gbb') && !user_access('gest_sessions gbb')) {
    $query->condition('s.en_attente', 0, '=');
  }
  $result = $query->execute();
  if (($result->rowCount() == 0)) return 'Aucun';

  $default = (isset($_GET['sess_id']))? $_GET['sess_id'] : 0;

  $row = array();
  setlocale(LC_TIME, 'fr_FR');

  foreach ($result as $r) {
    $visibilite = ($r->en_attente)? 'grisclair' : '';
    if ($r->en_attente) {
          $icon = theme('image', variable_get('_gbb_en_attente_icon'));
    } elseif ($r->session_alert) {
          $icon = theme('image', variable_get('_gbb_session_alert_icon'));
    } elseif ($r->convoc_sent) {
          $icon = theme('image', variable_get('_gbb_convoc_sent_icon'));
    } else {
          $icon = theme('image', variable_get('_gbb_not_en_attente_icon'));
    };
    $row[$r->sess_id] = array(
      'date_ts' => utf8_encode(strftime('%a %e %h %Y ', $r->date_ts)),
      'icone' => $icon,
      'horaires' => $r->horaires,
      'lieu' => $r->denom_princ . ' ' . $r->denom_comp .
                ' (' . $r->sigle . ')' . '&nbsp;' .
                l( theme('image', variable_get('_gbb_etab_edit_icon')),
                   'etablissement/' . $r->co_lieu,
                   array('attributes' => array('class' => 'anchor-class'), 
                         'html' => TRUE )
                ),
      'groupe' => $r->groupe,
      'formateur' => $r->prenom . ' ' . $r->nomu . '&nbsp;' .
                l( theme('image', variable_get('_gbb_user_edit_icon')),
                   'formateur/' . $r->co_resp,
                   array('attributes' => array('class' => 'anchor-class'), 
                         'html' => TRUE )
                ),
      'dureeprevue' => $r->dureeprevue . "h",
      'dureeapayer' => $r->dureeapayer . "h",
      'TypePaiement' => $r->typePaiement,
      '#attributes'  => array('class' => array($visibilite)),
    );
    $date_modif_ts = $r->date_modif_ts;
  }

  setlocale(LC_TIME, 'fr_FR');
  $form['f'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sessions - Dernière modifcation : ' . 
         utf8_encode(strftime('%A %e %B %Y %T', $date_modif_ts)) ),
    '#attributes' => array('class' => array('sessions', 'collapsible')),
  );



  $form['f']['table'] = array
  (
    '#type' => 'tableselect',
    '#header' => $header,
    '#multiple' => FALSE,
    '#options' => $row,
    '#default_value' => ((isset($_GET['sess_id']))? $_GET['sess_id'] : 0),
    '#empty' => t('Aucune session. cliquez sur Ajouter pour en créer.'),
  );
  if (user_access('gest_convocs gbb') && !user_access('gest_sessions gbb')) {
    $form['f']['table']['#multiple'] = TRUE;
    $form['f']['table']['#default_value'] = array();
  }
  $form['f']['comodu'] = array(  '#type' => 'hidden',
                                '#value' => arg(2)   );
  $form['f']['codegre'] = array( '#type' => 'hidden',
                                '#value' => arg(1)   );

  if (user_access('gest_sessions gbb')) {
    $form['f']['ajouter'] = array(
      '#type' => 'submit',
      '#value' => t('Ajouter'),
      '#submit' => array('gbb_sess_formatr_ajouter_submit'),
);
    $form['f']['modifier'] = array(
      '#type' => 'submit',
      '#value' => t('Modifier'),
      '#submit' => array('gbb_sess_formatr_modifier_submit'),
    );
    $form['f']['dupliquer'] = array( 
      '#type' => 'submit',
      '#value' => t('Dupliquer'),
      '#submit' => array('gbb_sess_formatr_dupliquer_submit'),
    );
    $form['f']['supprimer'] = array(
      '#type' => 'submit',
      '#value' => t('*Supprimer*'),
      '#submit' => array('gbb_sess_formatr_supprimer_submit'),
    );
  }

  if (user_access('gest_convocs gaiabb')) {
    $form['f']['convoc_envoyee'] = array(
      '#type' => 'submit',
      '#value' => t('Convocs envoyées'),
      '#submit' => array('gbb_sess_formatr_convoc_submit'),
    );
  }
  return $form;
}

/**
 * ----> Submit
 */
function gbb_sess_formatr_supprimer_submit($form, &$form_state) {
  if (!isset($form_state['values']['table']) 
        || $form_state['values']['table'] == 0) {
    drupal_set_message(t('Choisissez la session à supprimer.'),'error');
    return;
 }
  db_delete('gbb_session') // supprime la session
      ->condition('sess_id', $form_state['values']['table'])
      ->execute();

  db_merge('gbb_gmodu_plus')
      ->key(array('co_modu'  => $form_state['values']['comodu'],
                  'co_degre' => $form_state['values']['codegre']))
      ->fields(array('date_modif_ts' => time()))
      ->execute(); // met à jour la date de modification des sessions

  drupal_set_message(t('La session a été supprimée.'));
}

function gbb_sess_formatr_dupliquer_submit($form, &$form_state) {
  if (!isset($form_state['values']['table']) 
        || $form_state['values']['table'] == 0) {
    drupal_set_message(t('Choisissez la session à dupliquer.'),'error');
    return;
 }
  $query = db_select('gbb_session', 's') // récupère la session à cloner
         ->fields('s',array('session_alert', 'co_resp', 'co_modu',
                            'co_degre', 'co_lieu', 'date_ts', 'groupe',
                            'horaires', 'typePaiement', 
                            'dureeapayer', 'dureeprevue'))
         ->condition('s.sess_id', $form_state['values']['table'], '=');

  db_insert('gbb_session')->from($query)->execute(); // clone la session

  $query = db_select('gbb_session', 's');
  $query ->addExpression('MAX(sess_id)', 'newid');
  $result = $query->execute()->fetchAssoc(); // recupère l'id du clone
  drupal_set_message(t('La session a été dupliquée.'.$query));
  
  $url = url('module/' . $form_state['values']['codegre'] .
                   '/' . $form_state['values']['comodu'],
             array('fragment' => 'edit_session_anchor',
                   'query'    => array('sess_id'  => $result['newid']), 
                                       'absolute' => TRUE));
  $form_state['redirect'] = $url;
}

function gbb_sess_formatr_ajouter_submit($form, &$form_state) {
  db_insert('gbb_session')
      ->fields(array( 'co_modu' => arg(2),
                      'co_degre' => arg(1),
                      'date_ts' => time()  ))
      ->execute(); // ajoute une session toute neuve

  $query = db_select('gbb_session', 's');
  $query ->addExpression('MAX(sess_id)', 'newid');
  $result = $query->execute()->fetchAssoc(); // recupère l'id du clone

  drupal_set_message(t('La session a été créée.'.$query));

  $url = url('module/' . $form_state['values']['codegre'] .
                   '/' . $form_state['values']['comodu'],
             array('fragment' => 'edit_session_anchor',
                   'query'    => array('sess_id'  => $result['newid']), 
                                       'absolute' => TRUE));
  $form_state['redirect'] = $url;
}

function gbb_sess_formatr_modifier_submit($form, &$form_state) {
  if (!isset($form_state['values']['table']) 
        || $form_state['values']['table'] == 0) {
    drupal_set_message(t('Choisissez la session à modifier.'),'error');
    return;
 }
  $url = url('module/' . $form_state['values']['codegre'] .
                   '/' . $form_state['values']['comodu'],
             array('fragment' => 'edit_session_anchor',
                   'query' => array('sess_id' => $form_state['values']['table']),
                                   'absolute' => TRUE));
  $form_state['redirect'] = $url;
}

function gbb_sess_formatr_convoc_submit($form, &$form_state) {
  if (is_array($form_state['values']['table'])) {
    foreach ($form_state['values']['table'] as $i) {
      db_merge('gbb_session') // Marque comme envoyée chaque session cochée
        ->key(array('sess_id'  => $i))
        ->fields(array('convoc_sent' => 1,
                       'session_alert' => 0))
        ->execute();
    }
  } else {                    // boutons radion au lieu des cases à cocher
      db_merge('gbb_session') // Marque comme envoyée la session choisie
        ->key(array('sess_id'  => $form_state['values']['table']))
        ->fields(array('convoc_sent' => 1,
                       'session_alert' => 0))
        ->execute();
  }
  drupal_set_message(t('Les convocs ont été marquées comme envoyées.'));
}

/**
 * Formulaire de ** MODIF D'UNE SESSION ** particulière
 */
function gbb_sess_edition($form, $form_state, $args) {

  // On récupère les valeurs associées à la session pour peupler le formulaire
  $query = db_select('gbb_session', 's');
  $query ->leftjoin('gbb_gresp_dafor', 'r', 
                     'r.co_resp=s.co_resp AND r.co_degre=s.co_degre');
  $query ->leftjoin('gbb_netab', 'e', 'e.co_lieu=s.co_lieu');
  $query ->condition('sess_id', $_GET['sess_id'], '=')
         ->fields('s', array('sess_id', 'date_ts', 'horaires', 'groupe',
                             'dureeprevue', 'dureeapayer', 'typePaiement',
                             'session_alert', 'en_attente', 'convoc_sent'))
         ->fields('r', array('co_resp', 'nomu', 'prenom'))
         ->fields('e', array('co_lieu', 'denom_comp', 'sigle'));
  $r = $query->execute()->fetchObject();

  $form['f'] = array(
    '#type' => 'fieldset',
    '#title' => t('Editer/Ajouter la session n°' . $r->sess_id . 
                  '<a name="edit_session_anchor"></a>'),
    '#attributes' => array('class' => array('eeditsession')),
  );

  $form['f']['date_ts'] = array(
    '#type' => 'date_popup', 
    '#title' => t('Date'),
    '#default_value' => strftime('%Y-%m-%d %T', $r->date_ts),
    '#date_format' => 'y-m-d',
    '#date_label_position' => 'within', // see date_api_elements.inc
    '#date_year_range' => '-1:+1', // Optional, (-3:+3 years is the default).
    '#size' => 10,
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>&nbsp;',
  );

  $form['f']['horaires'] = array(
    '#type' => 'textfield',
    '#title' => 'Horaires',
    '#default_value' => $r->horaires,
    '#size' => 20,
    '#maxlength' => 50,
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>&nbsp;',
  );
  $form['f']['groupe'] = array(
    '#type' => 'textfield',
    '#title' => t('Groupe'),
    '#default_value' => $r->groupe,
    '#size' => 1,
    '#maxlength' => 2,
    '#element_validate' => array('element_validate_integer_positive'),
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>&nbsp;',
  );

  $form['f']['dureeprevue'] = array(
    '#type' => 'textfield',
    '#title' => t('Durée prévue'),
    '#default_value' => $r->dureeprevue,
    '#size' => 10,
    '#maxlength' => 6,
    '#element_validate' => array('element_validate_number'),
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>&nbsp;',
  );

  $form['f']['dureeapayer'] = array(
    '#type' => 'textfield',
    '#title' => t('Duree à payer'),
    '#default_value' => $r->dureeapayer,
    '#size' => 10,
    '#maxlength' => 6,
    '#element_validate' => array('element_validate_number'),
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>&nbsp;',
  );

  $form['f']['typePaiement'] = array(
    '#type' => 'select',
    '#title' => t('Type de paiement'),
    '#options' => array( 'VAC'  => t('VAC'),
                         'CONV' => t('CONV'),
                         'BDC'  => t('BDC'),
                         'DECH' => t('DECH'),
                         'HSE'  => t('HSE'),
                         'SF'   => t('SF'),  ),
    '#default_value' => $r->typePaiement,
    '#maxlength' => 4,
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>',
  );

  $default_value = ($r->convoc_sent)? 1 : $r->session_alert;
  $form['f']['session_alert'] = array(
    '#type' => 'checkbox',
    '#title' => t('Alerte'),
    '#default_value' => $default_value,
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>&nbsp;',
    '#states' => array(    // This #states rule limits visibility
      'visible' => array(  // action to take.
         ':input[name="en_attente"]' => array('checked' => FALSE),
          ':input[name="convoc_sent"]' => array('checked' => FALSE), ),
    ),
  );
  if ($r->convoc_sent != 1 || user_access('gest_sessions gbb') ) {
    $form['f']['en_attente'] = array(
      '#type' => 'checkbox',
      '#title' => t('En attente'),
      '#default_value' => $r->en_attente,
      '#prefix' => '<div class="inline">',
      '#suffix' => '</div>&nbsp;',
      '#states' => array(    // This #states rule limits visibility
        'visible' => array(  // action to take.
          ':input[name="session_alert"]' => array('checked' => FALSE),
          ':input[name="convoc_sent"]' => array('checked' => FALSE),  ),
      ),
    );
  }
  else {
    $form['f']['en_attente'] = array( '#type'  => 'hidden',
                                      '#value' => 0        );
  }

  if (user_access('gest_convocs gbb')) {
    $form['f']['convoc_sent'] = array(
      '#type' => 'checkbox',
      '#title' => t('Convoc envoyée'),
      '#default_value' => $r->convoc_sent,
      '#prefix' => '<div class="inline">',
      '#suffix' => '</div>&nbsp;',
      '#states' => array(    // This #states rule limits visibility
        'visible' => array(  // action to take.
           ':input[name="session_alert"]' => array('checked' => FALSE),
           ':input[name="en_attente"]' => array('checked' => FALSE)    ),
      ),
    );
  } 
  else {
    $form['f']['convoc_sent'] = array( '#type'  => 'hidden',
                                       '#value' => 0        );
  }

  $form['f']['lieu'] = array(
    '#type' => 'textfield',
    '#title' => t('Lieu&nbsp;' . 
                  l( theme('image',variable_get('_gbb_etab_add_icon')),
                     'etablissement/',
                     array('attributes' => array('class' => 'anchor-class'),
                                                 'html' => TRUE))
                ),
    '#default_value' => t("$r->sigle $r->denom_comp (rne:$r->co_lieu)"),
    '#size' => 40,
    '#maxlength' => 50,
    '#autocomplete_path' => 'autocomp/etablissement',
    '#prefix' => '<br/><div class="inline">',
    '#suffix' => '</div>&nbsp;',
  );
  if (!user_access('gest_etablissements gaiabb')) {
      $form['f']['lieu']['#title'] = t('Lieu');
  };

  $form['f']['formateur'] = array(
    '#type' => 'textfield',
    '#title' => t('Lieu&nbsp;' . 
                  l( theme('image',variable_get('_gbb_user_add_icon')),
                     'formateur/',
                     array('attributes' => array('class' => 'anchor-class'),
                                                 'html' => TRUE))
                ),
    '#default_value' => t("$r->prenom $r->nomu (id:$r->co_resp)"),
    '#size' => 30,
    '#autocomplete_path' => 'autocomp/formateur',
    '#maxlength' => 50,
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>',
  );

  $form['f']['co_degre'] = array( '#type' => 'hidden',
                                 '#value' => arg(1)  );
  $form['f']['co_modu']  = array( '#type' => 'hidden',
                                 '#value' => arg(2)  );
  $form['f']['sess_id']  = array( '#type' => 'hidden',
                                 '#value' => $r->sess_id );
  $form['f']['submit']  = array(  '#type' => 'submit',
                                 '#value' => t('Submit') );
  return $form;
}

/**
 * ----> Submit
 */
function gbb_sess_edition_submit($form, &$form_state) {
  $replace = array ('\1');
  $pattern = array ('/.*\(rne:(.*)\)/');
  $co_lieu = preg_replace($pattern, $replace,  
                          $form_state['values']['lieu']);
  $pattern = array ('/.*\(id:(.*)\)/');
  $co_resp = preg_replace($pattern, $replace,
                          $form_state['values']['formateur']);
  db_merge('gbb_session')
      ->key(array('sess_id'  => $form_state['values']['sess_id'] ))
      ->fields(array(
                     'co_modu'       => $form_state['values']['co_modu'],
                     'groupe'        => $form_state['values']['groupe'],
                     'convoc_sent'   => $form_state['values']['convoc_sent'],
                     'en_attente'    => $form_state['values']['en_attente'],
                     'session_alert' => $form_state['values']['session_alert'],
                     'horaires'      => $form_state['values']['horaires'],
                     'dureeprevue'   => $form_state['values']['dureeprevue'],
                     'dureeapayer'   => $form_state['values']['dureeapayer'],
                     'typePaiement'  => $form_state['values']['typePaiement'],
                     'co_lieu'       => $co_lieu,
                     'co_resp'       => $co_resp))
      ->execute(); // met à jour la session
  db_merge('gbb_gmodu_plus')
      ->key(array('co_modu'  => $form_state['values']['co_modu'],
                  'co_degre' => $form_state['values']['co_degre'] ))
      ->fields(array('date_modif_ts' => time()))
      ->execute(); // met à jour la date de modification des sessions

  drupal_set_message(t("Les modifications ont été prises en compte."));
}

/**
 * Formulaire de ** CONVOCATIONS INFOS ** à porter ou non sur la convoc
 */
function gbb_convoc_form($form, &$form_state) {

  // requete sur gbb_gmodu_plus. Le rightjoin sur gbb_gmodu assure un résultat.
  $query = db_select('gbb_gmodu_plus', 'mp');
  $query ->rightjoin('gbb_gmodu', 'm', 
                     'm.co_modu=mp.co_modu AND m.co_degre=mp.co_degre');
  $query ->condition('m.co_modu', arg(2), '=')
         ->condition('m.co_degre', arg(1), '=')
         ->fields('mp', array('convoc_info_off', 'convoc_info_on', 
                              'module_printed', 'module_alert'    ));
  $r = $query->execute()->fetchAssoc();

  $form['f'] = array(
    '#type' => 'fieldset',
    '#title' => t('Convocation'),
    '#attributes' => array('class' => array('collapsible', 'convoc')),
  );

  $form['f']['envoyer']['#markup']= 
    '<div class="flright">' . 
    theme('image', variable_get('_gbb_disk_icon')) . 
    '</div>';

        //'path' => ($r->module_printed)? 
        //drupal_get_path('module', 'gaiabb') . '/images/convoc_printed.gif' : 
        //drupal_get_path('module', 'gaiabb') . '/images/blank.gif',


  if (user_access('gest_convocs gbb')) {
    $form['f']['module_printed'] = array(
      '#type' => 'checkbox',
      '#title' => theme('image', variable_get('_gbb_print_icon')) . 
                  t('Imprimé'),
      '#default_value' => $r['module_printed'],
      '#prefix' => '<div class="inline">',
      '#suffix' => '</div>',
      '#ajax' => array(
        'callback' => 'form_module_printed_submit',
        'progress' => array('type' => 'throbber', 
                            'message' => 'Enregistrement...')),
    );
  }
  else {
    $form['f']['module_printed'] = array('#type' => 'hidden',
                                         '#value' => $r['module_printed']);
  }

  $form['f']['module_alert'] = array(
    '#type' => 'checkbox',
    '#title' => theme('image', variable_get('_gbb_module_alert_icon')) . 
                t('Alerte'),
    '#default_value' => $r['module_alert'],
    '#ajax' => array(
      'callback' => 'form_module_alert_submit',
      'progress' => array('type' => 'throbber', 
                          'message' => 'Enregistrement...')),
  );

  $html = $r['convoc_info_off'];
  $txtOff = preg_replace('#<br\s*/?>#i', "\n", $html); //ce <? est une ASTUCE
  $form['f']['convoc_info_off'] = array(
    '#type' => 'textarea',
    '#title' => t('Info à ne pas porter sur la convocation'),
    '#default_value' => $txtOff,
    '#cols' => 50,
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>',
    '#ajax' => array(
      'callback' => 'form_convoc_info_off_submit',
      'progress' => array('type' => 'throbber', 
                          'message' => 'Enregistrement...')),
  );

  $html = $r['convoc_info_on'];
  $txtOn = preg_replace('#<br\s*/?>#i', "\n", $html); //ce <? est une ASTUCE
  $form['f']['convoc_info_on'] = array(
    '#type' => 'textarea',
    '#title' => t('Info à porter sur la convocation'),
    '#default_value' => $txtOn,
    '#cols' => 50,
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>',
    '#ajax' => array(
      'callback' => 'form_convoc_info_on_submit',
      'progress' => array('type' => 'throbber', 
                          'message' => 'Enregistrement...')),
  );

  if (    user_access('gest_convocs gaiabb') 
      && !user_access('gest_sessions gaiabb')) {
    $form['f']['convoc_info_on']['#type']= 'markup';
    $form['f']['convoc_info_on']['#markup']=  '<div class="inlineEncadre">' .
      '<b>Infos a porter sur la convoc</b><br/>' . nl2br($txtOn) . '</div>';
    $form['f']['convoc_info_off']['#type']= 'markup';
    $form['f']['convoc_info_off']['#markup']= '<div class="inlineEncadre">' .
      '<b>Infos a ne pas porter sur la convoc</b><br/>' . nl2br($txtOff) . 
      '</div>';
  };


  $comodu = isset($form_state['values']['comodu'])? 
              $form_state['values']['comodu'] : arg(2);
  $form['f']['comodu'] = array('#type'  => 'hidden',
                               '#value' => $comodu  );

  $codegre = isset($form_state['values']['codegre'])? 
              $form_state['values']['codegre'] : arg(1);
  $form['f']['codegre'] = array('#type' => 'hidden',
                                '#value' => $codegre );

 return $form;
}

/**
 * ----> Submit
 */
function form_convoc_info_off_submit(&$form, &$form_state) {
  db_merge('gbb_gmodu_plus')
    ->key(array('co_modu'  => $form_state['values']['comodu'],
                'co_degre' => $form_state['values']['codegre'] ))
    ->fields(array('convoc_info_off' => $form_state['values']['convoc_info_off']))
    ->execute();
}
function form_convoc_info_on_submit(&$form, &$form_state) {
  db_merge('gbb_gmodu_plus')
    ->key(array('co_modu'  => $form_state['values']['comodu'],
                'co_degre' => $form_state['values']['codegre'] ))
    ->fields(array('convoc_info_on' => $form_state['values']['convoc_info_on']))
    ->execute();
}
function form_module_alert_submit(&$form, &$form_state) {
  db_merge('gbb_gmodu_plus')
    ->key(array('co_modu'  => $form_state['values']['comodu'],
                'co_degre' => $form_state['values']['codegre'] ))
    ->fields(array('module_alert' => $form_state['values']['module_alert'],))
    ->execute();
}
function form_module_printed_submit(&$form, &$form_state) {
  db_merge('gbb_gmodu_plus')
    ->key(array('co_modu'  => $form_state['values']['comodu'],
                'co_degre' => $form_state['values']['codegre'] ))
    ->fields(array('module_printed' => $form_state['values']['module_printed'],))
    ->execute();
}


/**
 * Formulaire de ** BILANS INFOS ** Listes d'émargement, bilans etc.
 */
function bilan_form($form, &$form_state) {

  global $user;
  $r->convoc_info_off = '';
  $r->convoc_info_on = '';
  $r->module_printed = 0;
  $r->module_alert = 0;

  // requete sur gbb_gmodu_plus. Le rightjoin sur gbb_gmodu assure un résultat.
  $query = db_select('gbb_gmodu_plus', 'mp');
  $query  ->rightjoin('gbb_gmodu', 'm', 'm.co_modu=mp.co_modu AND m.co_degre=mp.co_degre');
  $query  ->condition('m.co_modu', arg(2), '=')
          ->condition('m.co_degre', arg(1), '=')
          ->fields('mp', array('LE_etat', 'LE_info', 
                               'DSF_etat', 'DSF_info',
                               'DA_date_depot'));
  $r = $query->execute()->fetchObject();
  $Go = array(
        'path' => drupal_get_path('module', 'gaiabb') . '/images/disk.png',
        'alt' => 'enregistrer',
        'title' => t('Enregistrer.'),
        'attributes' => array('class' => 'some-img', 'id' => 'my-img'));
 
  $form['f'] = array(
    '#type' => 'fieldset',
    '#title' => t('Dossier'),
    '#attributes' => array('class' => array('dossier', 'collapsible', 'convoc')),
  );

  $form['f']['envoyer']['#markup']= '<div class="flright">' . theme('image', $Go)  . '</div>';

  $html = $r->LE_info;
  $txtOff = preg_replace('#<br\s*/?>#i', "\n", $html); //<? #### ASTUCE coloration syntaxique 
  $form['f']['LE_info'] = array(
    '#type' => 'textarea',
    '#title' => t('Listes d\'émargement rendues au conseiller'),
    '#default_value' => $txtOff,
    '#cols' => 50,
    '#rows' => 2,
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>',
    '#ajax' => array
    (
      'callback' => 'LE_info_form_submit',
      'progress' => array('type' => 'throbber', 'message' => 'Enregistrement...'),
    ),
  );
  $form['f']['LE_etat_part'] = array(
    '#type' => 'checkbox',
    '#title' => t('Partiellement'),
    '#default_value' => (($r->LE_etat == 1)? 1 : 0),
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>',
    '#ajax' => array
    (
      'callback' => 'LE_etat_part_form_submit',
      'progress' => array('type' => 'throbber', 'message' => 'Enregistrement...'),
    ),
   // This #states rule limits visibility to when the $form['f']['en_attente']
   // checkbox is not checked."
    '#states' => array(
      'visible' => array(  // action to take.
         ':input[name="LE_etat_total"]' => array('checked' => FALSE),
      ),
    ),
   );

  $form['f']['LE_etat_total'] = array(
    '#type' => 'checkbox',
    '#title' => t('En totalité'),
    '#default_value' => (($r->LE_etat == 2)? 1 : 0),
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>',
    '#ajax' => array
    (
      'callback' => 'LE_etat_total_form_submit',
      'progress' => array('type' => 'throbber', 'message' => 'Enregistrement...'),
    ),
   // This #states rule limits visibility to when the $form['f']['en_attente']
   // checkbox is not checked."
    '#states' => array(
      'visible' => array(  // action to take.
         ':input[name="LE_etat_part"]' => array('checked' => FALSE),
      ),
    ),
   );

  $form['f']['BR']['#markup']= '<br/>';

  $html = $r->DSF_info;
  $txtOff = preg_replace('#<br\s*/?>#i', "\n", $html); //<? #### ASTUCE coloration syntaxique 
  $form['f']['DSF_info'] = array(
    '#type' => 'textarea',
    '#title' => t('Déclarations de service fait rendues au conseiller'),
    '#default_value' => $txtOff,
    '#cols' => 50,
    '#rows' => 2,
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>',
    '#ajax' => array
    (
      'callback' => 'DSF_info_form_submit',
      'progress' => array('type' => 'throbber', 'message' => 'Enregistrement...'),
    ),
  );
  $form['f']['DSF_etat_part'] = array(
    '#type' => 'checkbox',
    '#title' => t('Partiellement'),
    '#default_value' => (($r->DSF_etat == 1)? 1 : 0),
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>',
    '#ajax' => array
    (
      'callback' => 'DSF_etat_part_form_submit',
      'progress' => array('type' => 'throbber', 'message' => 'Enregistrement...'),
    ),
   // This #states rule limits visibility to when the $form['f']['en_attente']
   // checkbox is not checked."
    '#states' => array(
      'visible' => array(  // action to take.
         ':input[name="DSF_etat_total"]' => array('checked' => FALSE),
      ),
    ),
   );

  $form['f']['DSF_etat_total'] = array(
    '#type' => 'checkbox',
    '#title' => t('En totalité'),
    '#default_value' => (($r->DSF_etat == 2)? 1 : 0),
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>',
    '#ajax' => array
    (
      'callback' => 'DSF_etat_total_form_submit',
      'progress' => array('type' => 'throbber', 'message' => 'Enregistrement...'),
    ),
   // This #states rule limits visibility to when the $form['f']['en_attente']
   // checkbox is not checked."
    '#states' => array(
      'visible' => array(  // action to take.
         ':input[name="DSF_etat_part"]' => array('checked' => FALSE),
      ),
    ),
   );

  $form['f']['BR']['#markup']= '<br/>';

  //$date = strftime('%F');  //$date = '2008-12-31 00:00:00';

  // Provide a format using regular PHP format parts (see documentation on php.net).
  // If you're using a date_select, the format will control the order of the date parts in the selector,
  // rearrange them any way you like. Parts left out of the format will not be displayed to the user.
  $format = 'Y-m-d';

  $form['f']['DA_date_depot'] = array(
     '#type' => 'date_popup', // types 'date_popup', 'date_text' and 'date_timezone' are also supported. See .inc file.
     '#title' => t('Date de dépôt du dossier'),
     '#default_value' => (($r->DA_date_depot == '1970-01-01 00:00:00')? '' : $r->DA_date_depot),
     '#date_format' => $format,
     '#date_label_position' => 'within', // See other available attributes and what they do in date_api_elements.inc
     '#date_year_range' => '-1:+1', // Optional, used to set the year range (back 3 years and forward 3 years is the default).
    '#ajax' => array
    (
      'callback' => 'DA_date_depot_form_submit',
      'progress' => array('type' => 'throbber', 'message' => 'Enregistrement...'),
    ),
  );

  $comodu = isset($form_state['values']['comodu'])? $form_state['values']['comodu'] : arg(2);
  $form['f']['comodu'] = array(
    '#type' => 'hidden',
    '#value' => $comodu,
  );

  $codegre = isset($form_state['values']['codegre'])? $form_state['values']['codegre'] : arg(1);
  $form['f']['codegre'] = array(
    '#type' => 'hidden',
    '#value' => $codegre,
  );

 return $form;
}
/**
 * ----> Submit
 */
function LE_etat_part_form_submit(&$form, &$form_state) {
  db_merge('gbb_gmodu_plus')
    ->key(array('co_modu'  => $form_state['values']['comodu'],
                'co_degre' => $form_state['values']['codegre'],))
    ->fields(array('LE_etat' => $form_state['values']['LE_etat_part'],))
    ->execute();
}
function LE_etat_total_form_submit(&$form, &$form_state) {
  db_merge('gbb_gmodu_plus')
    ->key(array('co_modu'  => $form_state['values']['comodu'],
                'co_degre' => $form_state['values']['codegre'],))
    ->fields(array('LE_etat' => $form_state['values']['LE_etat_total'] * 2,))
    ->execute();
}
/**
 * ----> Submit
 */
function LE_info_form_submit(&$form, &$form_state) {
  db_merge('gbb_gmodu_plus')
    ->key(array('co_modu'  => $form_state['values']['comodu'],
                'co_degre' => $form_state['values']['codegre'],))
    ->fields(array('LE_info' => $form_state['values']['LE_info'],))
    ->execute();
}
/**
 * ----> Submit
 */
function DSF_etat_part_form_submit(&$form, &$form_state) {
  db_merge('gbb_gmodu_plus')
    ->key(array('co_modu'  => $form_state['values']['comodu'],
                'co_degre' => $form_state['values']['codegre'],))
    ->fields(array('DSF_etat' => $form_state['values']['DSF_etat_part'],))
    ->execute();
}
function DSF_etat_total_form_submit(&$form, &$form_state) {
  db_merge('gbb_gmodu_plus')
    ->key(array('co_modu'  => $form_state['values']['comodu'],
                'co_degre' => $form_state['values']['codegre'],))
    ->fields(array('DSF_etat' => $form_state['values']['DSF_etat_total'] *2,))
    ->execute();
}
/**
 * ----> Submit
 */
function DSF_info_form_submit(&$form, &$form_state) {
  db_merge('gbb_gmodu_plus')
    ->key(array('co_modu'  => $form_state['values']['comodu'],
                'co_degre' => $form_state['values']['codegre'],))
    ->fields(array('DSF_info' => $form_state['values']['DSF_info'],))
    ->execute();
}
/**
 * ----> Submit
 */
function DA_date_depot_form_submit(&$form, &$form_state) {
  db_merge('gbb_gmodu_plus')
    ->key(array('co_modu'  => $form_state['values']['comodu'],
                'co_degre' => $form_state['values']['codegre'],))
    ->fields(array('DA_date_depot' => $form_state['values']['DA_date_depot'],))
    ->execute();
}



/**
 * Fonctions d' ** AUTOCOMPLETE AJAX ETABLISSEMENTS **
 */
function _gaiabb_autocompetab($string) {

  $matches = array();
  $result = db_select('gbb_netab', 'c')
    ->fields('c', array('co_lieu', 'denom_comp', 'sigle'))
    ->condition('denom_comp', '%' . db_like($string) . '%', 'LIKE')
    ->range(0, 10)
    ->execute();
  foreach ($result as $r) {
    $etab = $r->denom_comp . ' (' . $r->sigle . ')';
    $matches[$etab] = $etab;
  }
  $count = $result->rowCount();
  if ($count > 9) $matches[' '] = '..... Et bien d\'autres .....';
  drupal_json_output($matches);
}

/**
 * Fonctions d' ** AUTOCOMPLETE AJAX FORMATEURS **
 */
function _gaiabb_autocompform($string) {

  $matches = array();
  $result = db_select('gbb_gresp_dafor', 'c')
    ->fields('c', array('co_resp', 'nomu', 'prenom'))
    ->condition('nomu', '%' . db_like($string) . '%', 'LIKE')
    ->range(0, 10)
    ->execute();
  foreach ($result as $r) {
    $matches[$r->prenom . ' ' . $r->nomu] = $r->prenom . ' ' . $r->nomu;
  }
  $count = $result->rowCount();
  if ($count > 9) $matches[' '] = '..... Et bien d\'autres .....';
  drupal_json_output($matches);
}
