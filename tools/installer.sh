#!/bin/bash

######################################
#  DSCore Installer written by Deep  #
######################################

host="localhost"
user="root"
pass=""
db="world"


while [ "1" = "1" ]; do
	clear
	echo -e "########################"
	echo -e "### DSCORE Installer ###"
	echo -e "########################\n"

	echo -e "Source Update <1> "
	echo -e "Core Compile <2> "
	echo -e "DB Update <3>"

	read -p "" -n1 opt

	clear

	if [ "$opt" = "1" ]; then
		git pull
		echo -e "\nUpdate erfolreich!"
		read -p "Weiter mit beliebiger Taste" -n1
	fi

	if [ "$opt" = "2" ]; then
		read -p "absoluten Pfad zum Realm Dir eingeben:" path
		mkdir ../build
		cd ../build
		cmake ../dscore -DPREFIX=$path
		cd ../dscore
		make && make install
		echo -e "\nRealm Update erfolgreich!"
		read -p "Weiter mit beliebiger Taste" -n1
	fi

	if [ "$opt" = "3" ]; then
		cd sql/updates/world
		mkdir old_sqls
		cat *.sql > ../all_world.sql
		mv *.sql > old_sqls
		cd ..
		mysql -h$host -u$user -p$pass $db < all_world.sql
		rm -r all_world.sql
		echo -e "DB Update erfolgreich!"
		read -p "Weiter mit beliebiger Taste" -n1
	fi
done