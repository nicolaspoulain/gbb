#!/bin/sh
opt=`getopt :ngme: $*`; statut=$?
# Si une option invalide a été trouvée
echo "Usage: `basename $0` [-n nettoyage] [-g getFPT] [-m mysqlInjections]"

HOME="/var/www/drupal-7.14/sites/default/modules/gaiabb/unl_mysql"
BDD="drup"
source $HOME/BB-include.sh

GDISP="gdisp"
GMODU="gmodu"
GRESP="gresp"
GDIRE="gdire"
NCONT="ncont"
NTCAN="ntcan"
NORIE="norie"

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
  -g) ## recuperation des donnees par ftp**G**et
	echo "_G_et FTP des donnees"
	/usr/bin/ncftpget -R -u bgfc -p bgfc@bgfc ftpweb8.scola.ac-paris.fr $HOME/ /ftpbgfc/$GDISP.unl
	/usr/bin/ncftpget -R -u bgfc -p bgfc@bgfc ftpweb8.scola.ac-paris.fr $HOME/ /ftpbgfc/$GMODU.unl
	/usr/bin/ncftpget -R -u bgfc -p bgfc@bgfc ftpweb8.scola.ac-paris.fr $HOME/ /ftpbgfc/$GDIRE.unl
	/usr/bin/ncftpget -R -u bgfc -p bgfc@bgfc ftpweb8.scola.ac-paris.fr $HOME/ /ftpbgfc/$GRESP.unl
	/usr/bin/ncftpget -R -u bgfc -p bgfc@bgfc ftpweb8.scola.ac-paris.fr $HOME/ /ftpbgfc/$NCONT.unl
	/usr/bin/ncftpget -R -u bgfc -p bgfc@bgfc ftpweb8.scola.ac-paris.fr $HOME/ /ftpbgfc/$NTCAN.unl
	/usr/bin/ncftpget -R -u bgfc -p bgfc@bgfc ftpweb8.scola.ac-paris.fr $HOME/ /ftpbgfc/$NORIE.unl
    shift;;
  -m) ## Injections **M**YSQL après amélioration des données
	echo "Injections _M_YSQL après amélioration des données"

	echo -ne "* $GMODU.unl en traitement par unl2sql ...\t\t"
	$HOME/unl2sql.sh $GMODU 39
	echo "fait."
	NB_CHAMPS=`cat $GMODU.log | cut -d' ' -f4 | uniq`
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
	NB_CHAMPS=`cat $GDISP.log | cut -d' ' -f4 | uniq`
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
	NB_CHAMPS=`cat $GRESP.log | cut -d' ' -f4 | uniq`
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
	NB_CHAMPS=`cat $GDIRE.log | cut -d' ' -f4 | uniq`
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
	NB_CHAMPS=`cat $NCONT.log | cut -d' ' -f4 | uniq`
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
	NB_CHAMPS=`cat $NTCAN.log | cut -d' ' -f4 | uniq`
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
	NB_CHAMPS=`cat $NORIE.log | cut -d' ' -f4 | uniq`
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
    tail -n 7 ETAT.log
    shift;;
  --) # Fin des options - Sortie de boucle
   shift; break;;
esac
done
# Affichage du reste des paramètres s'il y en a
test $# -ne 0 && echo "Il reste encore $# paramètres qui sont $*" || echo "Il n'y a plus de paramètre"

