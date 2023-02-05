#!/bin/sh
LD_PRELOAD=libefence.so.0 ./plot_belib.exe ../db_sqlite/belib_data.db
