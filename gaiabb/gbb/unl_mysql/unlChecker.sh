#!/bin/bash
# usage : $ ./BB-logs.sh FILE CHAMPS_ATTENDUS CHAMPS POIDS_MIN POIDS
# Verification de validite de FILE.SQL

HOME="/var/www/gbbdr/sites/default/modules/gbb/unl_mysql"
X=`date -r $HOME/$1.unl +'%F'`
Y=`date +'%F'`

if [ "$2" != "$3" ] || [ $4 -ge $5 ] || [ $X != $Y ] ; then
  echo "ERROR"
  echo "`date +"%F %R"` ; $1 ; Erreur ; Date: `date -r $HOME/$1.unl +'%d/%m'`=`date +'%d/%m'` ; Nb champs: $3=$2 ; Poids: $5>$4"  >>  $HOME/ETAT.log
else
  echo "`date +"%F %R"` ; $1 ; OK ; Date: `date -r $HOME/$1.unl +'%d/%m'`=`date +'%d/%m'` ; Nb champs: $3=$2 ; Poids: $5>$4"  >>  $HOME/ETAT.log
fi
