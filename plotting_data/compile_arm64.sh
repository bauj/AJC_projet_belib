#!/bin/sh
alias arm64gcc='/usr/local/src/buildroot-2022.11-qemu-aarch64-rpi3/output/host/bin/aarch64-buildroot-linux-gnu-gcc'

PATH_OVERLAY_BR='/usr/local/src/buildroot-2022.11-qemu-aarch64-rpi3/board/qemu/aarch64-virt/add_stuff/overlay'

arm64gcc -Wall -g -O0 -o plot_belib_aarch64.exe src/main.c -lsqlite3 -lgd -lm
#./send_exe64.sh
cp plot_belib_aarch64.exe ${PATH_OVERLAY_BR}/home/jhamma/bin/.
cp ../db_sqlite/belib_data.db ${PATH_OVERLAY_BR}/home/jhamma/db_sqlite/.
