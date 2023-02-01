#!/bin/sh
alias arm64gcc='/usr/local/src/buildroot-2022.11-qemu-aarch64-rpi3/output/host/bin/aarch64-buildroot-linux-gnu-gcc'

arm64gcc -Wall -g -O0 -o plot_belib_aarch64.exe src/main.c -lsqlite3 -lgd
./send_exe64.sh
