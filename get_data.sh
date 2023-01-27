#!/bin/sh

# ===========================================================================
# Script de recuperation des données Belib du jour à l'aide d'une requete de 
# type ODSQL
#
# Author : Juba Hamma
# ===========================================================================

# $1 : chemin vers le contenu du dossier git projet AJC

date_du_jour=`date +%Y-%m-%d`
date_veille=`date -d "-1 day" +%Y-%m-%d`
file_output="raw_data_belib_${date_du_jour}.json"

# Limite a 100 records via l'api ...
#curl -X 'GET' \
#    "https://parisdata.opendatasoft.com/api/v2/catalog/datasets/belib-points-de-recharge-pour-vehicules-electriques-disponibilite-temps-reel/records?select=%2A&where=%20last_updated%20%3E%20date%27${date_veille}%27%20AND%20last_updated%20%3C%3D%20date%27${date_du_jour}%27&limit=40&offset=0&timezone=UTC" \
#    -H 'accept: application/json; charset=utf-8' > $1${file_output}

# On recupere le full 
curl -X 'GET' \
    "https://parisdata.opendatasoft.com/api/explore/v2.1/catalog/datasets/belib-points-de-recharge-pour-vehicules-electriques-disponibilite-temps-reel/exports/json?lang=fr&timezone=Europe%2FParis" \
    -H 'accept: application/json; charset=utf-8' > $1${file_output}
