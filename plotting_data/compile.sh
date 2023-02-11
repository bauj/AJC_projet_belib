#!/bin/sh
gcc -Wall -g -O0 -o plot_belib.exe src/main_stations_fav.c -lsqlite3 -lgd -lm
gcc -Wall -g -O0 -o plot_belib_live.exe src/main_stations_live.c -lsqlite3 -lgd -lm
