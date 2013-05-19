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
  if (is_numeric(arg(1))) {
    $build['tab'] = gbb_pia_stages(arg(1));
  } else {
    $build['tab'] = gbb_pia_categories($args);
  };
  return implode('', $build);
}


/*
  insert into gbb_gmodu_taxonomy (co_modu,tid,weight) 
   select co_modu,tid, weight 
     from gbb_gdisp as d 
     left join gbb_gmodu as g on g.co_disp=d.co_disp 
     left join gbb_gmoof_taxonomy as gt on gt.co_omodu=g.co_omodu  
     WHERE weight <500 AND id_disp like '13%'
 */
function gbb_pia_stages($tid) {
  $query = db_select('gbb_gmodu_taxonomy', 'gt');
  $query->condition('tid', $tid);
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
    $dispo[$stage->id_disp][] = array('co_modu' => $stage->co_modu,
                                      'id_disp' => $stage->id_disp,
                                      'lib_dispo' => $stage->lib_dispo,
                                      'lib' => $stage->lib);
  };
  $html = "";
  foreach ($dispo as $k=>$d) {
    $html .= "<p><span class=\"iddisp\">$k</span>";
    $html .= "<input type=\"image\" onclick=\"$('#$k').toggle('fast');\" alt=\"A\" src=\"/ent1112b/pub/skins/dafor/image/magnifier.png\" name=\"A\">";
    $html .= "<div id=\"$k\" style=\"display: none;\">";
    $html .= "details dispo";
    $html .= "</div>";
    foreach ($d as $m) {
      if (count($d) == 1) {
        $html .= "<div id=\"$k\" style=\"display: none;\">";
        $html .= $m['co_modu'] . " " . $m['lib'];
        $html .= "</div>";
    } else {
        $html .= "<div>".$m['co_modu'];
        $html .= "<input type=\"image\" onclick=\"$('#".$m['co_modu']."').toggle('fast');\" alt=\"A\" src=\"/ent1112b/pub/skins/dafor/image/magnifier.png\" name=\"A\">";
        $html .= $m['lib'];
        $html .= "<div id=\"".$m['co_modu']."\" style=\"display: none;\">";
        $html .= "details module";
        $html .= "</div>";
        $html .= "</div>";
    }
    };
    $html .= "</p>";
  };
  $html = "";
  foreach ($dispo as $k=>$d) {
    if (count($d) == 1) {
      $html .= "<p><span class=\"iddisp\">$k</span>";
      $html .= " <input class=\"image\" type=\"image\" onclick=\"$('#$k').toggle('fast');\" alt=\"A\" src=\"/ent1112b/pub/skins/dafor/image/magnifier.png\" name=\"A\">";
      $html .= $d[0]['lib_dispo'];
      $html .= "<div id=\"$k\" style=\"display: none;\">";
      $html .= "details dispo";
      $html .= "<span class=\"comodu\">" . $d[0]['co_modu'] . "</span>";
      $html .= $d[0]['lib'];
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
        $html .= "<div id=\"".$m['co_modu']."\" style=\"display: none;\">";
        $html .= "details module";
        $html .= "</div>";
        $html .= "</div>";
      }
    };
    $html .= "</p>";
  };
foreach ($dispo as $k=>$d) {
    $items[] = array('data'=> $k);
    foreach ($d as $m) {
      if (count($d) == 1) {
        $items[] = array('data'=> $m['co_modu']);
    } else {
        $items[] = array('data'=> $m['co_modu']. " ". $m['lib']);
    }
    };
  };
  $title = "TITRE";
  $type = 'ul';
  $attributes = array(
     'id' => 'my-custom-listing',
     'class' => 'ul-custom-class'.' another-custom-class', // a string or indexed
   );
  $output = theme_item_list(array('items' => $items, 'title' => $title, 'type' => $type, 'attributes' => $attributes));
  return $html;
}

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

function gbb_pia_vocab($vid,$title) {

  $terms = taxonomy_get_tree($vid);
  foreach ($terms as $term) {
    $query = db_select('gbb_gmodu_taxonomy', 'gt');
    $query->condition('tid', $term->tid);
    $query->fields('gt', array('co_modu'));
    $nb = $query->countQuery()->execute()->fetchField();
    $str = str_repeat(' - ', $term->depth) . $term->name;
    if ($nb > 0) $str = l($str,"pia/$term->tid");
    $items[] = array('data'=>$str);
  };
  $type = 'ul';
  $attributes = array(
     'id' => 'my-custom-listing'.$vid,
     'class' => 'ul-custom-class'.$vid.' another-custom-class', // a string or indexed
   );
  $output = theme_item_list(array('items' => $items, 'title' => $title, 'type' => $type, 'attributes' => $attributes));
  return $output;
  }
