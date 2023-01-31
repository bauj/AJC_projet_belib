#!/usr/bin/python3

import urllib3
import ujson
import sqlite3


def iterator_data(n, raw_data_all_bornes):
    for i in range(n):
        yield raw_data_all_bornes[i]["last_updated"],\
                raw_data_all_bornes[i]["id_pdc"], \
                raw_data_all_bornes[i]["statut_pdc"], \
                raw_data_all_bornes[i]["adresse_station"],\
                f"({raw_data_all_bornes[i]['coordonneesxy']['lon']},\
                   {raw_data_all_bornes[i]['coordonneesxy']['lat']})" 

http = urllib3.PoolManager()

resp = http.request("GET", "https://parisdata.opendatasoft.com/api/explore/v2.1/catalog/datasets/belib-points-de-recharge-pour-vehicules-electriques-disponibilite-temps-reel/exports/json?lang=fr&timezone=Europe%2FParis")

raw_data_all_bornes = ujson.loads(resp.data) 
nb_bornes = len(raw_data_all_bornes) 

wanted_keys = ["last_updated", "id_pdc", "statut_pdc", "adresse_station", "coordonneesxy"]

#filtered_data_all_bornes = []
#for borne in raw_data_all_bornes:
#    filtered_data_all_bornes.append({key: borne[key] for key in wanted_keys})
#
#print(filtered_data_all_bornes)

print(list(iterator_data(1, raw_data_all_bornes)))
conn = sqlite3.connect("belib_data.db")
cur = conn.cursor()
insert_query = "INSERT INTO Bornes ("+", ".join(wanted_keys)+") VALUES ("+\
                            ", ".join(len(wanted_keys)*['?']) + ");"
print(insert_query)
cur.executemany(insert_query, iterator_data(nb_bornes, raw_data_all_bornes))

conn.commit()
conn.close()
