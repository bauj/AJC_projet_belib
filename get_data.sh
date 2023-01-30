#!/bin/sh

# ===========================================================================
# Script de recuperation des données Belib du jour à l'aide d'une requete de 
# type ODSQL
#
# Author : Juba Hamma
# ===========================================================================

date_recolte=`date +%Y-%m-%dT%H-%MTZD`
date_du_jour=`date +%Y-%m-%d`
date_veille=`date -d "-1 day" +%Y-%m-%d`

# ----------------------------------------------------------------------------
Get_all_data () {
	## On recupere le dataset entier : utile si on veut utiliser plus de data
	## Equiivalent a requete SQL (si il n'y avait pas de limite) :
	## + SELECT : *
	file_output_tot="raw_data_tot_belib_${date_du_jour}.json"
	curl -X 'GET' \
	    "https://parisdata.opendatasoft.com/api/explore/v2.1/catalog/datasets/belib-points-de-recharge-pour-vehicules-electriques-disponibilite-temps-reel/exports/json?lang=fr&timezone=Europe%2FParis" \
	    -H "accept: application/json; charset=utf-8" > $1${file_output_tot}
}

# ----------------------------------------------------------------------------
Get_grouped_by_status_data () {
	## On recupere le resultat de la requete :
	## + SELECT  : count(*) as nb_bornes
	## + WHERE   : last_updated >  date'${date_veille} AND 
	##             last_updated <= date'${date_du_jour}
	## + GROUPBY : statut_pdc
	file_output_nb_bornes="raw_data_nb_bornes_belib_${date_recolte}.json"
	curl -X "GET" \
	  "https://parisdata.opendatasoft.com/api/v2/catalog/datasets/belib-points-de-recharge-pour-vehicules-electriques-disponibilite-temps-reel/records?select=count%28%2A%29%20as%20nb_bornes&where=last_updated%20%3E%20date%27${date_veille}%27%20AND%20last_updated%20%3C%3D%20date%27${date_du_jour}%27&group_by=statut_pdc&limit=100&offset=0&timezone=UTC" \
	  -H "accept: application/json; charset=utf-8" > $1${file_output_nb_bornes}
}

# ----------------------------------------------------------------------------
## Appel des deux fonctions de recuperation des donnees
## L'argument $1 correspond au chemin vers le dossier de sauvegarde des data
## Il est défini afin de pouvoir faire un crontab quelque soit la machine 
## utilisee
Get_all_data $1
Get_grouped_by_status_data $1
