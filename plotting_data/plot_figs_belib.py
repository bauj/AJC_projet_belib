#!/usr/bin/python3

# ===========================================================================
# Script de plotting de figures illustrant les donnees de la db sqlite belib.
# Author : Juba Hamma.
# ===========================================================================

import matplotlib as mpl
import sqlite3 as sql3

path_to_db = "../traitement_data/sqlitedb/"
db_filename = "records_belib.db"

con = sql3.connect(path_to_db+db_filename)
cur = con.cursor()
cur.execute("SELECT * FROM Records")
names = list(map(lambda x: x[0], cur.description))
result = cur.fetchall()

print(names)
print(*result, sep="\n")

