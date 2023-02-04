#!/bin/sh
#
#echo "****** Compilation"
#./compile.sh
echo "****** Valgrind"
valgrind -s --default-suppressions=no --leak-check=full --show-leak-kinds=all --track-origins=yes ./plot_belib.exe
