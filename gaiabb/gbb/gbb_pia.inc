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
  setlocale(LC_CTYPE, 'fr_FR.ISO-8859-1');
  if (is_numeric(arg(2))) {
    $build['tab'] = gbb_pia_stages(1,arg(2));
  //} elseif (isset($_GET['kw']) && ctype_lower(iconv( "UTF-8", "ISO-8859-1", $_GET['kw']))) {
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
    '#description' => t('par ex. : écriture, agrégation ou théâtre...'),
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
function gbb_pia_stages($type,$arg) {
  $query = db_select('gbb_gmodu_taxonomy', 'gt');
  $query->condition('weight', 500, '<');
  switch ($type) {
    case 1:
      $query->condition('tid', $arg);
      break;
    case 2:
      $query->condition('gd.lib', '%' . db_like($arg) . '%', 'LIKE');
      break;
  }
  $query->condition('id_disp', db_like('13') . '%', 'LIKE');
  $query->join('gbb_gmodu', 'gm', 'gt.co_modu = gm.co_modu');
  $query->join('gbb_gdisp', 'gd', 'gd.co_disp = gm.co_disp');
  $query->fields('gt', array('co_modu'));
  $query->fields('gm', array('lib'));
  $query->fields('gd', array('id_disp'));
  $query->addField('gd', 'lib', 'lib_dispo');
  $query->orderBy('weight', 'ASC');
  $res = $query->distinct()->execute()->fetchAll();
  foreach ($res as $stage) {
    /*
    $query = db_select('taxonomy_term_data', 'ttd');
    $query->condition('tid', $stage->tid);
    $query->fields('ttd', array('name'));
    $cats = $query->distinct()->execute()->fetchCol();
    */
    $query = db_select('gbb_gmodu_taxonomy', 'gt');
    $query->join('taxonomy_term_data', 'ttd', 'gt.tid=ttd.tid');
    $query->condition('co_modu', $stage->co_modu);
    $query->fields('ttd', array('vid'));
    $cats = $query->distinct()->execute()->fetchCol();
    $catss = "<span class=\"ul-custom-class".implode("\">&nbsp;&nbsp;</span> <span class=\"ul-custom-class",$cats)."\">&nbsp;&nbsp;</span>";
    $dispo[$stage->id_disp][] = array('co_modu' => $stage->co_modu,
                                      'id_disp' => $stage->id_disp,
                                      'lib_dispo' => $stage->lib_dispo,
                                      'categories' => $catss,
                                      'lib' => $stage->lib);
  };
  //$html = "<p class=\"retour\"><a href=\"pia\">Retour</a></p> ";
  $html = "";
  if (isset($dispo)) {
  foreach ($dispo as $k=>$d) {
    if (count($d) == 1) {
      $html .= "<p><span class=\"iddisp\">$k</span>";
      $html .= " <input class=\"image\" type=\"image\" onclick=\"$('#$k').toggle('fast');\" alt=\"A\" src=\"/ent1112b/pub/skins/dafor/image/magnifier.png\" name=\"A\">";
      $html .= $d[0]['lib_dispo'];
       $html .= " ".$d[0]['categories'];
      $html .= "<div id=\"$k\" style=\"display: none;\">";
      $html .= "details dispo";
      $html .= "<div class=\"liste\">";
      $html .= "<span class=\"comodu\">" . $d[0]['co_modu'] . "</span>";
      $html .= $d[0]['lib'];
      $html .= "<div>";
      $html .= "details module";
      $html .= "</div>";
      $html .= "</div>";
      $html .= "</div>";
    } else {
      $html .= "<p><span class=\"iddisp\">$k</span>";
      $html .= " <input class=\"image\" type=\"image\" onclick=\"$('#$k').toggle('fast');\" alt=\"A\" src=\"/ent1112b/pub/skins/dafor/image/magnifier.png\" name=\"A\">";
      $html .= $d[0]['lib_dispo'];
      $html .= "<div id=\"$k\" style=\"display: none;\">";
      $html .= "details dispo";
      $html .= "</div>";
      foreach ($d as $m) {
        $html .= "<div class=\"liste\">";
        $html .= "<span class=\"comodu\">" . $m['co_modu'] . "</span>";
        $html .= " <input class=\"image\" type=\"image\" onclick=\"$('#".$m['co_modu']."').toggle('fast');\" alt=\"A\" src=\"/ent1112b/pub/skins/dafor/image/magnifier.png\" name=\"A\">";
        $html .= $m['lib'];
         $html .= " ".$m['categories'];
        $html .= "<div id=\"".$m['co_modu']."\" style=\"display: none;\">";
        $html .= "details module";
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
    $str = str_repeat(' - ', $term->depth) . $term->name;
    if ($nb > 0) $str = l($str,"pia/$vid/$term->tid");
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
