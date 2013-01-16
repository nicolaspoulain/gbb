<?php

class GmoduViewsController extends EntityDefaultViewsController {

/**
 * Override views_data().
 * Ajoute les liaisons des autres tables à la table gmodu
 */
  public function views_data() {
    $data = parent::views_data();

    // la classe EntityDefaultViewsController a créé les éléments
    // pour views à partir du hook_views du .install
    $data['g_gmodu']['table']['group'] = t('gmodu');
    // On redéfinit ici les noms des champs pour views
    $data['g_gmodu']['co_disp']['title']
                 = t('co_disp *Code du dispositif*');
    $data['g_gmodu']['co_modu']['title']
                 = t('co_modu *Code du module*');
    $data['g_gmodu']['co_type']['title']
                 = t('co_type *Type du module*');
    $data['g_gmodu']['lib']['title']
                 = t('lib *Libellé*');
    $data['g_gmodu']['libl']['title']
                 = t('libl *Libellé long*');
    $data['g_gmodu']['co_cont']['title']
                 = t('co_cont *Code contenu*');
    $data['g_gmodu']['co_cont']['help']
                 = t('Pour la jointure avec la table NCONT qui contient les libellés des contenus');
    $data['g_gmodu']['co_form']['title']
                 = t('co_form *Code forme*');
    $data['g_gmodu']['co_moda']['title']
                 = t('co_moda *Code modalité*');
    $data['g_gmodu']['co_cibl']['title']
                 = t('co_cibl *Code du public cible*');
    $data['g_gmodu']['duree_prev']['title']
                 = t('duree_prev *Durée moyenne estimée par personne*');
    $data['g_gmodu']['nb_place_prev']['title']
                 = t('nb_place_prev *Nombre de places prévues*');
    $data['g_gmodu']['nb_interv']['title']
                 = t('nb_interv *Nombre d\'intervenants*');
    $data['g_gmodu']['nb_h_interv']['title']
                 = t('nb_h_interv *Nombre d\'heures d\'intervention*');
    $data['g_gmodu']['co_regr']['title']
                 = t('co_regr *Code regroupement*');
    $data['g_gmodu']['conv']['title']
                 = t('conv *Flag convention ? O/N*');
    $data['g_gmodu']['co_prac']['title']
                 = t('co_prac *Code priorité académique*');
    $data['g_gmodu']['co_prna']['title']
                 = t('co_prna *Code priorité nationale*');
    $data['g_gmodu']['co_budg']['title']
                 = t('co_budg *Code budget*');
    $data['g_gmodu']['dt_crea']['title']
                 = t('dt_crea *date de création*');
    $data['g_gmodu']['cumul']['title']
                 = t('cumul *Cumul pour le 1 degré si plus 36 semaines*');
    $data['g_gmodu']['lcont']['title']
                 = t('lcont *Contenu littéral*');
    $data['g_gmodu']['lmoda']['title']
                 = t('lmoda *Modalité littérale*');
    $data['g_gmodu']['lcibl']['title']
                 = t('lcibl *Cible littérale*');
    $data['g_gmodu']['lautre']['title']
                 = t('lautre *Autre zone littérale*');
    $data['g_gmodu']['co_anmo']['title']
                 = t('co_anmo *Code du type d\'annulation du module*');
    $data['g_gmodu']['co_remp']['title']
                 = t('co_remp *Code de remplacement 1er degré*');
    $data['g_gmodu']['co_omodu']['title']
                 = t('co_omodu *Code du module de l offre*');
    $data['g_gmodu']['lpeda']['title']
                 = t('lpeda *Descriptif de l\'objectif pédagogique*');
    $data['g_gmodu']['lform']['title']
                 = t('lform *Description littérale de la forme*');
    $data['g_gmodu']['nb_groupe']['title']
                 = t('nb_groupe *Nombre de groupes possible*');
    $data['g_gmodu']['nb_eff_groupe']['title']
                 = t('nb_eff_groupe *Effectif par groupe*');
    $data['g_gmodu']['cout_p_fonc']['title']
                 = t('cout_p_fonc *Cout prévisionnel de fonctionnement par groupe*');
    $data['g_gmodu']['cout_p_prest']['title']
                 = t('cout_p_prest *Cout prévisionnel de prestation par groupe*');
    $data['g_gmodu']['cout_p_excep']['title']
                 = t('cout_p_excep *Cout prévisionnel des frais exceptionnels*');
    $data['g_gmodu']['co_affich_org']['title']
                 = t('co_affich_org *Code affichage des organisations prévisionnelles - arborescence plan (O/N)*');
    $data['g_gmodu']['lcomm']['title']
                 = t('lcomm *Commentaires*');
    $data['g_gmodu']['co_etab_dem']['title']
                 = t('co_etab_dem *Code de l établissement demandeur*');
    $data['g_gmodu']['publie']['title']
                 = t('publie *Publication des demandes*');
    $data['g_gmodu']['co_degre']['title']
                 = t('co_degre *Code du degré*');

  return $data;
  }
}


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
      'group' => t('gdisp'),
      'join' => array(
        'g_gmodu' => array(
          'left_table' => 'g_gdire',
          'left_field' => 'co_disp',
          'field' => 'co_disp',
          'extra' => 'g_gdisp.co_degre = g_gmodu.co_degre',
        ),
      ),
    );
    $data['g_gdisp']['co_disp']['title']
                 = t('co_disp *Code interne du dispositif*');
    $data['g_gdisp']['id_disp']['title']
                 = t('id_disp *Identifiant du dispositif*');
    $data['g_gdisp']['lib']['title']
                 = t('lib *Libellé du dispositif*');
    $data['g_gdisp']['libl']['title']
                 = t('libl *Libellé long du dispositif*');
    $data['g_gdisp']['co_objt']['title']
                 = t('co_objt *Code de l\'objectif de formation*');
    $data['g_gdisp']['co_init']['title']
                 = t('co_init *Code du niveau d\'initiative*');
    $data['g_gdisp']['co_tcan']['title']
                 = t('co_tcan *Code du type de candidature*');
    $data['g_gdisp']['nb_place_prev']['title']
                 = t('nb_place_prev *Nombre de places prévues*');
    $data['g_gdisp']['duree_prev']['title']
                 = t('duree_prev *Durée moyenne estimée par personne*');
    $data['g_gdisp']['nb_mod_prev']['title']
                 = t('nb_mod_prev *Nombre de modules prévus*');
    $data['g_gdisp']['co_chap']['title']
                 = t('co_chap *Code du chapitre de prévision*');
    $data['g_gdisp']['co_art']['title']
                 = t('co_art *Code article de prévision*');
    $data['g_gdisp']['dt_crea']['title']
                 = t('dt_crea *date de création*');
    // ajouter le handler de date pour le champ précédent
    $data['g_gdisp']['ty_crea']['title']
                 = t('ty_crea *Type de création*');
    $data['g_gdisp']['co_camp']['title']
                 = t('co_camp *Code de la campagne*');
    $data['g_gdisp']['co_them']['title']
                 = t('co_them *Code du thème du dispositif*');
    $data['g_gdisp']['co_andi']['title']
                 = t('co_andi *Code du type d\'annulation du dispositif*');
    $data['g_gdisp']['lobjt']['title']
                 = t('lobjt *Objectif littéral*');
    $data['g_gdisp']['co_orga']['title']
                 = t('co_orga *Code de l\'organisme*');
    $data['g_gdisp']['co_offre']['title']
                 = t('co_offre *Code interne de l\'offre*');
    $data['g_gdisp']['co_orie']['title']
                 = t('co_orie *Code d\'orientation*');
    $data['g_gdisp']['co_orie']['help']
                 = t('Pour la jointure avec la table NORIE qui contient les libellés des orientations');
    $data['g_gdisp']['gestionnaire']['title']
                 = t('gestionnaire *Gestionnaire en charge du dispositif*');
    $data['g_gdisp']['co_tpla']['title']
                 = t('co_tpla *Code type de plan*');
    $data['g_gdisp']['co_offreur']['title']
                 = t('co_offreur *Code offreur*');
    $data['g_gdisp']['code_om_stag']['title']
                 = t('code_om_stag *Code du modèle choisi pour l\'OM stagiaire*');
    $data['g_gdisp']['code_remu']['title']
                 = t('code_remu *Code du modèle choisi pour la fiche de rémunération*');
    $data['g_gdisp']['code_om_interv']['title']
                 = t('code_om_interv *Code du modèle choisi pou l\'OM intervenant*');
    $data['g_gdisp']['co_degre']['title']
                 = t('co_degre *Code du degré*');

    // gbb_gdire table de liaison, pas de champ à extraire
    $data['g_gdire']['table'] = array(
            'group' => t('gdire'),
            'join' => array(
              'g_gmodu' => array(
                'left_field' => 'co_modu',
                'field' => 'co_modu',
                'extra' => 'g_gdire.co_degre = g_gmodu.co_degre',
              ),
            ),
    );
    $data['g_gdire']['co_tres'] = array(
            'title' => t('co_tres *Code trésorerie*'),
            'help' => t('1=>Interlocuteur - 2=>Resp.Orga - 3=>Resp.Péda'),
            'field' => array(
              'handler' => 'views_handler_field_numeric',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_numeric'),
           'argument' => array('handler' => 'views_handler_argument_numeric'),
    );

    $data['g_gdire']['co_resp'] = array(
            'title' => t('co_resp *Code responsable*'),
            'help' => t('Le nom correspondant se trouve dans la table "gresp *Responsable de module)"'),
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
            'group' => t('session'),
            'join' => array(
              'g_gmodu' => array(
                'left_field' => 'co_modu',
                'field' => 'co_modu',
                'extra' => 'g_session.co_degre = g_gmodu.co_degre',
              ),
            ),
    );
    $data['g_session']['sess_id']['title']
                   = t('sess_id *Identifiant de la session*');
    $data['g_session']['co_modu']['title']
                   = t('co_modu *Code du module*');
    $data['g_session']['co_degre']['title']
                   = t('co_degre *Code du degré*');
    $data['g_session']['co_resp']['title']
                   = t('co_resp *Code du responsable*');
    $data['g_session']['co_lieu']['title']
                   = t('co_lieu *Code du lieu*');
    $data['g_session']['co_lieu']['help']
                   = t('En liaison avec la table NETAB qui contient les renseignements sur les établissements');
    $data['g_session']['groupe']['title']
                   = t('groupe *Numéro du groupe*');
    $data['g_session']['date']['title']
                   = t('date *Date de la session*');
    $data['g_session']['horaires']['title']
                   = t('horaires *Horaires de la session*');
    $data['g_session']['type_paiement']['title']
                   = t('type_paiement *Type de paiement du formateur*');
    $data['g_session']['uid']['title']
                   = t('uid *User id du responsable de la dernière modif*');
    $data['g_session']['date_modif']['title']
                   = t('date_modif *Date de dernière modification*');
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
            'group' => t('gresp *Responsable de session*'),
            'join' => array(
              'g_gmodu' => array(
                'left_table' => 'g_session',
                'left_field' => 'co_resp',
                'field' => 'co_resp',
                'extra' => 'g_gresp.co_degre = g_gmodu.co_degre',
              ),
            ),
    );
    $data['g_resp']['co_resp']['title']
                = t('co_resp *Code responsable*');
    $data['g_resp']['id_resp']['title']
                = t('id_resp *Identifiant du responsable*');
    $data['g_resp']['qualite']['title']
                = t('qualite *Qualité du responsable càd fonction*');
    $data['g_resp']['num_voie']['title']
                = t('num_voie *Numéro pour la voie*');
    $data['g_resp']['nat_voie']['title']
                = t('nat_voie *Nature de la voie*');
    $data['g_resp']['des_voie']['title']
                = t('des_voie *Désignation de la voie*');
    $data['g_resp']['esc_res']['title']
                = t('esc_res *Escalier - résidence*');
    $data['g_resp']['cp']['title']
                = t('cp *Code postal*');
    $data['g_resp']['vil']['title']
                = t('vil *Ville*');
    $data['g_resp']['tel']['title']
                = t('tel *Numéro de téléphone*');
    $data['g_resp']['fax']['title']
                = t('fax *Numéro de fax*');
    $data['g_resp']['mel']['title']
                = t('mel *@mail*');
    $data['g_resp']['organ']['title']
                = t('organ *Organisme*');
    $data['g_resp']['nomu']['title']
                = t('nomu *Nom d usage*');
    $data['g_resp']['prenom']['title']
                = t('prenom *Prénom*');
    $data['g_resp']['civilite']['title']
                = t('civilite *Civilité*');
    $data['g_resp']['co_degre']['title']
                = t('co_degre *Code du degré*');

    $data['g_gresp_BIS']['table'] = array(
            'group' => t('gresp *Responsable de module*'),
            'join' => array(
              'g_gmodu' => array(
                'table' => 'g_gresp',
                'left_table' => 'g_gdire',
                'left_field' => 'co_resp',
                'field' => 'co_resp',
                'extra' => 'g_gresp_BIS.co_degre = g_gmodu.co_degre',
              ),
            ),
    );
    $data['g_gresp_BIS']['nomu'] = array(
            'title' => t('nomu *Nom d\'usage'),
            'help' => t('Nom du reponsable d\'un module'),
            'field' => array(
              'handler' => 'views_handler_field',
              'click sortable' => TRUE,
            ),
           'sort' => array('handler' => 'views_handler_sort'),
           'filter' => array('handler' => 'views_handler_filter_string'),
           'argument' => array('handler' => 'views_handler_argument_string'),
    );
    $data['g_gresp_BIS']['prenom'] = array(
            'title' => t('prenom *Prénom*'),
            'help' => t('Prénom du reponsable d\'un module'),
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
}

class NetabViewsController extends EntityDefaultViewsController {

/**
 * Override views_data().
 * Ajoute les liaisons des autres tables à la table gmodu
 */
  public function views_data() {
    $data = parent::views_data();
    $data['g_netab']['table'] = array(
            'group' => t('netab'),
            'join' => array(
              'g_gmodu' => array(
                'left_table' => 'g_session',
                'left_field' => 'co_lieu',
                'field' => 'co_lieu',
              ),
            ),
    );

    $data['g_netab']['co_lieu']['title']
                 = t('co_lieu *Code lieu ou RNE*');
    $data['g_netab']['co_natu']['title']
                 = t('co_natu *Code nature de l\'établissement*');
    $data['g_netab']['co_cate']['title']
                 = t('co_cate *Code catégorie de l\'établissement*');
    $data['g_netab']['secteur']['title']
                 = t('secteur *Secteur*');
    $data['g_netab']['description']['title']
                 = t('description *Sigle de l établissement*');
    $data['g_netab']['denom_princ']['title']
                 = t('denom_princ *Dénomination principale*');
    $data['g_netab']['denom_comp']['title']
                 = t('denom_comp *Dénomination complémentaire*');
    $data['g_netab']['adr']['title']
                 = t('adr *Adresse*');
    $data['g_netab']['mention']['title']
                 = t('mention *Mention*');
    $data['g_netab']['lieu_dit']['title']
                 = t('lieu_dit *Lieu-dit*');
    $data['g_netab']['bp']['title']
                 = t('bp *Boite postale*');
    $data['g_netab']['cedex']['title']
                 = t('cedex *Cedex*');
    $data['g_netab']['lieu_dist']['title']
                 = t('lieu_dist *Bureau distributeur*');
    $data['g_netab']['cp']['title']
                 = t('cp *Code postal*');
    $data['g_netab']['pays']['title']
                 = t('pays *Pays*');
    $data['g_netab']['tel']['title']
                 = t('tel *Numéro de téléphone*');
    $data['g_netab']['fax']['title']
                 = t('fax *Numéro de fax*');
    $data['g_netab']['mel']['title']
                 = t('mel (@mail*');
    $data['g_netab']['co_acad']['title']
                 = t('co_acad *Code de l\'académie*');
    $data['g_netab']['co_comm']['title']
                 = t('co_comm *Code commune*');
    $data['g_netab']['mdp']['title']
                 = t('mdp *Mot de passe*');
    $data['g_netab']['co_circ']['title']
                 = t('co_circ *Code circonscription*');
    $data['g_netab']['zeelno']['title']
                 = t('zeelno *Numéro de zone élémentaire*');
    $data['g_netab']['flag_mel']['title']
                 = t('flag_mel *Flag pour envois des Ordres de Mission -OM- par mail (O/N)*');
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
            'group' => t('ncont'),
            'join' => array(
              'g_gmodu' => array(
                'left_field' => 'co_cont',
                'field' => 'co_modu',
              ),
            ),
    );
    $data['g_ncont']['co_cont']['title']
                 = t('co_cont *Code contenu*');
    $data['g_ncont']['lib_court']['title']
                 = t('lib_court *Libellé court*');
    $data['g_ncont']['lib_long']['title']
                 = t('lib_long *Libellé long*');
    $data['g_ncont']['dt_deb']['title']
                 = t('dt_deb *Date de début*');
    $data['g_ncont']['dt_fin']['title']
                 = t('dt_fin *Date de fin*');
    return $data;
  }
}



