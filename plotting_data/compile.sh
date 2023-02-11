#!/bin/sh
gcc -Wall -g -O0 -o plot_belib.exe src/main_stations_fav.c -lsqlite3 -lgd -lm
