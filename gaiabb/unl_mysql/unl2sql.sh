#!/bin/bash
# usage : $ ./UNLcleaner FILE N
# Creation du fichier FILE.SQL depuis FILE.unl possedant N champs

# Conversion UTF8 -> latin1
iconv -f UTF-8 -t ISO-8859-15 $1.unl -o $1.unl_cleaned
# Suppression des espaces multiples et echappement des quotes
sed -i "s/ \( \)*/ /g" $1.unl_cleaned
sed -i "s/'/\\\'/g" $1.unl_cleaned

if [ "$2" == "28" ] || [ "$2" == "39" ]; then  # il s'agit d'un fichier gmodu ou gdisp
# Remplace 12345|12A100056 par A_A_A_A12345|12A100056
sed -i "s/^-*\([0-9]\{1,5\}|[0-9A-Z][0-9A-Z]*|\)/A_A_A_A\1/" $1.unl_cleaned
# deux cas particuliers (bizarreries gaia)
sed -i "s/^\(4885| |SELENE\)/A_A_A_A\1/" $1.unl_cleaned
sed -i "s/^\(1036| |SELENE\)/A_A_A_A\1/" $1.unl_cleaned

sed -i "s/|$//g" $1.unl_cleaned					# remplace |\n par \n
tr '\012' ' ' < $1.unl_cleaned > $1.cl2				# supprime les \n 
sed -i 's/A_A_A_A/\o174\n/g' $1.cl2				# remplace AAAA par |\n
sed '1d' $1.cl2 > $1.unl_cleaned
echo "|" >> $1.unl_cleaned
fi

# Affichage du resultat et fichier de log
awk -F'|' '{ print "ligne " NR " a " NF " champs" }' $1.unl_cleaned > $1.log

# Creation du fichier d'injection SQL
cut -d'|' -f1-$2 --output-delimiter="','" $1.unl_cleaned > $1.SQL
sed -i "s/^/INSERT INTO gbb_$1 VALUES ('/" $1.SQL
sed -i "s/$/');/" $1.SQL
sed -i "s/INSERT INTO $1 VALUES ('','');//" $1.SQL
echo "TRUNCATE TABLE gbb_$1 ;" > $1.SQL2
cat $1.SQL >> $1.SQL2
mv $1.SQL2 $1.SQL
rm $1.unl_cleaned 
