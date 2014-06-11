<?php
/**
 * @file
 *
 * Page d'affichage du plan
 *
 */

/**
 * Fonction d'appel aux differents elements composant la page
 */
function gbb_pia_view($args = 'NaN') {
  // Charge la feuille des style et les fichiers javascript pour le module
  drupal_add_css(drupal_get_path('module', 'gbb') . '/css/gbb.css',
                 array('group' => CSS_SYSTEM -1 , 'preprocess' => FALSE));
  drupal_add_library('system', 'drupal.collapse');

  // Construction de la page
  $build = array();

  $ledispo = arg(3);
  setlocale(LC_CTYPE, 'fr_FR.ISO-8859-1');
  if (is_numeric(arg(2)) && !preg_match('/^14/',$ledispo)) {
    $build['tab'] = gbb_pia_stages(1,arg(2));
  } elseif (is_numeric(arg(1)) && arg(2) != "" && !is_numeric(arg(2))) {
    $build['tab'] = gbb_pia_stages(4,arg(1),arg(2));
  } elseif (is_numeric(arg(2)) && preg_match('/^14/',$ledispo)) {
    $build['tab'] = gbb_pia_stages(3,arg(2),$ledispo);
  } elseif (isset($_GET['kw']) && preg_match('#[[:alpha:]]#', $_GET['kw'])) {
    $build['tab'] = gbb_pia_stages(2,$_GET['kw']);
  } else {
    switch (arg(1)) {
      case "Axe1":
        $build['tab'] = gbb_pia_vocab(2,'Second degré');
        break;
      case 2:
        $build['tab'] = gbb_pia_vocab(2,'Second degré');
        break;
      case 3:
        $build['tab'] = gbb_pia_vocab(3,'IATOSS');
        break;
      case 5:
        $build['tab'] = gbb_pia_vocab(5,'Premier degré');
        break;
      case 6:
        $build['tab'] = gbb_pia_vocab(6,'Encadrement');
        break;
      case 7:
        $build['tab'] = gbb_pia_vocab(7,'Interdegré');
        break;
      case "rech":
        $build['search'] = render(drupal_get_form('gbb_pia_search_form',$args));
        break;
    };
  };
  return implode('', $build);
}

/**
 * désactive drupal js en conflit avec le système de toggle utilisé ici
 */
function gbb_js_alter(&$js) {
  if (arg(0) == "pia14") {
    unset( $js['misc/drupal.js']);
  }
}

/**
 * Formulaire de recherche d'un stage 
 */
function gbb_pia_search_form($form, &$form_state, $args) {
  $form['f'] = array(
    '#type' => 'fieldset',
    '#title' => t('Rechercher un STAGE'),
    '#attributes' => array('class' => array('collapsible',)),
  );
  $form['f']['kw'] = array(
    '#type' => 'textfield',
    '#description' => t('par ex. : Agrégation, lettres ou théâtre...'),
    '#size' => 50, 
  );

  $options = array();
  $options["1"] = t('1er degré&nbsp;&nbsp;&nbsp;&nbsp;');
  $options["2"] = t('Interdegré&nbsp;&nbsp;&nbsp;&nbsp;');
  $options["3"] = t('2nd degré <br/>');
  $options["4"] = t('IATSS&nbsp;&nbsp;&nbsp;&nbsp;');
  $options["5"] = t('Encadrement&nbsp;&nbsp;&nbsp;&nbsp;');
  $options["6"] = t('Intercatégoriel');

  $form['f']['boxes'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => array("1", "2", "3", "4", "5", "6"),
    '#title' => t('Dans quel plan ?'),
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  $form['f']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Chercher',
    '#submit' => array('gbb_pia_search_form_submit'),
  );
  $form['f']['#action'] = url('pia14', array('absolute' => TRUE));

  return $form;
}

function gbb_pia_search_form_validate($form, &$form_state) {
  if (!preg_match('#[[:alpha:]]#', $_POST['kw'])) {
    form_set_error('ttt',t("Merci de reformuler votre requête."));
    return FALSE;
  } else {
    return TRUE;
  }
}
function gbb_pia_search_form_submit($form, &$form_state) {
  $form_state['redirect'] = array('pia14',
                    array('query' => array('kw' => $_POST['kw'], 'opt' => $_POST['boxes'])),
                                 );
}


/**
 * Affiche les stages, au choix
 * - pour une taxonomie (type=1, arg=tid) 
 * - pour un mot clé (type=2, arg=keyword)
 * - pour un axe d'un tid (type=4, arg=tid/axe)
 *
 * récupération du classement dans l'offre vers le classement dans le plan.
 */
function gbb_pia_stages($type,$arg,$id=NULL) {

    $query = db_select('taxonomy_term_data', 'ttd');
    $query->condition('tid', arg(2));
    $query->fields('ttd', array('name'));
    $names = $query->distinct()->execute()->fetchCol();
    $nb = $query->countQuery()->execute()->fetchField();
    
    $html = ($nb >0)? "<h1><a href=\"/gbbdr/pia14/".arg(1)."/".arg(2)."\">" . $names[0] . "</a></h1>" : "<h1>Résultat(s)</h1>";
    if (arg(2)==113) $html .= "<p>Les enseignants du <strong>premier degré</strong> désirant s'inscrire à un des stages interdegré ci-dessous doivent choisir le dispositif correspondant dont le code commence par <strong>14D</strong>.</p>";
    if (arg(2)==113) $html .= "<p>Les enseignants du <strong>second degré</strong> désirant s'inscrire à un des stages interdegré ci-dessous doivent choisir le dispositif correspondant dont le code commence par <strong>14A</strong>.</p>";
  $query = db_select('gbb_gmodu_taxonomy', 'gt');
  $query->condition('gt.weight', 500, '<')
        ->condition('gd.co_andi' , '', '=')
        ->condition('gm.co_anmo' , '', '=');
  switch ($type) {
    case 1:
      $query->condition('gt.tid', $arg);
      break;
    case 2:  # recherche
      $query->condition(
           db_or()->condition('gd.lib',   '%' . db_like($arg) . '%', 'LIKE')
                  ->condition('gd.lobjt', '%' . db_like($arg) . '%', 'LIKE')
                  ->condition('gm.lib',   '%' . db_like($arg) . '%', 'LIKE')
                  ->condition('gm.lpeda', '%' . db_like($arg) . '%', 'LIKE')
                  ->condition('gm.lcont', '%' . db_like($arg) . '%', 'LIKE')
      );
      $plan = db_or()->condition('gm.lib', 'PAS DE STAGE', 'LIKE');
      if ( isset($_GET['opt'][1]) ) { $plan->condition('gd.co_tpla', 'P', 'LIKE');};
      if ( isset($_GET['opt'][2]) ) { $plan->condition('gd.co_tpla', 'D', 'LIKE');};
      if ( isset($_GET['opt'][3]) ) { $plan->condition('gd.co_tpla', 'S', 'LIKE');};
      if ( isset($_GET['opt'][4]) ) { $plan->condition('gd.co_tpla', 'A', 'LIKE');};
      if ( isset($_GET['opt'][5]) ) { $plan->condition('gd.co_tpla', 'E', 'LIKE');};
      if ( isset($_GET['opt'][6]) ) { $plan->condition('gd.co_tpla', 'C', 'LIKE');};
      $query->condition( $plan );
      $query->orderBy('gd.co_orie', 'ASC');
      break;
    case 3:
      $query->condition('gt.tid', $arg);
      $query->condition('id_disp', $id);
      break;
    case 4:
      $query->condition('ttd.vid', $arg);
      $query->condition('co_prac', db_like($id), 'LIKE');
      break;
  }
  $query->condition('id_disp', db_like('14') . '%', 'LIKE');
  $query->join('taxonomy_term_data', 'ttd', 'ttd.tid = gt.tid');
  $query->join('gbb_gmodu', 'gm', 'gt.co_modu = gm.co_modu');
  $query->join('gbb_gdisp', 'gd', 'gd.co_disp = gm.co_disp');
  $query->join('gbb_norie', 'go', 'go.co_orie = gd.co_orie');
  $query->fields('ttd', array('vid'));
  $query->fields('gt', array('co_modu'));
  $query->fields('gm', array('lib','lcont','lmoda','lpeda','lform','duree_prev','co_degre','lcibl','co_prac'));
  $query->fields('gd', array('id_disp','lobjt','co_tcan', 'co_orie'));
  $query->addField('gd', 'lib', 'lib_dispo');
  $query->addField('go', 'lib_long', 'lib_orie');
  if ($type!=4) {
    $query->orderBy('gd.co_tcan', 'ASC');
    $query->orderBy('gt.weight', 'ASC');
  }
  $res = $query->distinct()->execute()->fetchAll();
  $nb = $query->countQuery()->execute()->fetchField();
  if ($type==2) $html = "<h1>$nb Résultat(s)</h1>";
  #print($query);
  foreach ($res as $stage) {

    $query = db_select('gbb_gmodu_taxonomy', 'gt');
    $query->join('taxonomy_term_data', 'ttd', 'gt.tid=ttd.tid');
    $query->condition('co_modu', $stage->co_modu);
    $query->fields('ttd', array('vid'));
    $cats = $query->distinct()->execute()->fetchCol();
    $catss = "<span class=\"ul-custom-class".implode("\">&nbsp;&nbsp;</span> <span class=\"ul-custom-class",$cats)."\">&nbsp;&nbsp;</span>";
    $catss = "&nbsp;";

    
    $query = db_select('gbb_gdire', 'gdr');
    $query->condition('gdr.co_modu', $stage->co_modu, "=");
    $query->condition('gdr.co_degre', $stage->co_degre, "=");
    $query->condition('gdr.co_tres', 3, "=");
    $query->join('gbb_gresp', 'gr', 'gr.co_resp = gdr.co_resp
                              AND gr.co_degre = gdr.co_degre');
    $query->fields('gr', array('nomu','prenom'));
    $resps = $query->distinct()->execute()->fetchAll();
    $nb = $query->countQuery()->execute()->fetchField();
    $resp = ($nb >0)? $resps[0]->prenom." ".$resps[0]->nomu : "?";


    $dispo[$stage->id_disp][] = array('co_modu' => $stage->co_modu,
                                      'id_disp' => $stage->id_disp,
                                      'lib_dispo' => $stage->lib_dispo,
                                      'lcibl' => $stage->lcibl,
                                      'lobjt' => $stage->lobjt,
                                      'lcont' => $stage->lcont,
                                      'lpeda' => $stage->lpeda,
                                      'lmoda' => $stage->lmoda,
                                      'lform' => $stage->lform,
                                      'duree' => $stage->duree_prev,
                                      'co_prac' => $stage->co_prac,
                                      'co_tcan' => $stage->co_tcan,
                                      'co_orie' => $stage->co_orie,
                                      'lib_orie' => $stage->lib_orie,
                                      #'resppeda' => $stage->prenom . " " . $stage->nomu,
                                      'resppeda' => $resp,
                                      'categories' => $catss,
                                      'lib' => $stage->lib);
  };
    
    $axe=array(  "TA" => "<img src=\"/gbbdr/sites/all/themes/sky/axe1-bloc.png\" title=\"Numérique\">",
               "TB" => "<img src=\"/gbbdr/sites/all/themes/sky/axe2-bloc.png\" title=\"Culture\" >",
               "TC" => "<img src=\"/gbbdr/sites/all/themes/sky/axe3-bloc.png\" title=\"International et LVE\" >",
               "TD" => "<img src=\"/gbbdr/sites/all/themes/sky/axe4-bloc.png\" title=\"Parcours scolaires\" >",
               "TE" => "<img src=\"/gbbdr/sites/all/themes/sky/axe5-bloc.png\" title=\"Climat scolaire\" >",
               "" => "",
             );
  if (isset($dispo)) {
    if ($type == 4) {
      $orie ="<div>";
      $html = "<h1>" . $axe[arg(2)] . "</h1>";
    }
    foreach ($dispo as $k=>$d) {
      if ($type == 4 && $orie != $d[0]['co_orie']) {
        $html .= "</div><h4><a href=\"#\" class=\"image\" type=\"image\" onclick=\"$('#".$d[0]['co_orie']."').toggle('fast');\" alt=\"info\" src=\"/gbbdr/sites/all/themes/sky/info.png\" name=\"info\">";
        $html .= "<span style=\"background:#4D453E;color: #FFF;\">&nbsp;+&nbsp;</span>&nbsp;";
        $html .= $d[0]['lib_orie'] . "</a></h4>";
        $html .= "<div id=\"".$d[0]['co_orie']."\" style=\"display: none;\">";
      }
      $html .= gbb_pia_aff_dispo($k, $d);
      $orie = $d[0]['co_orie'];
    }
    if ($type == 4) {$orie ="</div>";}

  } else {
    $html = " Aucun stage ne correspond à votre recherche.";
  };
  return $html;
}

/**
 * Crée la liste des catégories
 */
function gbb_pia_vocab($vid,$title) {

  $terms = taxonomy_get_tree($vid);
  foreach ($terms as $term) {
    $query = db_select('gbb_gmodu_taxonomy', 'gt');
    $query->condition('tid', $term->tid);
    $query->fields('gt', array('co_modu'));
    $nb = $query->countQuery()->execute()->fetchField();
    $str = $term->name;
    if ($nb > 0) $str = l($str,"pia14/$vid/$term->tid");
    $str = str_repeat(' &nbsp;&nbsp;&nbsp; ', $term->depth) . $str;
    $items[] = array('data'=>$str);
  };
  $type = 'ul';
  $attributes = array(
     'id' => 'my-custom-listing'.$vid,
     'class' => 'ul-custom-class'.$vid.' another-custom-class', // a string or indexed
   );
  $title="";
  $output = theme_item_list(array('items' => $items, 'title' => $title, 'type' => $type, 'attributes' => $attributes));
  $axes = "<h1>Axes du projet académique</h1>"; 
  $axes .= "<a href=\"/gbbdr/pia14/".arg(1)."/TA\"><img src=\"/gbbdr/sites/all/themes/sky/axe1-bloc.png\"></a>&nbsp;&nbsp;&nbsp;";
  $axes .= "<a href=\"/gbbdr/pia14/".arg(1)."/TB\"><img src=\"/gbbdr/sites/all/themes/sky/axe2-bloc.png\"></a>&nbsp;&nbsp;&nbsp;";
  $axes .= "<a href=\"/gbbdr/pia14/".arg(1)."/TC\"><img src=\"/gbbdr/sites/all/themes/sky/axe3-bloc.png\"></a>&nbsp;&nbsp;&nbsp;";
  $axes .= "<a href=\"/gbbdr/pia14/".arg(1)."/TD\"><img src=\"/gbbdr/sites/all/themes/sky/axe4-bloc.png\"></a>&nbsp;&nbsp;&nbsp;";
  $axes .= "<a href=\"/gbbdr/pia14/".arg(1)."/TE\"><img src=\"/gbbdr/sites/all/themes/sky/axe5-bloc.png\"></a>";
  $axes .= "<h1>Domaines et disciplines</h1>"; 
  return $axes.$output;
}

function gbb_pia_aff_dispo($k, $d) {
  ## Les icônes permettant d'identifier les stages
  $tcan=array( 1 => "",      // Initaiative indivieduelle
               2 => "",      // FIL,
               3 => "<img src=\"/gbbdr/sites/all/themes/sky/pubDes.png\" alt=\"pubDes\" title=\"public désigné\">&nbsp;"
             );
  $axe=array(  "TA" => "<img src=\"/gbbdr/sites/all/themes/sky/axe1-16.png\" title=\"Numérique\">",
               "TB" => "<img src=\"/gbbdr/sites/all/themes/sky/axe2-16.png\" title=\"Culture\" >",
               "TC" => "<img src=\"/gbbdr/sites/all/themes/sky/axe3-16.png\" title=\"International et LVE\" >",
               "TD" => "<img src=\"/gbbdr/sites/all/themes/sky/axe4-16.png\" title=\"Parcours scolaires\" >",
               "TE" => "<img src=\"/gbbdr/sites/all/themes/sky/axe5-16.png\" title=\"Cadre Serein\" >",
               "" => "",
             );
  $dclass = (preg_match("/^14D/", $k))? "iddispD" : "iddispA";
  $mclass = (preg_match("/^14D/", $k))? "comoduD" : "comoduA";

  $html = "<p><span class=\"$dclass\">$k</span>";
  $html .= " <input class=\"image\" type=\"image\" onclick=\"$('#$k').toggle('slow');\" alt=\"info\" src=\"/gbbdr/sites/all/themes/sky/info.png\" name=\"info\">";
  $html .= "&nbsp;" . $d[0]['lib_dispo'] . "&nbsp;";
  $html .= "" . $tcan[$d[0]['co_tcan']];
  $html .= "" . $axe[$d[0]['co_prac']] ;
  #$html .= "" . $d[0]['lib_orie'] ;
  if (count($d) > 1) {
    $html .= "<div id=\"$k\" style=\"display: none;\">";
    $html .= "<span class=\"titre\">Objet : </span>" . $d[0]['lobjt'] . "<br/>";
    $html .= "</div>";
    foreach ($d as $m) {
      $html .= gbb_pia_aff_details_module($m, $mclass, TRUE);
    }
  } else {
    $html .= "<div id=\"$k\" style=\"display: none;\">";
    $html .= gbb_pia_aff_details_module($d[0], $mclass, FALSE);
    $html .= "</div>";
  }
  $html .= "</p>";
  return $html;
}

function gbb_pia_aff_details_module($m, $mclass, $is_hidden) {
      $html  = "<div class=\"liste\">";
      $html .= "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
      $html .= "<span class=\"$mclass\">" . $m['co_modu'] . "</span>";
      if ($is_hidden) {
        $html .= " <input class=\"image\" type=\"image\" onclick=\"$('#".$m['co_modu']."').toggle('fast');\" alt=\"info\" src=\"/gbbdr/sites/all/themes/sky/info.png\" name=\"info\">";
      }
      $html .= "&nbsp;" . $m['lib'];
      $html .= " ".$m['categories'];
      if ($is_hidden) {
        $html .= "<div id=\"".$m['co_modu']."\" style=\"display: none;\">";
      } else {
        $html .= "<div id=\"".$m['co_modu']."\">";
      }

      $html .= "<span class=\"titre\">Objectif : </span>" . $m['lpeda'] . "<br/>";
      $html .= "<span class=\"titre\">Contenu :  </span>" . $m['lcont'] . "<br/>";
      $html .= "<span class=\"titre\">Modalité : </span>" . $m['lmoda'] . "<br/>";
      $html .= "<span class=\"titre\">Public :   </span>" . $m['lcibl'] . "<br/>";
      $html .= "<span class=\"titre\">Durée :    </span>" . $m['duree'] . "h<br/>";
      $html .= "<span class=\"titre\">Forme :    </span>" . $m['lform'] . "<br/>";
      $html .= "<span class=\"titre\">Responsable pédagogique : </span>" . $m['resppeda'] . "<br/>";
      $html .= "</div>";
      $html .= "</div>";
  return $html;
}
