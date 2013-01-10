scp nico@www.ent-paris.fr:/home/nico/drup*-mardi* .
tar zxf drupal-mardi.tgz
mysql -u root -p drupal_OVH < /home/nico/drup-mardi.sql 
sudo cp var/www/drupal-7.14/sites/default/files/* /var/www/gbbdr/sites/default/files/ 
sudo chmod www-data.www-data /var/www/gbbdr/sites/default/files/*





insert into gbbdr.gbb_gdire select * from drupal_OVH.gbb_gdire;
insert into gbbdr.gbb_gdisp select * from drupal_OVH.gbb_gdisp;
insert into gbbdr.gbb_gmodu select * from drupal_OVH.gbb_gmodu;
insert into gbbdr.gbb_gresp select * from drupal_OVH.gbb_gresp;
insert into gbbdr.gbb_ncont select * from drupal_OVH.gbb_ncont;
insert into gbbdr.gbb_norie select * from drupal_OVH.gbb_norie;
insert into gbbdr.gbb_ntcan select * from drupal_OVH.gbb_ntcan;




TRUNCATE TABLE gbb_netab_dafor;
insert into gbbdr.gbb_netab_dafor select * from drupal_OVH.gbb_netab;
TRUNCATE TABLE gbb_gresp_dafor;
insert into gbbdr.gbb_gresp_dafor select * from drupal_OVH.gbb_gresp_dafor;
TRUNCATE TABLE users;
insert into gbbdr.users select * from drupal_OVH.users;
TRUNCATE TABLE og_users_roles;
insert into gbbdr.og_users_roles select * from drupal_OVH.og_users_roles;
TRUNCATE TABLE og_membership;
insert into gbbdr.og_membership select * from drupal_OVH.og_membership;
TRUNCATE TABLE users_roles;
insert into gbbdr.users_roles select * from drupal_OVH.users_roles;
TRUNCATE TABLE file_managed;
insert into gbbdr.file_managed select * from drupal_OVH.file_managed;
TRUNCATE TABLE gbb_file;
insert into gbbdr.gbb_file select * from drupal_OVH.gbb_file;
TRUNCATE TABLE gbb_gmodu_plus;
insert into gbbdr.gbb_gmodu_plus
( `co_modu`,     `co_degre`,       `module_alert`,
  `prioritaire`, `module_printed`, `organisation`,
  `CDC_etat`, `convoc_info_off`, `convoc_info_on`,
  `LE_etat`, `LE_info`, `DSF_etat`, `DSF_info`,
  `DA_info`, `DA_date_depot`)
select
  `co_modu`,     `co_degre`,       `module_alert`,
  `prioritaire`, `module_printed`, `organisation`,
  `CDC_etat`, `convoc_info_off`, `convoc_info_on`,
  `LE_etat`, `LE_info`, `DSF_etat`, `DSF_info`,
  `DA_info`, `DA_date_depot`
from drupal_OVH.gbb_gmodu_plus ;

TRUNCATE TABLE gbb_gresp_plus;
insert into gbbdr.gbb_gresp_plus select * from drupal_OVH.gbb_gresp_plus;

TRUNCATE TABLE gbb_session;
insert into gbbdr.gbb_session
( `sess_id`, `session_alert`, `en_attente`, `convoc_sent`,
  `co_resp`, `co_modu`, `co_degre`, `co_lieu`,
  `groupe`, `date`, `date_ts`, `horaires`, `type_paiement`, `duree_a_payer`, `duree_prevue`,
  `uid`, `date_modif`)
select
  `sess_id`, `session_alert`, `en_attente`, `convoc_sent`,
  `co_resp`, `co_modu`, `co_degre`, `co_lieu`,
  `groupe`, `date`, UNIX_TIMESTAMP(`date`), `horaires`, `typePaiement`, `dureeapayer`, `dureeprevue`,
  1, NOW()
from drupal_OVH.gbb_session ;

UPDATE gbbdr.gbb_session SET date_ts=UNIX_TIMESTAMP(`date`);
