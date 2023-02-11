#!/bin/sh

PATH_RECUP="/home/jhamma/Documents/test_projetAJC"

cd ${PATH_RECUP}/recuperation_data/
python3 recuperation_data_belib.py --live -a "$1" -d "$2"
cp mapbox_Stations_live.png /var/www/html/figures/.

cd ../plotting_data/
./plot_belib_live.exe ../db_sqlite/belib_data.db
cp figures/fig2_barplot_live.png /var/www/html/figures/.
