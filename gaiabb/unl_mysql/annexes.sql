TRUNCATE TABLE `gbb_gmodu_bkp`;
INSERT INTO `gbbdr`.`gbb_gmodu_bkp` SELECT * FROM `gbbdr`.`gbb_gmodu`;

UPDATE `gbb_gmodu` set `co_etab_dem` = (SELECT substring(lcomm, locate('075', lcomm),8) FROM gbb_gmodu_bkp WHERE locate('075', lcomm)>0  AND gbb_gmodu.co_modu = gbb_gmodu_bkp.co_modu ) WHERE gbb_gmodu.co_etab_dem NOT LIKE '075%' OR gbb_gmodu.co_etab_dem IS NULL;
UPDATE `gbb_gmodu` set `co_etab_dem` = (SELECT substring(lautre, locate('075', lautre),8) FROM gbb_gmodu_bkp WHERE locate('075', lautre)>0  AND gbb_gmodu.co_modu = gbb_gmodu_bkp.co_modu ) WHERE gbb_gmodu.co_etab_dem NOT LIKE '075%' OR gbb_gmodu.co_etab_dem IS NULL;
INSERT INTO `gbbdr`.`gbb_norie` (`co_orie`, `lib_court`, `lib_long`, `dt_deb`, `dt_fin`) VALUES ('', 'null', 'null', '2014-06-25', '2014-06-25');
