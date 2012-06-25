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
