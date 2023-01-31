#!/usr/bin/python3

import urllib3
import ujson
import sqlite3
from datetime import date, timedelta, datetime

def iterator_data(n, raw_data_all_bornes):
    for i in range(n):
        yield raw_data_all_bornes[i]["last_updated"],\
                raw_data_all_bornes[i]["id_pdc"], \
                raw_data_all_bornes[i]["statut_pdc"], \
                raw_data_all_bornes[i]["adresse_station"],\
                raw_data_all_bornes[i]['coordonneesxy']['lon'],\
                raw_data_all_bornes[i]['coordonneesxy']['lat']

def update_global(http):

    resp = http.request("GET", "https://parisdata.opendatasoft.com/api/explore/v2.1/catalog/datasets/belib-points-de-recharge-pour-vehicules-electriques-disponibilite-temps-reel/exports/json?lang=fr&timezone=Europe%2FParis")
    
    raw_data_all_bornes = ujson.loads(resp.data) 
    nb_bornes = len(raw_data_all_bornes) 
    
    wanted_keys = ["last_updated", "id_pdc", "statut_pdc", "adresse_station", "lon", "lat"]
    
    conn = sqlite3.connect("belib_data.db")
    cur = conn.cursor()
    insert_query = "INSERT INTO Bornes ("+", ".join(wanted_keys)+") VALUES ("+\
                                ", ".join(len(wanted_keys)*['?']) + ");"
    
    cur.executemany(insert_query, iterator_data(nb_bornes, raw_data_all_bornes))
    
    conn.commit()
    conn.close()

    return

def get_lon_lat_from_coordxy(str_coordxy):
    #'[48.84621396660805, 2.27988394908607]'
    
    lon, lat = str_coordxy.split(",")
    lon = float(lon[1:])
    lat = float(lat[:-1])
    
    return lon, lat

 
def transforme_dict_station(list_records):

    # check si liste non vide

    list_dict_station = []

    # 1ere adresse
    rec0 = list_records[0]["record"]["fields"]

    adresse_record0 = rec0["adresse_station"] 
    lon0,lat0 = get_lon_lat_from_coordxy(rec0["coordonneesxy"])

    dict_station = {"adresse_station" : adresse_record0,
                    "lon" : lon0,
                    "lat" : lat0,
                    }

    for dico_record in list_records:
        rec = dico_record["record"]["fields"]

        if (dict_station["adresse_station"] == rec["adresse_station"]) :
            statut_record = dict_labels[rec['statut_pdc']]
            dict_station[statut_record] = rec["nb_bornes"]
        else :
            list_dict_station.append(dict_station)

            adresse_record = rec["adresse_station"] 
            lon,lat = get_lon_lat_from_coordxy(rec["coordonneesxy"])
            dict_station = {"adresse_station" : adresse_record,
                            "lon" : lon,
                            "lat" : lat,
                            }
            statut_record = dict_labels[rec['statut_pdc']]
            dict_station[statut_record] = rec["nb_bornes"]

    return list_dict_station  

if __name__ == "__main__":

    http = urllib3.PoolManager()
    pos_lat="48.843507"
    pos_long="2.280672"
    dist="0.5km"

    now = datetime.now()
    date_du_jour = date.today().strftime("%Y-%m-%d")
    date_veille = (date.today() - timedelta(days=1)).strftime("%Y-%m-%d")
    date_recolte = datetime.now().strftime("%Y-%m-%dT%H-%MTZD")
    
    url_req = f"https://parisdata.opendatasoft.com/api/v2/catalog/datasets/belib-points-de-recharge-pour-vehicules-electriques-disponibilite-temps-reel/records?select=count%28id_pdc%29%20as%20nb_bornes&where=last_updated%20%3E%20date%27{date_veille}%27%20AND%20last_updated%20%3C%3D%20date%27{date_du_jour}%27%20AND%20distance%28coordonneesxy%2C%20GEOM%27POINT%28{pos_long}%20{pos_lat}%29%27%2C%20{dist}%29&group_by=adresse_station%2C%20coordonneesxy%2Cstatut_pdc&limit=100&offset=0&timezone=UTC"

    resp = http.request("GET", url_req)

    raw_data_stations_pref = ujson.loads(resp.data)["records"] 

    dict_labels = {
        'Disponible'                  :   "disponible"          ,
        'Occupé'                      :   "occupe"              ,
        'Inconnu'                     :   "inconnu"             ,
        'En maintenance'              :   "en_maintenance"      ,
        'Supprimée'                   :   "supprime"            ,
        'Réservée'                    :   "reserve"             ,
        'En cours de mise en service' :   "en_cours_mes"        ,
        'Mise en service planifiée'   :   "mes_planifiee"       ,
        'Pas implémentée'             :   "non_implemente"      ,
    }

    wanted_keys = ["date_recolte",
                   "adresse_station",
                   "lon",
                   "lat"] + list(dict_labels.keys())

    list_stations = transforme_dict_station(raw_data_stations_pref)

    print(list_stations)

    


#{'record': 
#        {'fields': 
#            {'adresse_station': '18 Avenue Emile Zola 75015 Paris', 
#             'coordonneesxy': '[48.84621396660805, 2.27988394908607]', 
#             'statut_pdc': 'Disponible',
#             'nb_bornes': 2
#             }
#        }
#}, 
#{'record': 
#        {'fields': 
#            {'adresse_station': '18 Avenue Emile Zola 75015 Paris', 
#             'coordonneesxy': '[48.84621396660805, 2.27988394908607]',
#             'statut_pdc': 'Occupé (en charge)',
#             'nb_bornes': 1
#             }
#       }
#}
