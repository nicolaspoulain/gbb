%GaiaBB
%Documentation Technique du module Drupal
%N. Poulain

<!-- Commande pour générer le pdf
file=README && java -jar ../../asciiArt/plantuml.jar $file.md && pandoc-1.9.1.2 --toc --number-sections --smart -s $file.md -o $file.pdf &&  evince $file.pdf
-->


Git
===

Ajout d'un nouveau fichier
--------------------------

Après ajout du nouveau fichier `FILE` dans le module gaiabb, on crée un lien
matériel vers dropbox puis on l'ajoute dans git.

`$ cd ~/Dropbox/gaiabb`
`$ ln /var/www/drupal-7.14/sites/default/modules/gaiabb/FILE FILE`
`$ git add FILE`


Commmiter des changements
-------------------------

le but est de créer un commit puis de 
tranférer vers http://code.google.com/p/npoulain/source/browse/

`$ git commit -a -m "Ajout du fichier FILE"` && git push google master`



Le module GaiaBB
================
Il est placé dans le dossier `sites/default/modules/`
de l'arborescence drupal.
Le dossier `sites/default/modules/gaiabb` contient notamment

* `gaiabb.info` (fichier de description du module)
* `gaiabb.module`

Scripts de mise à jour de la base de données
============================================

Le dossier unl_mysql contenu dans le dossier gaiabb
contient les scripts relatifs à la mise à jour de la base
de données :

`unl2sql.sh`
------------
  
Création du fichier `FILE.SQL` depuis `FILE.unl` possédant N champs. 
Le fichier est converti de UTF-8 vers ISO-8859-15 puis
nettoyé, les sauts de ligne sont supprimés. Le fichier SQL est créé.

`root@ks:# ./unl2sql FILE N`

`unlChecker.sh`
---------------

Verification de validité de FILE.unl et log des résultats. 
On vérifie que `CHAMPS_ATTENDUS = CHAMPS` et que `POIDS_MIN < POIDS`.

`root@ks:# ./unlChecker.sh FILE CHAMPS_ATTENDUS CHAMPS POIDS_MIN POIDS`

`GaiaBBupdateSQL.sh`
--------------------

C'est le script principal à lancer dans le cadre d'une mise à jour automatisée
par cron.
Il Lance la mise à jour des tables gaia lues par le module gaiaBB.
Lance aussi les  sauvegardes.

* `-n`  Nettoie en supprimant les fichiers .unl
* `-g`  Récupère par GET FTP les fichiers au format unl.
* `-m`  Lance le script unl2sql sur chacun des fichier unl afin de les traduire 
  en lignes d'injection SQL puis lance les injections

Voici un exemple d'utilisation et la sortie associée. 
On constate ici que la mise à jour de la table gmodu de GAIA a rencontré
un problème qui est analysé dans le rapport de sortie daté du jour : 
le poids du fichier est plus petit qu'attendu. 
Le fichier ETAT.log conserve la trace des rapports de sortie au format csv.


~~~
root@ks:# ./GaiaBBupdateSQL.sh -ngm
Usage: GaiaBBupdateSQL.sh [-n nettoyage] [-g getFPT] [-m mysqlInjections]
_N_etttoyage du dossier
_G_et FTP des donnees
.../sites/default/modules/gaiabb/unl_mysql/gdisp.unl:    6.49 MB    6.54 MB/s  
.../sites/default/modules/gaiabb/unl_mysql/gmodu.unl:   21.21 MB    4.71 MB/s  
.../sites/default/modules/gaiabb/unl_mysql/gdire.unl:  936.49 kB    8.74 MB/s  
.../sites/default/modules/gaiabb/unl_mysql/gresp.unl:  638.41 kB    8.07 MB/s  
.../sites/default/modules/gaiabb/unl_mysql/ncont.unl:    7.61 kB  415.51 kB/s  
.../sites/default/modules/gaiabb/unl_mysql/ntcan.unl:  185.00 B    28.91 kB/s  
.../sites/default/modules/gaiabb/unl_mysql/norie.unl:   25.53 kB  993.44 kB/s  
Injections _M_YSQL après amélioration des données
* gmodu.unl en traitement par unl2sql ...               fait.
                                                        ERROR
* gdisp.unl en traitement par unl2sql ...               fait.
* gdisp.SQL en cours d'injection  .....                 fait.

* gresp.unl en traitement par unl2sql ...               fait.
* gresp.SQL en cours d'injection  .....                 fait.

* gdire.unl en traitement par unl2sql ...               fait.
* gdire.SQL en cours d'injection  .....                 fait.

* ncont.unl en traitement par unl2sql ...               fait.
* ncont.SQL en cours d'injection  .....                 fait.

* ntcan.unl en traitement par unl2sql ...               fait.
* ntcan.SQL en cours d'injection  .....                 fait.

* norie.unl en traitement par unl2sql ...               fait.
* norie.SQL en cours d'injection  .....                 fait.

2012-06-10 18:21:14 ; gmodu ; Erreur ; Nb champs : 40 = 40 ; Poids : 37 < 5
2012-06-10 18:21:17 ; gdisp ; OK ; Nb champs : 29 = 29 ; Poids : 8000 < 8128
2012-06-10 18:21:26 ; gresp ; OK ; Nb champs : 18 = 18 ; Poids : 960 < 984
2012-06-10 18:21:30 ; gdire ; OK ; Nb champs : 6 = 6 ; Poids : 2650 < 2876
2012-06-10 18:21:45 ; ncont ; OK ; Nb champs : 6 = 6 ; Poids : 11 < 16
2012-06-10 18:21:45 ; ntcan ; OK ; Nb champs : 6 = 6 ; Poids : 3 < 4
2012-06-10 18:21:45 ; norie ; OK ; Nb champs : 6 = 6 ; Poids : 39 < 40
Il n'y a plus de paramètre
~~~


Tables ajoutées à la base de données drupal
===========================================

<!--
@startuml BDD.png

package "Legende" #DDDDDD {
  Table_locale << (+,Red) >>
  Table_GAIA << (®,YellowGreen) >>  
}

gbb_gmodu -- gbb_ncont
gbb_gmodu -- gbb_file
gbb_gmodu -- gbb_gmodu_plus
gbb_session -- gbb_gmodu
gbb_session -- gbb_gmodu_plus
gbb_session -- gbb_gresp_dafor
gbb_session -- gbb_netab

gbb_gresp_dafor .. gbb_gresp : idem
gbb_gresp_dafor -- gbb_gresp_plus
gbb_gresp -- gbb_gresp_plus

gbb_gdire -- gbb_gmodu
gbb_gdire -- gbb_gresp
gbb_gdire -- gbb_gdisp

gbb_gdisp -- gbb_norie
gbb_gdisp -- gbb_ntcan


gbb_gdire << (®,YellowGreen) >>
class gbb_gdire {
  {static} co_disp
  {static} co_resp
  {static} co_tres
  {static} co_modu
  {static} co_degre
}

gbb_gdisp << (®,YellowGreen) >>
class gbb_gdisp {
  {static} co_disp
  id_disp
  lib
  libl
  co_objt
  co_init
  co_tcan
  nb_place_prev
  duree_prev
  nb_mod_prev
  co_chap
  co_art
  dt_crea
  ty_crea
  co_camp
  co_them
  co_andi
  lobjt
  co_orga
  co_offre
  co_orie
  gestionnaire
  - co_tpla
  - co_offreur
  - code_om_stag
  - code_remu
  - code_om_interv
  - {static} co_degre
}

gbb_gmodu << (®,YellowGreen) >> 
class gbb_gmodu {
  co_disp
  {static} co_modu
  co_type
  lib
  libl
  co_cont
  co_form
  co_moda
  co_cibl
  duree_prev
  nb_place_prev
  nb_interv
  nb_h_interv
  co_regr
  conv
  co_prac
  co_prna
  co_budg
  dt_crea
  cumul
  lcont
  lmoda
  lcibl
  lautre
  co_anmo
  co_remp
  co_omodu
  lpeda
  lform
  nb_groupe
  nb_eff_groupe
  cout_p_fonc
  cout_p_prest
  -cout_p_excep
  -co_affich_org
  -lcomm
  -co_etab_dem
  -publie
  -{static} co_degre
}

gbb_gresp << (®,YellowGreen) >> 
class  gbb_gresp {
  {static} co_resp
  id_resp
  qualite
  num_voie
  nat_voie
  des_voie
  esc_res
  cp
  vil
  tel
  fax
  mel
  organ
  nomu
  prenom
  civilite
  -{static} co_degre
}

gbb_ncont << (®,YellowGreen) >>
class gbb_ncont {
  {static} co_cont
  lib_court
  lib_long
  dt_deb
  dt_fin
}

gbb_netab << (®,YellowGreen) >>
class gbb_netab{
  {static} co_lieu
  co_natu
  co_cate
  secteur
  sigle
  denom_princ
  denom_comp
  adr
  mention
  lieu_dit
  bp
  cedex
  lieu_dist
  cp
  pays
  tel
  fax
  mel
  co_acad
  co_comm
  mdp
  co_circ
  zeelno
  flag_mel
}

gbb_norie << (®,YellowGreen) >>
class gbb_norie {
  {static} co_orie
  lib_court
  lib_long
  dt_deb
  dt_fin
}

gbb_ntcan << (®,YellowGreen) >>
class gbb_ntcan {
  {static} co_tcan
  lib_court
  lib_long
  dt_deb
  dt_fin
}

gbb_session << (+,Red) >>
class gbb_session {
  {static} sess_id
  session_alert
  co_resp
  co_modu
  co_degre
  co_lieu
  groupe
  date
  horaires
  typePaiement
  dureeapayer
  dureeprevue
}

gbb_gresp_dafor << (+,Red) >> 
class  gbb_gresp_dafor {
  {static} co_resp
  id_resp
  qualite
  num_voie
  nat_voie
  des_voie
  esc_res
  cp
  vil
  tel
  fax
  mel
  organ
  nomu
  prenom
  civilite
  {static} co_degre
}

gbb_gmodu_plus << (+,Red) >>
class gbb_gmodu_plus {
  {static} co_modu
  {static} co_degre
  en_attente
  module_alert
  co_lieu
  date_demande
  date_premiere_session
  date_derniere_session
  organisation
  CDC_etat
  liste_stagiaires
  convoc_info_off
  convoc_info_on
  convoc_alert
  convoc_printed
  convoc_sent
  LE_etat
  LE_info
  DSF_etat
  DSF_info
  DA_info
}

gbb_file << (+,Red) >>
class gbb_file {
  {static} co_modu
  {static} co_degre
  {static} id
}

gbb_gresp_plus << (+,Red) >>
class gbb_gresp_plus {
  {static} co_resp
  resp_dafor
  discipline
  decharge
  grade
  divers
}
@enduml
-->

![Schéma de la base de données du module GaiaBB](BDD.png)



TABLE        COL               TYPE      Max   Null Key Deflt  Comment
----------   ---------------   --------- ---   ---- --- ------ --------
file         co\_modu          int       NULL  NO       0      Code du module pour un dispositif donné
file         co\_degre         smallint  NULL  NO       2      Code du degré
file         fid               smallint  NULL  NO       NULL   Clé étrangère avec table file\_managed
gdire        co\_disp          int       NULL  NO   PRI NULL   Code interne du dispositif
gdire        co\_resp          int       NULL  NO   PRI NULL   Code responsable
gdire        co\_tres          char      1     NO   PRI NULL   code du type de responsable
gdire        co\_modu          int       NULL  NO   PRI 0      Code du module pour un dispositif donné
gdire        co\_degre         smallint  NULL  NO   PRI 2      Code du degré
gdisp        co\_disp          int       NULL  NO   PRI NULL   Code interne du dispositif
gdisp        id\_disp          char      10    NO   MUL NULL   Identifiant du dispositif
gdisp        lib               char      50    YES      NULL   Libellé
gdisp        libl              char      250   YES      NULL   Libellé long
gdisp        co\_objt          char      2     YES      NULL   Code de l objectif de formation
gdisp        co\_init          char      2     YES      NULL   Code du niveau d initiative
gdisp        co\_tcan          char      1     YES      NULL   Code du type de candidature
gdisp        nb\_place\_prev   smallint  NULL  YES      NULL   Nombre de places prévues
gdisp        duree\_prev       smallint  NULL  YES      NULL   Durée moyenne estimée par personne
gdisp        nb\_mod\_prev     smallint  NULL  YES      NULL   Nombre de modules prévus
gdisp        co\_chap          char      4     YES      NULL   Code du chapitre de prévision
gdisp        co\_art           char      2     YES      NULL   Code article de prévision
gdisp        dt\_crea          date      NULL  YES      NULL   date de création
gdisp        ty\_crea          char      1     YES      NULL   Type de création
gdisp        co\_camp          char      2     YES      NULL   Code de la campagne
gdisp        co\_them          char      2     YES      NULL   Code du thème du dispositif
gdisp        co\_andi          char      2     YES      NULL   Code du type d annulation du dispositif--
gdisp        lobjt             text      65535 YES      NULL   Objectif littéral
gdisp        co\_orga          int       NULL  YES      NULL   Code de l organisme
gdisp        co\_offre         int       NULL  YES      NULL   Code interne de l offre
gdisp        co\_orie          char      4     YES      NULL   Code nomenclature locale d orientation 
gdisp        gestionnaire      char      50    YES      NULL   Gestionnaire en charge du dispositif
gdisp        co\_tpla          char      1     YES      NULL   Code type de plan
gdisp        co\_offreur       char      8     YES      NULL   Code offreur
gdisp        code\_om\_stag    char      2     YES      NULL   Code du modèle choisi pour l OM stagiaire
gdisp        code\_remu        char      2     YES      NULL   code du modèle choisi pour la fiche de rémunération
gdisp        code\_om\_interv  char      2     YES      NULL   code du modèle choisi pou l OM intervenant
gdisp        co\_degre         smallint  NULL  NO   PRI 2      Code du degré
gmodu        co\_disp          int       NULL  NO   MUL NULL   Code interne du dispositif
gmodu        co\_modu          int       NULL  NO   PRI NULL   Code du module pour un dispositif donné
gmodu        co\_type          char      1     YES      NULL   type du module
gmodu        lib               char      50    YES      NULL   Libellé
gmodu        libl              char      150   YES      NULL   Libellé long
gmodu        co\_cont          char      3     YES      NULL   Code contenu
gmodu        co\_form          char      1     YES      NULL   Code forme
gmodu        co\_moda          char      1     YES      NULL   Code modalité
gmodu        co\_cibl          char      2     YES      NULL   Code du public cible
gmodu        duree\_prev       smallint  NULL  YES      NULL   Durée moyenne estimée par personne
gmodu        nb\_place\_prev   smallint  NULL  YES      NULL   Nombre de places prévues
gmodu        nb\_interv        smallint  NULL  YES      NULL   Nombre d intervenant
gmodu        nb\_h\_interv     smallint  NULL  YES      NULL   Nombre d heures d intervention
gmodu        co\_regr          char      6     YES      NULL   Code regroupement
gmodu        conv              char      1     YES      NULL   Flag convention ? O/N
gmodu        co\_prac          char      2     YES      NULL   Code priorité académique
gmodu        co\_prna          char      2     YES      NULL   Code priorité nationale
gmodu        co\_budg          char      3     YES      NULL   Code budget
gmodu        dt\_crea          date      NULL  YES      NULL   date de création
gmodu        cumul             char      1     YES      NULL   cumul pour le 1 degré si plus 36 semaines
gmodu        lcont             text      65535 YES      NULL   Contenu littéral
gmodu        lmoda             text      65535 YES      NULL   modalité littérale
gmodu        lcibl             text      65535 YES      NULL   Cible littérale
gmodu        lautre            text      65535 YES      NULL   Autre zone littérale
gmodu        co\_anmo          char      2     YES      NULL   Code du type d annulation des modules
gmodu        co\_remp          char      1     YES      NULL   code de remplacement 1er degré
gmodu        co\_omodu         int       NULL  YES      NULL   code du module de l offre
gmodu        lpeda             char      250   YES      NULL   Descriptif de l objectif pédagogique
gmodu        lform             char      250   YES      NULL   Description littérale de la forme
gmodu        nb\_groupe        smallint  NULL  YES      NULL   Nombre de groupes possible
gmodu        nb\_eff\_groupe   smallint  NULL  YES      NULL   Effectif par groupe
gmodu        cout\_p\_fonc     decimal   NULL  YES      NULL   cout prévisionnel de fonctionnement par groupe
gmodu        cout\_p\_prest    decimal   NULL  YES      NULL   cout prévisionnel de prestation par groupe
gmodu        cout\_p\_excep    decimal   NULL  YES      NULL   cout prévisionnel des frais exceptionnels
gmodu        co\_affich\_org   char      1     NO       NULL   Code affichage des organisations prévisionnelles -arborescence plan (O/N)
gmodu        lcomm             char      250   YES      NULL   Commentaires
gmodu        co\_etab\_dem     char      8     YES      NULL   Code de l établissement demandeur
gmodu        publie            char      1     YES      NULL   Publication des demandes
gmodu        co\_degre         smallint  NULL  NO   PRI 2      Code du degré
gmodu\_plus  co\_modu          int       NULL  NO   PRI NULL   Code du module pour un dispositif donné
gmodu\_plus  co\_degre         smallint  NULL  NO   PRI 2      Code du degré
gmodu\_plus  module\_alert     tinyint   NULL  YES      0      Alerte sur le module : service administratif prévenu d un changt important.
gmodu\_plus  prioritaire       tinyint   NULL  YES      0      Prioritaire
gmodu\_plus  module\_printed   tinyint   NULL  YES      0      L ensemble des convocations a été imprimé.
gmodu\_plus  organisation      text      65535 YES      NULL   Journal de bord
gmodu\_plus  CDC\_etat         tinyint   NULL  YES      0      Le CDC a-t-il été transmis au CE ?
gmodu\_plus  convoc\_info\_off text      65535 YES      NULL   Info à ne pas porter sur la convocation 
gmodu\_plus  convoc\_info\_on  text      65535 YES      NULL   Info à porter sur la convocation 
gmodu\_plus  LE\_etat          tinyint   NULL  YES      0      Listes d émargement transmises
gmodu\_plus  LE\_info          text      65535 YES      NULL   Infos à propos des listes d émargement
gmodu\_plus  DSF\_etat         tinyint   NULL  YES      0      Déclaration de Service Fait transmises
gmodu\_plus  DSF\_info         text      65535 YES      NULL   Infos à propos des Déclaration de Service Fait 
gmodu\_plus  DA\_info          text      65535 YES      NULL   Infos à propos du Dossier Administratif
gmodu\_plus  DA\_date\_depot   datetime  NULL  NO       1970... Date de dépot du dossier administratif
gresp        co\_resp          int       NULL  NO   PRI NULL   Code responsable
gresp        id\_resp          char      5     YES      NULL   Identifiant du responsable
gresp        qualite           char      60    YES      NULL   Qualité du responsable (fonction)
gresp        num\_voie         char      4     YES      NULL   Numéro de la voie
gresp        nat\_voie         char      4     YES      NULL   Nature de la voie
gresp        des\_voie         char      20    YES      NULL   adresse
gresp        esc\_res          char      30    YES      NULL   escalier - résidence
gresp        cp                char      5     YES      NULL   code postal
gresp        vil               char      30    YES      NULL   Ville
gresp        tel               char      14    YES      NULL   Téléphone
gresp        fax               char      14    YES      NULL   fax
gresp        mel               char      60    YES      NULL   mail
gresp        organ             char      60    YES      NULL   Organisme
gresp        nomu              char      20    YES      NULL   Nom d usage
gresp        prenom            char      15    YES      NULL   Prénom de la personne
gresp        civilite          char      1     YES      NULL   Civilité
gresp        co\_degre         smallint  NULL  NO   PRI 2      Code du degré
gresp\_dafor co\_resp          int       NULL  NO   PRI NULL   Code responsable
gresp\_dafor id\_resp          char      5     YES      NULL   Identifiant du responsable
gresp\_dafor qualite           char      60    YES      NULL   Qualité du responsable (fonction)
gresp\_dafor num\_voie         char      4     YES      NULL   Numéro de la voie
gresp\_dafor nat\_voie         char      4     YES      NULL   Nature de la voie
gresp\_dafor des\_voie         char      20    YES      NULL   adresse
gresp\_dafor esc\_res          char      30    YES      NULL   escalier - résidence
gresp\_dafor cp                char      5     YES      NULL   code postal
gresp\_dafor vil               char      30    YES      NULL   Ville
gresp\_dafor tel               char      14    YES      NULL   Téléphone
gresp\_dafor fax               char      14    YES      NULL   fax
gresp\_dafor mel               char      60    YES      NULL   mail
gresp\_dafor organ             char      60    YES      NULL   Organisme
gresp\_dafor nomu              char      20    YES      NULL   Nom d usage
gresp\_dafor prenom            char      15    YES      NULL   Prénom de la personne
gresp\_dafor civilite          char      1     YES      NULL   Civilité
gresp\_dafor co\_degre         smallint  NULL  NO   PRI 2      Code du degré
gresp\_plus  co\_resp          int       NULL  NO   PRI NULL   Code responsable
gresp\_plus  resp\_dafor       varchar   10    NO       NULL   Initiales du conseiller responsable
gresp\_plus  discipline        varchar   100   NO       NULL   Discipline
gresp\_plus  decharge          decimal   NULL  NO       0.0    Nombre d heures de décharge
gresp\_plus  grade             varchar   100   NO       NULL   Certifié, agrégé,...
gresp\_plus  divers            varchar   1000  NO       NULL   Infos diverses
ncont        co\_cont          char      3     NO   PRI NULL   Code contenu
ncont        lib\_court        char      10    YES      NULL   Libellé court
ncont        lib\_long         char      50    YES      NULL   Libellé long
ncont        dt\_deb           date      NULL  YES      NULL   Date de début
ncont        dt\_fin           date      NULL  YES      NULL   Date de fin
netab        co\_lieu          char      8     NO   PRI NULL   Code lieu
netab        co\_natu          char      3     YES      NULL   Code nature
netab        co\_cate          char      4     YES      NULL   Code catégorie
netab        secteur           char      2     YES      NULL   Secteur
netab        sigle             char      6     YES      NULL   Sigle de l établissement
netab        denom\_princ      char      30    YES      NULL   Dénomination principale
netab        denom\_comp       char      30    YES      NULL   Dénomination complémentaire
netab        adr               char      200   YES      NULL   Adresse
netab        mention           char      32    YES      NULL   Mention
netab        lieu\_dit         char      24    YES      NULL   Lieu-dit
netab        bp                char      7     YES      NULL   Bureau Postal
netab        cedex             char      2     YES      NULL   Cedex
netab        lieu\_dist        char      26    YES      NULL   Bureau distributeur
netab        cp                char      5     YES      NULL   code postal
netab        pays              char      32    YES      NULL   Pays
netab        tel               char      14    YES      NULL   Téléphone
netab        fax               char      14    YES      NULL   fax
netab        mel               char      60    YES      NULL   mail
netab        co\_acad          char      3     YES      NULL   Code de l académie
netab        co\_comm          char      6     YES      NULL   Code commune
netab        mdp               char      8     YES      NULL   Mot de passe
netab        co\_circ          char      8     YES      NULL   Code circonscription
netab        zeelno            char      5     YES      NULL   Numéro de zone élémentaire
netab        flag\_mel         char      1     YES      NULL   Flag pour envois des Ordres de Mission -OM- par mail (O/N)
netab\_dafor co\_lieu          char      8     NO   PRI NULL   Code lieu
netab\_dafor co\_natu          char      3     YES      NULL   Code nature
netab\_dafor co\_cate          char      4     YES      NULL   Code catégorie
netab\_dafor secteur           char      2     YES      NULL   Secteur
netab\_dafor sigle             char       6    YES      NULL   Sigle de l établissement
netab\_dafor denom\_princ      char      30    YES      NULL   Dénomination principale
netab\_dafor denom\_comp       char      30    YES      NULL   Dénomination complémentaire
netab\_dafor adr               char      200   YES      NULL   Adresse
netab\_dafor mention           char      32    YES      NULL   Mention
netab\_dafor lieu\_dit         char      24    YES      NULL   Lieu-dit
netab\_dafor bp                char      7     YES      NULL   Bureau Postal
netab\_dafor cedex             char      2     YES      NULL   Cedex
netab\_dafor lieu\_dist        char      26    YES      NULL   Bureau distributeur
netab\_dafor cp                char      5     YES      NULL   code postal
netab\_dafor pays              char      3     YES      NULL   Pays
netab\_dafor tel               char      14    YES      NULL   Téléphone
netab\_dafor fax               char      14    YES      NULL   fax
netab\_dafor mel               char      60    YES      NULL   mail
netab\_dafor co\_acad          char      3     YES      NULL   Code de l académie
netab\_dafor co\_comm          char      6     YES      NULL   Code commune
netab\_dafor mdp               char      8     YES      NULL   Mot de passe
netab\_dafor co\_circ          char      8     YES      NULL   Code circonscription
netab\_dafor zeelno            char      5     YES      NULL   Numéro de zone élémentaire
netab\_dafor flag\_mel         char      1     YES      NULL   Flag pour envois des Ordres de Mission -OM- par mail (O/N)
norie        co\_orie          char      4     NO   PRI NULL   Code nomenclature locale d orientation
norie        lib\_court        char      10    YES      NULL   Libellé court
norie        lib\_long         char      50    YES      NULL   Libellé long
norie        dt\_deb           date      NULL  YES      NULL   Date de début
norie        dt\_fin           date      NULL  YES      NULL   Date de fin
ntcan        co\_tcan          char      1     NO   PRI NULL   Code du type de candidature
ntcan        lib\_court        char      10    YES      NULL   Libellé court
ntcan        lib\_long         char      50    YES      NULL   Libellé long
ntcan        dt\_deb           date      NULL  YES      NULL   Date de début
ntcan        dt\_fin           date      NULL  YES      NULL   Date de fin
session      sess\_id          smallint  NULL  NO   PRI NULL   Identifiant de la session
session      session\_alert    tinyint   NULL  YES      0      Alerte sur la session, service administratif prévenu changt important. 0/ 1
session      en\_at tente      tinyint   NULL  YES      0      O:stage en attente - 1:Demande de prép de convocation pour cette session
session      convoc\_sent      tinyint   NULL  YES      0      La convocation pour cette session a été envoyée. 0/1
session      co\_resp          int       NULL  NO   MUL NULL   Code responsable
session      co\_modu          int       NULL  NO   MUL NULL   Code du module pour un dispositif donné
session      co\_degre         smallint  NULL  NO       2      Code du degré
session      co\_lieu          char      8     NO       NULL   Code lieu
session      groupe            tinyint   NULL  YES      1      Numéro du groupe
session      date              date      NULL  YES  MUL NULL   Date de la session format YYYY-MM-DD
session      date\_ts          timestamp NULL  YES  MUL NULL   Date de la session format nb de secondes depuis 01/01/1970
session      horaires          varchar   50    YES      0      Horaires de la session
session      type\_paiement    varchar   8     YES      VAC    type de paiement du formateur
session      duree\_a\_payer   decimal   NULL  YES      0.00   Nombre d heures à payer
session      duree\_prevue     decimal   NULL  YES      0.00   Nombre d heures prévues
session      uid               int       NULL  NO       NULL   User uid du responsable de la dernière modif
session      date\_modif       datetime  NULL  NO       NULL   Date de dernière modification


Notes :

~~~
CREATE table zzzz as SELECT
 id 'co_resp',
 id 'id_resp',
 NULL 'qualite',
 NULL 'num_voie',
 NULL 'nat_voie',
 NULL 'des_voie',


 NULL 'esc_res',
 NULL 'cp',
 NULL 'vil',
 tel_fixe 'tel',
 NULL 'fax',
 mail 'mel',
 NULL 'org',
 CONCAT(SUBSTRING(formateur,1,LOCATE('-',formateur)-1),SUBSTRING(formateur,1,LOCATE(' ',formateur)-1)) 'nomu',
 SUBSTRING(SUBSTRING(formateur,LOCATE('-',formateur)+1),LOCATE(' ',SUBSTRING(formateur,LOCATE('-',formateur)+1))+1) 'prenom',
 NULL 'civilite',
 NULL 'co_degre'
from gbb_gresp_dafor
~~~