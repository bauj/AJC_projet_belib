#!/bin/sh

# ===========================================================================
# Script de recuperation des données Belib du jour à l'aide d'une requete de 
# type ODSQL
#
# Author : Juba Hamma
# ===========================================================================

date_du_jour=`date +%Y-%m-%d`
date_veille=`date -d "-1 day" +%Y-%m-%d`
file_output="raw_data_belib_${date_du_jour}.json"

curl -X 'GET' \
    "https://parisdata.opendatasoft.com/api/v2/catalog/datasets/belib-points-de-recharge-pour-vehicules-electriques-disponibilite-temps-reel/records?select=last_updated%2Cid_pdc%2Cadresse_station&where=%20last_updated%20%3E%20date%27${date_veille}%27%20AND%20last_updated%20%3C%3D%20date%27${date_du_jour}%27&limit=40&offset=0&timezone=UTC" \
    -H 'accept: application/json; charset=utf-8' > ${file_output}
