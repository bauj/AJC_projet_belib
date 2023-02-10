#!/bin/sh

# ===========================================================================
# Script de mise a jour du serveur httpd (images vitrine) des data belib.
# Author : Juba Hamma, 2023.
# 
# ===========================================================================

ddj=`date "+%d/%M/%Y %H:%m"`
export PATH_BELIB_DB='/var/db_belib/belib_data.db'
export PATH_BELIB_BIN='/usr/bin/plot_belib'

# Recuperation des donnees stations favoris open data paris :
echo "> Date update : ${ddj}"
echo "> Recuperation des data et stockage dans db ..."
${PATH_BELIB_BIN}/recup_data_belib.py --favoris  

# Fabrication des images
echo "> Creation des figures a la racine du serveur httpd ..."
${PATH_BELIB_BIN}/plot_belib_aarch64.exe ${PATH_BELIB_DB}


echo "> Update done"
