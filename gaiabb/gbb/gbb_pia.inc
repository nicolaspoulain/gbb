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
  if (is_numeric(arg(2)) && !preg_match('/^13/',$ledispo)) {
    $build['tab'] = gbb_pia_stages(1,arg(2));
  } elseif (is_numeric(arg(2)) && preg_match('/^13/',$ledispo)) {
    $build['tab'] = gbb_pia_stages(3,arg(2),$ledispo);
  } elseif (isset($_GET['kw']) && preg_match('#[[:alpha:]]#', $_GET['kw'])) {
    $build['tab'] = gbb_pia_stages(2,$_GET['kw']);
  } else {
    //$build['tab'] = gbb_pia_categories($args);
    switch (arg(1)) {
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
    #'#element_validate' => array('element_validate_integer_positive'),
    #'#prefix' => '<div class="ajaxform inline">',
    #'#suffix' => '</div>',
    '#size' => 50, 
  );
  $form['f']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Chercher',
    '#submit' => array('gbb_pia_search_form_submit'),
  );
  $form['f']['#action'] = url('pia', array('absolute' => TRUE));

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
  $form_state['redirect'] = array('pia',
                    array('query' => array('kw' => $_POST['kw'])),
                                 );
}


/**
 * Affiche les stages, au choix
 * - pour une taxonomie (type=1, arg=tid) 
 * - pour un mot clé (type=2, arg=keyword)
 *
 * récupération du classement dans l'offre vers le classement dans le plan.
 *
 *  insert into gbb_gmodu_taxonomy (co_modu,tid,weight)
 *   select co_modu,tid, weight
 *     from gbb_gdisp as d
 *     left join gbb_gmodu as g on g.co_disp=d.co_disp 
 *     left join gbb_gmoof_taxonomy as gt on gt.co_omodu=g.co_omodu  
 *     WHERE weight <500 AND id_disp like '13%'
 */
function gbb_pia_stages($type,$arg,$id=NULL) {

    $query = db_select('taxonomy_term_data', 'ttd');
    $query->condition('tid', arg(2));
    $query->fields('ttd', array('name'));
    $names = $query->distinct()->execute()->fetchCol();
    $nb = $query->countQuery()->execute()->fetchField();
    $html = ($nb >0)? "<h1><a href=\"/gbbdr/pia/".arg(1)."/".arg(2)."\">" . $names[0] . "</a></h1>" : "<h1>Résultat(s)</h1>";

  $query = db_select('gbb_gmodu_taxonomy', 'gt');
  $query->condition('weight', 500, '<');
  switch ($type) {
    case 1:
      $query->condition('tid', $arg);
      break;
    case 2:
      $query->condition(
           db_or()->condition('gd.lib',   '%' . db_like($arg) . '%', 'LIKE')
                  ->condition('gd.lobjt', '%' . db_like($arg) . '%', 'LIKE')
                  ->condition('gm.lib',   '%' . db_like($arg) . '%', 'LIKE')
                  ->condition('gm.lpeda', '%' . db_like($arg) . '%', 'LIKE')
                  ->condition('gm.lcont', '%' . db_like($arg) . '%', 'LIKE')
      );
      break;
    case 3:
      $query->condition('tid', $arg);
      $query->condition('id_disp', $id);
      break;
  }
  $query->condition('id_disp', db_like('13') . '%', 'LIKE');
  $query->join('gbb_gmodu', 'gm', 'gt.co_modu = gm.co_modu');
  $query->join('gbb_gdisp', 'gd', 'gd.co_disp = gm.co_disp');
  #$query->join('gbb_gdire', 'gdr','gdr.co_modu = gm.co_modu
  #                             AND gdr.co_tres = 3 
  #                             AND gdr.co_degre = gm.co_degre');
  #$query->join('gbb_gresp', 'gr', 'gr.co_resp = gdr.co_resp
  #                             AND gr.co_degre = gdr.co_degre');
  #$query->fields('gr', array('prenom', 'nomu'));
  $query->fields('gt', array('co_modu'));
  $query->fields('gm', array('lib','lcont','lmoda','lpeda','lform','duree_prev','co_degre','lcibl'));
  $query->fields('gd', array('id_disp','lobjt','co_tcan'));
  $query->addField('gd', 'lib', 'lib_dispo');
  $query->orderBy('weight', 'ASC');
  $res = $query->distinct()->execute()->fetchAll();
  #print($query);
  foreach ($res as $stage) {

    $query = db_select('gbb_gmodu_taxonomy', 'gt');
    $query->join('taxonomy_term_data', 'ttd', 'gt.tid=ttd.tid');
    $query->condition('co_modu', $stage->co_modu);
    $query->fields('ttd', array('vid'));
    $cats = $query->distinct()->execute()->fetchCol();
    $catss = "<span class=\"ul-custom-class".implode("\">&nbsp;&nbsp;</span> <span class=\"ul-custom-class",$cats)."\">&nbsp;&nbsp;</span>";

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
                                      'co_tcan' => $stage->co_tcan,
                                      #'resppeda' => $stage->prenom . " " . $stage->nomu,
                                      'resppeda' => $resp,
                                      'categories' => $catss,
                                      'lib' => $stage->lib);
  };
  //$html = "<p class=\"retour\"><a href=\"pia\">Retour</a></p> ";
  $tcan=array( 1 => "",      // Initaiative indivieduelle
               2 => "",      // FIL,
               3 => "<img src=\"http://www.ent-paris.fr/gbbdr/sites/all/themes/sky/pubDes.png\">"
             );
  #print "<pre>";
  #print_r($dispo);
  #print "</pre>";
  if (isset($dispo)) {
  foreach ($dispo as $k=>$d) {
    $dclass = (preg_match("/^13D/", $k))? "iddispD" : "iddispA";
    $mclass = (preg_match("/^13D/", $k))? "comoduD" : "comoduA";
    if (count($d) == 1) {
      $html .= "<p><span class=\"$dclass\">$k</span>";
      $html .= " <input class=\"image\" type=\"image\" onclick=\"$('#$k').toggle('fast');\" alt=\"A\" src=\"/ent1112b/pub/skins/dafor/image/magnifier.png\" name=\"A\">";
      $html .= "<a href=\"/gbbdr/".current_path()."/".$k."\">" . $d[0]['lib_dispo'];
      $html .= " " . $tcan[$d[0]['co_tcan']];
      $html .= " ".$d[0]['categories'] . "</a>";
      if ($type==3) {
        $html .= "<div id=\"$k\">";
      } else {
        $html .= "<div id=\"$k\" style=\"display: none;\">";
      }
      // détails dispo
      $html .= "<span class=\"titre\">Objet : </span>" . $d[0]['lobjt'] . "<br/>";
      $html .= "<div class=\"liste\">";
      $html .= "<span class=\"$mclass\">" . $d[0]['co_modu'] . "</span>";
      $html .= $d[0]['lib'];
      $html .= "<div>";
      // détails module
      $html .= "<span class=\"titre\">Objectif : </span>"  . $d[0]['lpeda'] . "<br/>";
      $html .= "<span class=\"titre\">Contenu : </span>"  . $d[0]['lcont'] . "<br/>";
      $html .= "<span class=\"titre\">Public : </span>"  . $d[0]['lcibl'] . "<br/>";
      $html .= "<span class=\"titre\">Modalité : </span>" . $d[0]['lmoda'] . "<br/>";
      $html .= "<span class=\"titre\">Durée : </span>"    . $d[0]['duree'] . "h<br/>";
      $html .= "<span class=\"titre\">Forme : </span>"    . $d[0]['lform'] . "<br/>";
      $html .= "<span class=\"titre\">Responsable pédagogique: </span>" . $d[0]['resppeda'] . "<br/>";
      $html .= "</div>";
      $html .= "</div>";
      $html .= "</div>";
    } else {
      $html .= "<p><span class=\"$dclass\">$k</span>";
      $html .= " <input class=\"image\" type=\"image\" onclick=\"$('#$k').toggle('fast');\" alt=\"A\" src=\"/ent1112b/pub/skins/dafor/image/magnifier.png\" name=\"A\">";
      $html .= "<a href=\"".$k."\">" . $d[0]['lib_dispo'];
      $html .= " " . $tcan[$d[0]['co_tcan']] . "</a>";
      if ($type==3) {
        $html .= "<div id=\"$k\">";
      } else {
        $html .= "<div id=\"$k\" style=\"display: none;\">";
      }
      // détails dispo
      $html .= "<span class=\"titre\">Objet : </span>" . $d[0]['lobjt'] . "<br/>";
      $html .= "</div>";
      foreach ($d as $m) {
        $html .= "<div class=\"liste\">";
        $html .= "<span class=\"$mclass\">" . $m['co_modu'] . "</span>";
        $html .= " <input class=\"image\" type=\"image\" onclick=\"$('#".$m['co_modu']."').toggle('fast');\" alt=\"A\" src=\"/ent1112b/pub/skins/dafor/image/magnifier.png\" name=\"A\">";
        $html .= $m['lib'];
        $html .= " ".$m['categories'];
        if ($type==3) {
          $html .= "<div id=\"".$m['co_modu']."\">";
        } else {
          $html .= "<div id=\"".$m['co_modu']."\" style=\"display: none;\">";
        }
        // détails module
        $html .= "<span class=\"titre\">Objectif : </span>"  . $m['lpeda'] . "<br/>";
        $html .= "<span class=\"titre\">Contenu : </span>"  . $m['lcont'] . "<br/>";
        $html .= "<span class=\"titre\">Modalité : </span>" . $m['lmoda'] . "<br/>";
        $html .= "<span class=\"titre\">Public : </span>"  . $d[0]['lcibl'] . "<br/>";
        $html .= "<span class=\"titre\">Durée : </span>"    . $m['duree'] . "h<br/>";
        $html .= "<span class=\"titre\">Forme : </span>"    . $m['lform'] . "<br/>";
        $html .= "<span class=\"titre\">Responsable pédagogique: </span>" . $m['resppeda'] . "<br/>";
        $html .= "</div>";
        $html .= "</div>";
      }
    };
    $html .= "</p>";
  }
  } else {
    $html = " Aucun stage ne correspond à votre recherche.";
  };
  return $html;
}

/**
 * affiche le tableau des catégories
 */
function gbb_pia_categories($args) {
  foreach (taxonomy_get_vocabularies() as $k=>$v) {
    if ($k == 6) {
      $str .= gbb_pia_vocab($k,$v->name);
    } else {
      $str = gbb_pia_vocab($k,$v->name);
    };
    $rows[0][] = array('data' => $str,
                       'class'=>'topAlign');
  }
  $item[0][] = $rows[0][3];
  $item[0][] = $rows[0][0];
  $item[0][] = $rows[0][4];
  $output = theme('table',array('rows'   => $item ));
  return $output;
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
    if ($nb > 0) $str = l($str,"pia/$vid/$term->tid");
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
  return $output;
}

