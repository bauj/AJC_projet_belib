#!/bin/sh

PATH_PROJET='/home/ajc/Projet_AJC/AJC_projet_belib'
PATH_OVERLAY_BR='/usr/local/src/buildroot-2022.11-qemu-aarch64-rpi3/board/qemu/aarch64-virt/add_stuff/overlay'

cp ${PATH_PROJET}/recuperation_data/recuperation_data_belib.py ${PATH_OVERLAY_BR}/usr/bin/plot_belib/. && echo "Recup sent"

cp ${PATH_PROJET}/recuperation_data/adresse_introuvable.png ${PATH_OVERLAY_BR}/var/www/html/figures/. && echo "Img introuvable sent"

cp ${PATH_PROJET}/.mapbox_token ${PATH_OVERLAY_BR}/usr/bin/plot_belib/. && echo "mapbox token sent"

cp ${PATH_PROJET}/plotting_data/plot_belib_aarch64.exe ${PATH_OVERLAY_BR}/usr/bin/plot_belib/. && echo "Plot exe sent"

cp ${PATH_PROJET}/plotting_data/plot_belib_aarch64_live.exe ${PATH_OVERLAY_BR}/usr/bin/plot_belib/. && echo "Plot exe live sent"

cp ${PATH_PROJET}/plotting_data/fonts/* ${PATH_OVERLAY_BR}/usr/share/fonts/truetype/lato/. && echo "Fonts sent"

cp ${PATH_PROJET}/plotting_data/update_server_belib.sh ${PATH_OVERLAY_BR}/usr/bin/plot_belib/. && echo "Serv update script sent"

cp ${PATH_PROJET}/db_sqlite/belib_data.db ${PATH_OVERLAY_BR}/var/db_belib/. && echo "db sent"

