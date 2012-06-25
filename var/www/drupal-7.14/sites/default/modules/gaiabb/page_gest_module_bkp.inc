<?php

/**
 * Fonction d'appel aux differents elements composant la page
 */
function gest_module($args = 'NaN') 
{
  // Charge la feuille des style et les fichiers javascript pour le module
  drupal_add_css(drupal_get_path('module', 'gaiabb') . '/gaiabb.css', array('group' => CSS_SYSTEM -1 , 'preprocess' => FALSE));
  drupal_add_library('system', 'drupal.collapse');

  // Construction de la page
  $build = array();
  $build['infos_gaia']   = infos_gaia($args);
  $build['infos_detail'] = theme('fieldset', array(
                             'element' => array(
                               '#attributes' => array('class'=>array('details','collapsible','collapsed')),
                               '#children' => '',
                               '#title' => t('Détails ...'),
                               '#value' => infos_detail($args),
                               )
                             )
                           );
  $build['organisation'] = render(drupal_get_form('form_organisation',$args));
  $build['attachements'] = render(drupal_get_form('form_attachements',$args));
  $build['sessions']     = render(drupal_get_form('form_sessions',$args));

  $build['form_edit_session'] = render(drupal_get_form('form_edit_session',$args));

  return implode('',$build);
}


/**
 * Un tableau à deux lignes qui affiche le dispo et le module
 */
function infos_gaia($args) 
{

  #if (arg(1) == '' || arg(2)='' ) return "";
  $result = db_query('
       SELECT id_disp, co_modu, m.lib AS libm, ds.lib AS libd, co_camp, co_andi
       FROM {gbb_gmodu} AS m
       JOIN {gbb_gdisp} AS ds ON ds.co_disp = m.co_disp
                             AND ds.co_degre = m.co_degre 
       WHERE m.co_modu  = :comodu
         AND m.co_degre = :codegre
       ', array(':codegre' => arg(1), 
                ':comodu'  => arg(2)) );
  foreach ($result as $r) {
    $paf_ou_fil = ($r->co_camp == 'BS')? 'fil' : 'paf';
    $ouvert_ou_ferme = ($r->co_andi == 'Fe')? 'Ferme' : '';
    $rows[] = array( 
                array('data' => $r->id_disp.' '.$r->libd,
                      'class' => array($paf_ou_fil,$ouvert_ou_ferme),
                      'colspan' => '2',
                )
              );
    $rows[] = array( 
                array('data' => $r->co_modu.' '.$r->libm,
                      'class' => array($ouvert_ou_ferme),
                      'colspan' => '2',
                     )
              );
  }

  if (!isset($rows)) return $args;
  return theme('table', array('rows'=> $rows));
}


/**
 * Un tableau qui affiche les détails du module
 */
function infos_detail($args) 
{

  if ($args == 'NaN') return "";
  $liste = array();
  $result = db_query('
       SELECT lobjt, n.lib_long AS ori, lautre, m.duree_prev, lmoda, lform, r.co_resp, concat(prenom,\' \',nomu) AS resp, co_tres
       FROM {gbb_gmodu} AS m 
       JOIN {gbb_gdisp} AS ds ON ds.co_disp = m.co_disp
                             AND ds.co_degre = m.co_degre 
       LEFT JOIN {gbb_norie} AS n  ON n.co_orie  = ds.co_orie
       LEFT JOIN {gbb_gdire} AS dr ON dr.co_modu = m.co_modu
                             AND dr.co_degre = m.co_degre 
       LEFT JOIN {gbb_gresp} AS r  ON r.co_resp  = dr.co_resp
                             AND r.co_degre = m.co_degre 
       WHERE m.co_modu  = :comodu
         AND m.co_degre = :codegre
       ', array(':codegre' => arg(1),
                ':comodu'  => arg(2)) );

  $TRES = array( 1 => t("Interlocuteur"),
                 2 => t("Responsable organisationnel"),
                 3 => t("Responsable pédagogique")
    );
  foreach ($result as $r) {
    $rows[] = array( 
                array('data' => $TRES[$r->co_tres]),
                array('data' => $r->resp),
              );
  }
    $rows[] = array( 
                array('data' => "Objectif Dispositif"),
                array('data' => $r->lobjt),
              );
    $rows[] = array( 
                array('data' => "Orientation"),
                array('data' => $r->ori),
              );
    $rows[] = array( 
                array('data' => "Observation"),
                array('data' => $r->lautre),
              );
    $rows[] = array( 
                array('data' => t("Durée")),
                array('data' => $r->duree_prev."h"),
              );
    $rows[] = array( 
                array('data' => t("Modalité")),
                array('data' => $r->lmoda),
              );
    $rows[] = array( 
                array('data' => "Forme"),
                array('data' => $r->lform),
              );
  if (!isset($rows)) return $args;
  return theme('table', array('rows'=> $rows));
}


/**
 * Formulaire d'organisation de la formation
 */
function form_organisation($form, &$form_state) 
{
  $result = db_query("
     SELECT organisation,
            module_alert
     FROM {gbb_gmodu_plus}
     WHERE co_modu  = :comodu
       AND co_degre = :codegre
     ", array(':codegre' => arg(1),
              ':comodu'  => arg(2)) );
  foreach ($result as $r) {  }

  $form['f'] = array(
    '#type' => 'fieldset',
    '#title' => t('Organisation'),
    '#attributes' => array('class'=>array('organisation','collapsible')),
  );

  $html = $r->organisation;
  $txt = preg_replace('#<br\s*/?>#i', "\n", $html); //<? #### ASTUCE coloration syntaxique 
  $form['f']['organisation'] = array(
    '#type' => 'textarea',
    '#title' => t('Journal'),
    '#default_value' => $txt,
    '#ajax' => array
    (
      'callback' => 'form_organisation_submit',
      'progress' => array('type'=> 'throbber','message'=>'Enregistrement...'),
    ),
  );
  $image = array(
        'path' => drupal_get_path('module', 'gaiabb') . '/images/module_alert.png',
        'alt' => 'module_alert',
        'title' => t('Alerte entre conseillers'),
        'attributes' => array('class' => 'some-img', 'id' => 'my-img'));

  $form['f']['module_alert'] = array(
    '#type' => 'checkbox',
    '#title' => theme('image', $image).t('Alerte entre conseillers'),
    '#default_value' => $r->module_alert,
    '#ajax' => array
    (
      'callback' => 'form_organisation_submit',
      'progress' => array('type'=> 'throbber','message'=>'Enregistrement...'),
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


function form_organisation_submit(&$form, &$form_state) 
{
  db_update('gbb_gmodu_plus')
    ->fields(array('organisation' => $form_state['values']['organisation'],
                   'module_alert' => $form_state['values']['module_alert'],
             ))
    ->condition('co_modu',  $form_state['values']['comodu'],  '=')
    ->condition('co_degre', $form_state['values']['codegre'], '=')
    ->execute();
}

/**
 * Formulaire des attachements
 */
function form_attachements($form, &$form_state,$args) 
{
  $form['f'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fichiers joints'),
    '#attributes' => array('class'=>array('attachements','collapsible')),
  );

  $files = db_query("
     SELECT fm.fid, filename, uri, filemime, timestamp
     FROM {gbb_file} AS f
     JOIN {file_managed} AS fm ON fm.fid = f.fid
     WHERE co_modu  = :comodu
       AND co_degre = :codegre
       AND status = 1
     ", array(':codegre' => arg(1),
              ':comodu'  => arg(2)) );
  $icon_directory = variable_get('file_icon_directory', drupal_get_path('module', 'file') . '/icons');
  $file_list = array();
  foreach ($files as $f) {
    $icon = theme_file_icon(array('file' => $f, 'icon_directory' =>  $icon_directory));
    $date = strftime('%d/%m/%Y à %X', $f->timestamp);
    $file_list[$f->fid] = $icon.l($f->filename,file_create_url($f->uri),array('attributes'=> array('title' => $date)));
  }

  if ( count($file_list)>0 ) {
    $form['f']['files'] = array(
      '#type' => 'radios',
      '#options' => $file_list,
    );
  $form['f']['supprimer_file'] = array(
    '#type' => 'submit',
    '#value' => t('Supprimer'),
  );
  }

  $form['f']['Myfile'] = array(
    '#title' => t('Joindre un fichier à cette page'),
    '#type'  => 'managed_file',
    '#upload_location' => 'public://',
    '#process' => array('form_attachment_process')
  );

  $form['f']['comodu'] = array(
    '#type' => 'hidden',
    '#value' => arg(2),
  );

  $form['f']['codegre'] = array(
    '#type' => 'hidden',
    '#value' => arg(1),
  );
  $form['f']['add_file'] = array(
    '#type' => 'submit',
    '#value' => t('Joindre'),
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>',
  );
  return $form;
}

function form_attachment_process($element, &$form_state, $form) {
  $element = file_managed_file_process($element, $form_state, $form);
  $element['upload_button']['#access'] = FALSE;
  return $element;
}

function form_attachements_submit($form, &$form_state) 
{

  // TODO : actuellement on écrit dans public:// il faudrait écrire dans le dir ci_dessous :
  $dir = drupal_get_path('module', 'gaiabb') . '/files';

  if( !file_prepare_directory( $dir ) ) {
    drupal_set_message( t('WARNING: Server DIR is not accessible. Consult with site admin!', 'status') );
  } elseif ( isset($form_state['values']['files'])) {
    $file = file_load($form_state['values']['files']);
    $file->status |= 'FILE_STATUS_TEMPORARY';
    $file = file_save($file);
    drupal_set_message( t('Le fichier n°'). $form_state['values']['files'] .t(' a été archivé. En cas de problème ce numéro permet de le récupérer, contactez pour cela l\'administarteur.'),'warning' );
  } elseif ( $form_state['values']['Myfile'] == '' ) {
    drupal_set_message( t('Echec. Veillez à remplir les champs avant de valider une action.'),'warning' );
  } else {
    // Load the file via file.fid.
    $file = file_load($form_state['values']['Myfile']);
    // Change status to permanent.
    $file->status = FILE_STATUS_PERMANENT;
    // Save.
    file_save($file);
    db_insert('gbb_file')
      ->fields(array('co_modu'  => $form_state['values']['comodu'],
                     'co_degre' => $form_state['values']['codegre'],
                     'fid'      => $file->fid))
      ->execute();
    drupal_set_message( t('Le nouveau fichier a été enregistré.') );
  }
  $form_state['redirect'] = "gest_module/" . $form_state['values']['codegre'] .'/'. $form_state['values']['comodu'];
}

/**
 * Formulaire d'affichage de toutes les sessions
 */
function form_sessions($form, $form_state,$args) 
{

  $date_demande = '1970-01-01';
  $sessions = array();
  $result = db_query("
        SELECT sess_id, 
               date,
               date_demande,
               horaires, 
               s.co_lieu, 
               denom_princ, 
               denom_comp, 
               session_alert, 
               groupe, 
               s.co_resp, 
               dureeprevue, 
               dureeapayer, 
               typePaiement, 
               nomu, 
               prenom
        FROM {gbb_session} AS s
        JOIN {gbb_gmodu_plus} AS m ON m.co_modu = s.co_modu
                                  AND m.co_degre = s.co_degre
        JOIN {gbb_gresp_dafor} AS r ON r.co_resp = s.co_resp
                                   AND r.co_degre = s.co_degre
        JOIN {gbb_netab} AS e ON e.co_lieu = s.co_lieu
        WHERE s.co_modu  = :comodu
          AND s.co_degre = :codegre
        ", array(':codegre' => arg(1),
                 ':comodu'  => arg(2)) );
  foreach ($result as $r) {
    setlocale (LC_TIME, 'fr_FR');
    $sessions[] = array('sess_id' => $r->sess_id,
                       'date' => utf8_encode(strftime('%a %e %h %Y ',strtotime($r->date))),
                       'horaires' => $r->horaires,
                       'lieu' => $r->denom_princ.' '.$r->denom_comp,
                       'session_alert' => $r->session_alert,
                       'groupe' => $r->groupe,
                       'formateur' => $r->prenom.' '.$r->nomu,
                       'dureeprevue' => $r->dureeprevue."h",
                       'dureeapayer' => $r->dureeapayer."h",
                       'TypePaiement' => $r->typePaiement,
                      );
    $date_demande = $r->date_demande;
  }

$header = array(
    'date' => t('Date'),
    'horaires' => t('Horaires'),
    'lieu' => t('Lieu'),
    'session_alert' => t('!'),
    'groupe' => t('Gr'),
    'formateur' => t('Formateur'),
    'dureeprevue' => t('Prévues'),
    'dureeapayer' => t('À payer'),
    'TypePaiement' => t('Type paiemt'),
  );
  $options = array();
  foreach($sessions as $session)
  {
    $options[$session['sess_id']] =array
    (
      'date' => $session['date'],
      'horaires' => $session['horaires'],
      'lieu' => $session['lieu'],
      'session_alert' => $session['session_alert'],
      'groupe' => $session['groupe'],
      'formateur' => $session['formateur'],
      'dureeprevue' => $session['dureeprevue'],
      'dureeapayer' => $session['dureeapayer'],
      'TypePaiement' => $session['TypePaiement'],
    );
  }

  $default = (isset($_GET['sess_id']))? $_GET['sess_id'] : 0;

  $form['f'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sessions'),
    '#attributes' => array('class'=>array('sessions','collapsible')),
  );

  setlocale (LC_TIME, 'fr_FR');
  $form['f']['date_d'] = array(
    '#type' => 'fieldset',
    '#title' => t('Dernière modifcation : '.utf8_encode(strftime('%A %e %B %Y ',strtotime($date_demande) ))),
    '#attributes' => array('class'=>array('sessions','collapsible','collapsed')),
  );
  drupal_add_library('system','ui.datepicker');
  drupal_add_js('jQuery(document).ready(function(){jQuery( ".pickadate" ).datepicker({
      dateFormat: "yy-mm-dd",
      autoSize: true,
      showButtonPanel: true,
      firstDay: 1,
      closeText: "Fermer",
      currentText: "Aujourd\'hui",
      numberOfMonths: 3,
      dayNamesMin: ["Di", "Lu", "Ma", "Me", "Je", "Ve", "Sa"],
      monthNames: ["Janvier","Fevrier","Mars","Avril","Mai","Juin","Juillet","Aout","Sepembre","Octobre","Novembre","Decembre"]
    });});', 'inline'); 

  $form['f']['date_d']['date_demande'] = array(
    '#type' => 'textfield',
    '#title' => t('Date demande'),
    '#default_value' => $date_demande,
    '#size' => 10,
    '#maxlength' => 10,
    '#attributes' => array('class' => array('pickadate')),
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>',
  );

  $form['f']['table'] = array
  (
    '#type' => 'tableselect',
    '#header' => $header,
    '#multiple' => FALSE,
    '#options' => $options,
    '#default_value' => (isset($_GET['sess_id']))? $_GET['sess_id'] : 0,
    '#empty' => t('No users found'),
  );
  $form['f']['comodu'] = array(
    '#type' => 'hidden',
    '#value' => arg(2),
  );
  $form['f']['codegre'] = array(
    '#type' => 'hidden',
    '#value' => arg(1),
  );
  $form['f']['modifier'] = array
  (
    '#type' => 'submit',
    '#value' => t('Modifier'),
    '#submit' => array('form_sessions_modifier_submit'),
  );
  $form['f']['supprimer'] = array
  (
    '#type' => 'submit',
    '#value' => t('Supprimer'),
    '#submit' => array('form_sessions_supprimer_submit'),
  );
  $form['f']['ajouter'] = array
  (
    '#type' => 'submit',
    '#value' => t('Ajouter'),
    '#submit' => array('form_sessions_ajouter_submit'),
  );
  return $form;
}

function form_sessions_supprimer_submit($form, &$form_state) 
{
  db_delete('gbb_session')
      ->condition('sess_id', $form_state['values']['table'])
      ->execute();
  drupal_set_message(t('La session a été supprimée.'));
  $url = url('gest_module/' . $form_state['values']['codegre'].
                        '/' . $form_state['values']['comodu'],array('absolute' => TRUE));
  $form_state['redirect'] = $url;
}

function form_sessions_ajouter_submit($form, &$form_state) 
{
  db_insert('gbb_session')
    ->fields(array(
        'session_alert' => 0,
        'co_resp' => 1,
        'co_modu' => arg(2),
        'co_degre' => arg(1),
        'co_lieu' => 0,
        'date' => date('Y-m-d'),
        'groupe' => 1,
        'horaires' => 0,
        'typePaiement' => 'CONV',
        'dureeapayer' => 0,
        'dureeprevue' => 0,
        ))
    ->execute();
  $sessid = db_query("SELECT MAX(sess_id) as sessid FROM {gbb_session}")->fetchField();
  $url = url('gest_module/' . $form_state['values']['codegre'].
                        '/' . $form_state['values']['comodu'],
           array('fragment' => 'edit_session_anchor',
                    'query' => array('sess_id'=>$sessid),'absolute' => TRUE));
  $form_state['redirect'] = $url;
}

function form_sessions_modifier_submit($form, &$form_state) 
{
  $url = url('gest_module/' . $form_state['values']['codegre'].
                        '/' . $form_state['values']['comodu'],
           array('fragment' => 'edit_session_anchor',
                    'query' => array('sess_id'=>$form_state['values']['table']),'absolute' => TRUE));
  $form_state['redirect'] = $url;
}

/**
 * Formulaire de modification d'une session particulière
 */
function form_edit_session($form, $form_state,$args) 
{
  if (!isset($_GET['sess_id'])) return '';
  // On récupère les valeurs associées à la session pour peupler le formulaire
  $result = db_query("
        SELECT * 
        FROM {gbb_session} 
        LEFT JOIN {gbb_gresp_dafor} on {gbb_gresp_dafor}.co_resp = {gbb_session}.co_resp
        LEFT JOIN {gbb_netab} on {gbb_netab}.co_lieu = {gbb_session}.co_lieu
        WHERE sess_id = :sessid", array(':sessid' => $_GET['sess_id']));
   foreach ($result as $r) {
  }

  $form['f'] = array(
    '#type' => 'fieldset',
    '#title' => t('Editer/Ajouter la session ').$r->sess_id.'<a name="edit_session_anchor"></a>' ,
  );

  $form['f']['date'] = array(
    '#type' => 'textfield',
    '#title' => 'Date',
    '#default_value' => $r->date,
    '#size' => 10,
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>&nbsp;',
    '#attributes' => array('class' => array('pickadate')),
  );
  $form['f']['horaires'] = array(
    '#type' => 'textfield',
    '#title' => 'Horaires',
    '#default_value' => $r->horaires,
    '#size' => 20,
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>&nbsp;',
  );
  $form['f']['dureeprevue'] = array(
    '#type' => 'textfield',
    '#title' => t('Durée prévue'),
    '#default_value' => $r->dureeprevue,
    '#size' => 10,
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>&nbsp;',
  );

  $form['f']['dureeapayer'] = array(
    '#type' => 'textfield',
    '#title' => t('Duree à payer'),
    '#default_value' => $r->dureeapayer,
    '#size' => 10,
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>&nbsp;',
  );

  $form['f']['typePaiement'] = array(
    '#type' => 'textfield',
    '#title' => t('Type de paiement'),
    '#default_value' => $r->typePaiement,
    '#size' => 10,
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>',
  );

  $form['f']['lieu'] = array(
    '#type' => 'textfield',
    '#title' => t('Lieu'),
    '#default_value' => $r->denom_comp,
    '#size' => 40,
    '#autocomplete_path' => 'autocomp/etablissement',
    '#prefix' => '<br/><div class="inline">',
    '#suffix' => '</div>&nbsp;',
  );

  $form['f']['formateur'] = array(
    '#type' => 'textfield',
    '#title' => t('Formateur'),
    '#default_value' => $r->prenom.' '.$r->nomu,
    '#size' => 30,
    '#autocomplete_path' => 'autocomp/formateur',
    '#prefix' => '<div class="inline">',
    '#suffix' => '</div>',
  );


  $form['f']['co_degre'] = array(
    '#type' => 'hidden',
    '#value' => arg(1),
  );
  $form['f']['co_modu'] = array(
    '#type' => 'hidden',
    '#value' => arg(2),
  );
  $form['f']['sess_id'] = array(
    '#type' => 'hidden',
    '#value' => $r->sess_id,
  );
  $form['f']['submit'] = array
  (
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

function form_edit_session_submit($form, &$form_state) 
{
  $valeurs = array(':sess_id' => $form_state['values']['sess_id'],
                   ':co_modu' => $form_state['values']['co_modu'], 
                   ':date' => $form_state['values']['date'], 
                   ':horaires' => $form_state['values']['horaires'],
                   ':dureeprevue' => $form_state['values']['dureeprevue'], 
                   ':dureeapayer' => $form_state['values']['dureeapayer'], 
                   ':typePaiement' => $form_state['values']['typePaiement'], 
                   ':lieu' => $form_state['values']['lieu'], 
                   ':formateur' => $form_state['values']['formateur'],
  ); 
  db_query("UPDATE {gbb_session} SET date = :date,
                                   co_modu = :co_modu,
                                   horaires = :horaires,
                                   dureeprevue = :dureeprevue,
                                   dureeapayer = :dureeapayer,
                                   typePaiement = :typePaiement,
                                   co_lieu = (SELECT co_lieu FROM {gbb_netab} WHERE denom_comp = :lieu),
                                   co_resp = (SELECT co_resp FROM {gbb_gresp_dafor} WHERE CONCAT(prenom,' ',nomu) = :formateur)
            WHERE sess_id = :sess_id", $valeurs);
  /*
  $subquery1 = db_select('gbb_netab', 'c')
    ->fields('c', array('co_lieu'))
    ->condition('denom_comp', $form_state['values']['lieu'], '=')
    ->execute();

  $subquery2 = db_select('gbb_gresp_dafor', 'r')
    ->fields('r', array('co_resp'))
    ->condition('CONCAT\(prenom,\' \',nomu\)', $form_state['values']['formateur'], '=')
    ->execute();

  db_update('gbb_session','s')
    ->fields(array('date' => $form_state['values']['date'],
                   'co_modu' => $form_state['values']['co_modu'],
                   'date' => $form_state['values']['date'],
                   'horaires' => $form_state['values']['horaires'],
                   'dureeprevue' => $form_state['values']['dureeprevue'],
                   'dureeapayer' => $form_state['values']['dureeapayer'],
                   'typePaiement' => $form_state['values']['typePaiement'],
                   'co_lieu' => $subquery1,
                   'co_resp' => $subquery2,
             ))
    ->condition('sess_id',  $form_state['values']['sess_id'],  '=')
    ->execute();
  */

  $url = url('gest_module/' . $form_state['values']['co_degre'].
                        '/' . $form_state['values']['co_modu'],array('query' => array('sess_id' => $form_state['values']['sess_id']),'absolute' => TRUE));
  drupal_set_message(t('Les modifications ont &eacute;t&eacute; prises en compte.'));
  $form_state['redirect'] = $url;
}

/**
 * Fonctions d'autoclompletion AJAX
 */
function _gaiabb_autocompetab($string) 
{
  $matches = array();
  $result = db_select('gbb_netab', 'c')
    ->fields('c', array('co_lieu','denom_comp'))
    ->condition('denom_comp', '%' . db_like($string) . '%', 'LIKE')
    ->range(0,10)
    ->execute();
  foreach ($result as $r) {
    $matches[$r->denom_comp] = $r->denom_comp;
  }
  $count = $result->rowCount();
  if ($count > 9) $matches[' '] = '..... Et bien d\'autres .....';
  drupal_json_output($matches);
}

function _gaiabb_autocompform($string) 
{
  $matches = array();
  $result = db_select('gbb_gresp_dafor', 'c')
    ->fields('c', array('co_resp','nomu','prenom'))
    ->condition('nomu', '%' . db_like($string) . '%', 'LIKE')
    ->range(0,10)
    ->execute();
  foreach ($result as $r) {
    $matches[$r->prenom.' '.$r->nomu] = $r->prenom.' '.$r->nomu;
  }
  $count = $result->rowCount();
  if ($count > 9) $matches[' '] = '..... Et bien d\'autres .....';
  drupal_json_output($matches);
}
