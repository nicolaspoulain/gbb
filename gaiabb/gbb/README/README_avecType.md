%GaiaBB
%Documentation Technique du module Drupal
%N. Poulain

<!-- Commande pour générer le pdf
file=README.md && java -jar ../../asciiArt/plantuml.jar $file.md && pandoc-1.9.1.2 --toc --number-sections --smart -s $file.md -o $file.pdf &&  evince $file.pdf
-->


Le module GaiaBB
================
Il est placé dans le dossier sites/default/modules/
de l'arborescence drupal.
On trouve donc
sites/default/modules/gaiabb qui contient notamment
* gaiabb.info (fichier de description du module)
* gaiabb.module

Scripts de mise à jour de la base de données
============================================

Le dossier unl_mysql contenu dans le dossier gaiabb
contient les scripts relatifs à la mise à jour de la base
de données :

GaiaBBupdateSQL.sh
--------------------

SYNOPSIS
  ~ GaiaBBupdateSQL.sh [OPTION]...
DESCRIPTION
  ~ Lance la mise à jour des tables gaia lues par le module gaiaBB. Lance 
    les  sauvegardes.
  ~ -g  Récupère par GET FTP les fichiers au format unl.
  ~ -a  Lance le script unl2sql sur chacun des fichier unl afin de les traduire en lignes d'injection SQL.
  ~ -m lance les injections SQL.

unl2sql.sh
------------
  
SYNOPSIS
  ~ unl2sql FILE N
DESCRIPTION
  ~ Creation du fichier FILE.SQL depuis FILE.unl possedant N champs. Le fchier est converti de UTF8 vers ISO-8859-15 puis
    nettoyé, les sauts de ligne sont supprmés. Le fichier SQL est créé.

unlChecker.sh
---------------

SYNOPSIS
  ~ unlChecker.sh FILE CHAMPS_ATTENDUS CHAMPS POIDS_MIN POIDS
DESCRIPTION
  ~ Verification de validite de FILE.unl et log des résultats. On vérifie que CHAMPS_ATTENDUS=CHAMPS et que POIDS_MIN<POIDS.


Tables ajoutées à la base de données
====================================

<!--
@startuml BDD.png

gbb_gmodu -- gbb_ncont
gbb_session -right- gbb_gmodu
gbb_session -- gbb_gmodu_plus
gbb_gresp -down- gbb_session
gbb_gdire -right- gbb_gmodu
gbb_gdire -- gbb_gresp
gbb_gdire -down- gbb_gdisp
gbb_session -- gbb_netab
gbb_gdisp -- gbb_norie
gbb_gdisp -- gbb_ntcan

class gbb_gdire {
  +co_disp -int(11)
  +co_resp -int(11)
  +co_tres -char(1)
  +co_modu -int(11)
  +co_degre -char(1)
}

class gbb_gdisp {
  +<b>co_disp</b> -int(11)
  id_disp char(10)
  lib char(50)
  libl char(250)
  co_objt char(2)
  co_init char(2)
  co_tcan char(1)
  nb_place_prev smallint(6)
  duree_prev smallint(6)
  nb_mod_prev smallint(6)
  co_chap char(4)
  co_art char(2)
  dt_crea date
  ty_crea char(1)
  co_camp char(2)
  co_them char(2)
  co_andi char(2)
  lobjt text
  co_orga int(11)
  co_offre int(11)
  co_orie char(4)
  gestionnaire char(50)
  co_tpla char(1)
  co_offreur char(8)
  code_om_stag char(2)
  code_remu char(2)
  code_om_interv char(2)
  +co_degre char(1)
}

class gbb_gmodu {
  co_disp int(11)
  +co_modu int(11)
  co_type char(1)
  lib char(50)
  libl char(250)
  co_cont char(3)
  co_form char(1)
  co_moda char(1)
  co_cibl char(2)
  duree_prev smallint(6)
  nb_place_prev smallint(6)
  nb_interv smallint(6)
  nb_h_interv smallint(6)
  co_regr char(6)
  conv char(1)
  co_prac char(2)
  co_prna char(2)
  co_budg char(3)
  dt_crea date
  cumul char(1)
  lcont text
  lmoda text
  lcibl text
  lautre text
  co_anmo char(2)
  co_remp char(1)
  co_omodu int(11)
  lpeda text
  lform text
  nb_groupe smallint(6)
  nb_eff_groupe smallint(6)
  cout_p_fonc decimal(7,2)
  cout_p_prest decimal(7,2)
  cout_p_excep decimal(7,2)
  co_affich_org char(1)
  lcomm char(250)
  co_etab_dem char(8)
  publie char(1)
  +co_degre char(1)
}

class gbb_gresp {
  +co_resp int(11)
  id_resp char(5)
  qualite char(60)
  num_voie char(4)
  nat_voie char(4)
  des_voie char(20)
  esc_res char(30)
  cp char(5)
  vil char(30)
  tel char(14)
  fax char(14)
  mel char(60)
  organ char(60)
  nomu char(20)
  prenom char(15)
  civilite char(1)
  +co_degre char(1)
}

class gbb_ncont {
  +co_cont char(3)
  lib_court char(10)
  lib_long char(50)
  dt_deb date
  dt_fin date
}


class gbb_netab {
  +co_lieu char(8)
  co_natu char(3)
  co_cate char(4)
  secteur char(2)
  sigle char(6)
  denom_princ char(30)
  denom_comp char(30)
  adr char(32)
  mention char(32)
  lieu_dit char(24)
  bp char(7)
  cedex char(2)
  lieu_dist char(26)
  cp char(5)
  pays char(32)
  tel char(14)
  fax char(14)
  mel char(60)
  co_acad char(3)
  co_comm char(6)
  mdp char(8)
  co_circ char(8)
  zeelno char(5)
  flag_mel char(1)
}
note bottom of gbb_netab : On n'utilise pas la version\n extraitre de GAIA

class gbb_norie {
  +co_orie char(4)
  lib_court char(10)
  lib_long char(50)
  dt_deb date
  dt_fin date
}

class gbb_session {
  +sess_id smallint(8)
  session_alert tinyint(1)
  co_resp smallint(8)
  co_modu int(11)
  co_lieu varchar(8)
  groupe tinyint(2)
  date date
  horaires varchar(50)
  typePaiement
  dureeapayer decimal(4,2)
  dureeprevue decimal(4,2)
}

class gbb_gmodu_plus {
  +co_modu int(11)
  en_attente tinyint(1)
  module_alert tinyint(1)
  rne varchar(10)
  date_demande date
  date_premiere_session date
  date_derniere_session date
  organisation text
  CDC_etat tinyint(1)
  liste_stagiaires text
  convoc_info_off text
  convoc_info_on text
  convoc_alert tinyint(1)
  convoc_printed tinyint(1)
  convoc_sent tinyint(1)
  LE_etat tinyint(1)
  LE_info text
  DSF_etat tinyint(1)
  DSF_info text
  DA_info text
}

class gbb_ntcan {
  +co_tcan char(1)
  lib_court char(10)
  lib_long char(50)
  dt_deb date
  dt_fin date
}
@enduml
-->

![Schéma](BDD.png)


