#!/bin/bash
# usage : $ ./BB-logs.sh FILE CHAMPS_ATTENDUS CHAMPS POIDS_MIN POIDS
# Verification de validite de FILE.SQL

HOME="/var/www/drupal-7.14/sites/default/modules/gaiabb/unl_mysql"

if [ "$2" != "$3" ] || [ $4 -ge $5 ] ; then
  echo "ERROR"
  echo "`date +"%F %T"` ; $1 ; Erreur ; Nb champs : $2 = $3 ; Poids : $4 < $5"  >>  $HOME/ETAT.log
else
  echo "`date +"%F %T"` ; $1 ; OK ; Nb champs : $2 = $3 ; Poids : $4 < $5"  >>  $HOME/ETAT.log
fi
