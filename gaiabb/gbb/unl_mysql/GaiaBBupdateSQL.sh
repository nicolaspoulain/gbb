#!/bin/bash
opt=`getopt :nspgmre: $*`; statut=$?
# Si une option invalide a été trouvée
echo "Usage: `basename $0` [-n nettoyage] [-s Sauvegardes locales] [-p Put FTP des sauvegardes] [-g getFPT] [-m mysqlInjections] [-r rapport par email]"

HOME="/var/www/gbbdr/sites/default/modules/gbb/unl_mysql"
BACKUPANDMIGRATE="/var/www/gbbdr/sites/default/files/private/backup_migrate/*"
BDD="gbbdr"
source $HOME/BB-include.sh

touch $HOME/TOUCH

GDISP="gdisp"
GMODU="gmodu"
GRESP="gresp"
GDIRE="gdire"
NCONT="ncont"
NTCAN="ntcan"
NORIE="norie"
GDIOF="gdiof"
GMOOF="gmoof"

dow=`date +%u` ; dom=`date +%d`
jour=`date +%A`
date=`date +%F--%H-%M`

if test $statut -ne 0; then
	echo "Pour tout lancer : ./`basename $0` -ngm"
	exit $statut
fi

# Traitement de chaque option du script
set -- $opt
while true
do
# Analyse de l'option reçue - Chaque option sera effacée ensuite via un "shift"
case "$1" in
  -n) ## **N**ettoyage
	echo "_N_etttoyage du dossier"
	cd $HOME && rm *.unl *.SQL *.cl2
    shift;;
  -s) ## **S**auvegardes locales (drupal-jour.tgz est un backup complet)
	echo "S_auvegardes"
	/usr/bin/mysqldump --user=root --password=$BDDPW $BDD > $HOME/../$BDD-$jour.sql
	/bin/tar czfv $HOME/drupal-$jour.tgz $DIR --exclude=$HOME --exclude=$BACKUPANDMIGRATE > /dev/null
	mv $HOME/../$BDD-$jour.sql $HOME/
    shift;;
  -p) ## upload par ftp**P**ut du backup quotidien, et hebdo
	echo "P_ut FTP des sauvegardes"
	/usr/bin/ncftpput -u bgfc -p bgfc@bgfc ftpweb8.scola.ac-paris.fr /ftpbgfc/ $HOME/drupal-$jour.tgz
	if [[ $dow == 4 ]]
	then
	    cp $HOME/drupal-$jour.tgz $HOME/drupal-$date.tgz
	    /usr/bin/ncftpput -u bgfc -p bgfc@bgfc ftpweb8.scola.ac-paris.fr /ftpbgfc/ $HOME/drupal-$date.tgz
	fi
    shift;;
  -g) ## recuperation des donnees par ftp**G**et
	echo "_G_et FTP des donnees"
	/usr/bin/ncftpget -R -u bgfc -p bgfc@bgfc ftpweb8.scola.ac-paris.fr $HOME/ /ftpbgfc/$GDISP.unl
	/usr/bin/ncftpget -R -u bgfc -p bgfc@bgfc ftpweb8.scola.ac-paris.fr $HOME/ /ftpbgfc/$GMODU.unl
	/usr/bin/ncftpget -R -u bgfc -p bgfc@bgfc ftpweb8.scola.ac-paris.fr $HOME/ /ftpbgfc/$GDIRE.unl
	/usr/bin/ncftpget -R -u bgfc -p bgfc@bgfc ftpweb8.scola.ac-paris.fr $HOME/ /ftpbgfc/$GRESP.unl
	/usr/bin/ncftpget -R -u bgfc -p bgfc@bgfc ftpweb8.scola.ac-paris.fr $HOME/ /ftpbgfc/$NCONT.unl
	/usr/bin/ncftpget -R -u bgfc -p bgfc@bgfc ftpweb8.scola.ac-paris.fr $HOME/ /ftpbgfc/$NTCAN.unl
	/usr/bin/ncftpget -R -u bgfc -p bgfc@bgfc ftpweb8.scola.ac-paris.fr $HOME/ /ftpbgfc/$NORIE.unl
	/usr/bin/ncftpget -R -u bgfc -p bgfc@bgfc ftpweb8.scola.ac-paris.fr $HOME/ /ftpbgfc/$GDIOF.unl
	/usr/bin/ncftpget -R -u bgfc -p bgfc@bgfc ftpweb8.scola.ac-paris.fr $HOME/ /ftpbgfc/$GMOOF.unl
    cd $HOME && echo `ls -alh *.unl`
    shift;;
  -r) ## **R**apport par mail
  if grep -q "Erreur" $HOME/LOG.log
    then
      /usr/bin/mail -s "$(echo -e "ECHEC de la mise à jour BB")" nico.poulain@gmail.com azouz.manai@ac-paris.fr catherine.laigle@ac-paris.fr michel.denise@ac-paris.fr christian.muir@ac-paris.fr < $HOME/LOG.log
    else
      /usr/bin/mail -s "$(echo -e "SUCCÈS de la mise à jour BB")" nico.poulain@gmail.com azouz.manai@ac-paris.fr catherine.laigle@ac-paris.fr michel.denise@ac-paris.fr christian.muir@ac-paris.fr < $HOME/LOG.log
    fi

    shift;;
  -m) ## Injections **M**YSQL après amélioration des données
	echo "Injections _M_YSQL après amélioration des données"

	echo -ne "* $GMODU.unl en traitement par unl2sql ...\t\t"
	$HOME/unl2sql.sh $GMODU 39
	echo "fait."
	NB_CHAMPS=`/bin/cat $HOME/$GMODU.log |/usr/bin/cut -d' ' -f4 |/usr/bin/uniq`
	POIDS=$(/usr/bin/du $HOME/$GMODU.SQL | /usr/bin/cut -f1)
	TEST=`$HOME/unlChecker.sh $GMODU 40 $NB_CHAMPS 23900 $POIDS`
	if [[ "$TEST" = "ERROR" ]]; then 
      echo -e "\t\t\t\t\t\t\t$TEST"
	else
	  echo -ne "* $GMODU.SQL en cours d'injection  .....\t\t\t"
	  /usr/bin/mysql --user=root --password=$BDDPW $BDD < $HOME/$GMODU.SQL
	  echo -e "fait."
	  echo -e "\t\t\t\t\t\t\t$TEST"
	fi

	echo -ne "* $GDISP.unl en traitement par unl2sql ...\t\t"
	$HOME/unl2sql.sh $GDISP 28
	echo "fait."
	NB_CHAMPS=`/bin/cat $HOME/$GDISP.log |/usr/bin/cut -d' ' -f4 |/usr/bin/uniq`
	POIDS=$(/usr/bin/du $HOME/$GDISP.SQL | /usr/bin/cut -f1)
	TEST=`$HOME/unlChecker.sh $GDISP 29 $NB_CHAMPS 8000 $POIDS`
	if [[ "$TEST" = "ERROR" ]]; then 
	  echo -e "\t\t\t\t\t\t\t$TEST"
	else
	  echo -ne "* $GDISP.SQL en cours d'injection  .....\t\t\t"
	  /usr/bin/mysql --user=root --password=$BDDPW $BDD < $HOME/$GDISP.SQL
	  echo -e "fait."
	  echo -e "\t\t\t\t\t\t\t$TEST"
	fi

	echo -ne "* $GRESP.unl en traitement par unl2sql ...\t\t"
	$HOME/unl2sql.sh $GRESP 17
	echo "fait."
	NB_CHAMPS=`/bin/cat $HOME/$GRESP.log |/usr/bin/cut -d' ' -f4 |/usr/bin/uniq`
	POIDS=$(/usr/bin/du $HOME/$GRESP.SQL | /usr/bin/cut -f1)
	TEST=`$HOME/unlChecker.sh $GRESP 18 $NB_CHAMPS 960 $POIDS`
	if [ "$TEST" == "ERROR" ]; then 
	  echo -e "\t\t\t\t\t\t\t$TEST"
	else
	  echo -ne "* $GRESP.SQL en cours d'injection  .....\t\t\t"
	  /usr/bin/mysql --user=root --password=$BDDPW $BDD < $HOME/$GRESP.SQL
	  echo -e "fait."
	  echo -e "\t\t\t\t\t\t\t$TEST"
	fi

	echo -ne "* $GDIRE.unl en traitement par unl2sql ...\t\t"
	$HOME/unl2sql.sh $GDIRE 5
	echo "fait."
	NB_CHAMPS=`/bin/cat $HOME/$GDIRE.log |/usr/bin/cut -d' ' -f4 |/usr/bin/uniq`
	POIDS=$(/usr/bin/du $HOME/$GDIRE.SQL | /usr/bin/cut -f1)
	TEST=`$HOME/unlChecker.sh $GDIRE 6 $NB_CHAMPS 2650 $POIDS`
	if [ "$TEST" == "ERROR" ]; then 
	  echo -e "\t\t\t\t\t\t\t$TEST"
	else
	  echo -ne "* $GDIRE.SQL en cours d'injection  .....\t\t\t"
	  /usr/bin/mysql --user=root --password=$BDDPW $BDD < $HOME/$GDIRE.SQL
	  echo -e "fait."
	  echo -e "\t\t\t\t\t\t\t$TEST"
	fi

	echo -ne "* $NCONT.unl en traitement par unl2sql ...\t\t"
	$HOME/unl2sql.sh $NCONT 5
	echo "fait."
	NB_CHAMPS=`/bin/cat $HOME/$NCONT.log |/usr/bin/cut -d' ' -f4 |/usr/bin/uniq`
	POIDS=$(/usr/bin/du $HOME/$NCONT.SQL | /usr/bin/cut -f1)
	TEST=`$HOME/unlChecker.sh $NCONT 6 $NB_CHAMPS 11 $POIDS`
	if [ "$TEST" == "ERROR" ]; then 
	  echo -e "\t\t\t\t\t\t\t$TEST"
	else
	  echo -ne "* $NCONT.SQL en cours d'injection  .....\t\t\t"
	  /usr/bin/mysql --user=root --password=$BDDPW $BDD < $HOME/$NCONT.SQL
	  echo -e "fait."
	  echo -e "\t\t\t\t\t\t\t$TEST"
	fi

	echo -ne "* $NTCAN.unl en traitement par unl2sql ...\t\t"
	$HOME/unl2sql.sh $NTCAN 5
	echo "fait."
	NB_CHAMPS=`/bin/cat $HOME/$NTCAN.log |/usr/bin/cut -d' ' -f4 |/usr/bin/uniq`
	POIDS=$(/usr/bin/du $HOME/$NTCAN.SQL | /usr/bin/cut -f1)
	TEST=`$HOME/unlChecker.sh $NTCAN 6 $NB_CHAMPS 3 $POIDS`
	if [ "$TEST" == "ERROR" ]; then 
	  echo -e "\t\t\t\t\t\t\t$TEST"
	else
	  echo -ne "* $NTCAN.SQL en cours d'injection  .....\t\t\t"
	  /usr/bin/mysql --user=root --password=$BDDPW $BDD < $HOME/$NTCAN.SQL
	  echo -e "fait."
	  echo -e "\t\t\t\t\t\t\t$TEST"
	fi

	echo -ne "* $NORIE.unl en traitement par unl2sql ...\t\t"
	$HOME/unl2sql.sh $NORIE 5
	echo "fait."
	NB_CHAMPS=`/bin/cat $HOME/$NORIE.log |/usr/bin/cut -d' ' -f4 |/usr/bin/uniq`
	POIDS=$(/usr/bin/du $HOME/$NORIE.SQL | /usr/bin/cut -f1)
	TEST=`$HOME/unlChecker.sh $NORIE 6 $NB_CHAMPS 39 $POIDS`
	if [ "$TEST" == "ERROR" ]; then 
	  echo -e "\t\t\t\t\t\t\t$TEST"
	else
	  echo -ne "* $NORIE.SQL en cours d'injection  .....\t\t\t"
	  /usr/bin/mysql --user=root --password=$BDDPW $BDD < $HOME/$NORIE.SQL
	  echo -e "fait."
	  echo -e "\t\t\t\t\t\t\t$TEST"
	fi

	echo -ne "* $GDIOF.unl en traitement par unl2sql ...\t\t"
	$HOME/unl2sql.sh $GDIOF 25
	echo "fait."
	NB_CHAMPS=`/bin/cat $HOME/$GDIOF.log |/usr/bin/cut -d' ' -f4 |/usr/bin/uniq`
	POIDS=$(/usr/bin/du $HOME/$GDIOF.SQL | /usr/bin/cut -f1)
	TEST=`$HOME/unlChecker.sh $GDIOF 26 $NB_CHAMPS 3 $POIDS`
	if [ "$TEST" == "ERROR" ]; then 
	  echo -e "\t\t\t\t\t\t\t$TEST"
	else
	  echo -ne "* $GDIOF.SQL en cours d'injection  .....\t\t\t"
	  /usr/bin/mysql --user=root --password=$BDDPW $BDD < $HOME/$GDIOF.SQL
	  echo -e "fait."
	  echo -e "\t\t\t\t\t\t\t$TEST"
	fi

	echo -ne "* $GMOOF.unl en traitement par unl2sql ...\t\t"
	$HOME/unl2sqlGMOOF.sh $GMOOF 38
	echo "fait."
	NB_CHAMPS=`/bin/cat $HOME/$GMOOF.log |/usr/bin/cut -d' ' -f4 |/usr/bin/uniq`
	POIDS=$(/usr/bin/du $HOME/$GMOOF.SQL | /usr/bin/cut -f1)
	TEST=`$HOME/unlChecker.sh $GMOOF 39 $NB_CHAMPS 3 $POIDS`
	if [ "$TEST" == "ERROR" ]; then 
	  echo -e "\t\t\t\t\t\t\t$TEST"
	else
	  echo -ne "* $GMOOF.SQL en cours d'injection  .....\t\t\t"
	  /usr/bin/mysql --user=root --password=$BDDPW $BDD < $HOME/$GMOOF.SQL
	  echo -e "fait."
	  echo -e "\t\t\t\t\t\t\t$TEST"
	fi

    echo "Rapport du jour" > LOG.log
    echo "===============" >> LOG.log
    echo " " >> LOG.log
    echo "Exécution ; FichierUnl ; Réussite ;" >> LOG.log
    echo "  Date: duFichierUnl = attendu; " >> LOG.log
    echo "  NbChamps: duFichierUnl = attendu ;" >> LOG.log
    echo "  Poids: duFichierUnl > minimum attendu" >> LOG.log
    echo " " >> LOG.log
    /usr/bin/tail -n 7 ETAT.log >> LOG.log
    shift;;
  --) # Fin des options - Sortie de boucle
   shift; break;;
esac
done
# Affichage du reste des paramètres s'il y en a
test $# -ne 0 && echo "Il reste encore $# paramètres qui sont $*" || echo "Il n'y a plus de paramètre"

