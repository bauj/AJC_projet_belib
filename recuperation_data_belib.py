#!/usr/bin/python3


# ===========================================================================
# Script de récupération des données Belib sur l'OpenDataParis.
# La récupération des données se fait via l'API OpenDataSoft utilisée par 
# OpenDataParis. 
# Nom du dataset : 
# `belib-points-de-recharge-pour-vehicules-electriques-disponibilite-temps-reel`
#
# La base de données `belib_paris.db` est mise à jour après lancement du script :
#
# + Table Bornes : Contient l'ensemble des données des bornes, mise à jour 
# quotidiennement. En tête :
# | ID | last_updated | id_pdc | statut_pdc | adresse_station | lon | lat |
#
# + Table Stations_fav : Contient les données des stations en 
# favoris, mise à jour 3x par jour. En tête :
# | ID | date_recolte | adresse_station | lon | lat | disponible | occupe | ...
# 
# + Table Stations_live : Contient les données des stations les plus proches de 
# la position entrée par un utilisateur. En tête :
#  | ID | date_recolte | adresse_station | lon | lat | disponible | occupe | ...
#
# Author : Juba Hamma. 2023.
# ===========================================================================


import urllib3
import ujson
import sqlite3
import argparse
from datetime import date, timedelta, datetime

# -----------------------------------------------------------------------------
def iterator_data_global(n, raw_data_all_bornes):
    for i in range(n):
        yield \
            raw_data_all_bornes[i]["last_updated"],\
            raw_data_all_bornes[i]["id_pdc"], \
            raw_data_all_bornes[i]["statut_pdc"], \
            raw_data_all_bornes[i]["adresse_station"],\
            raw_data_all_bornes[i]['coordonneesxy']['lon'],\
            raw_data_all_bornes[i]['coordonneesxy']['lat']

# -----------------------------------------------------------------------------
def iterator_data_stations(n, list_stations_pref):
    for i in range(n):
        yield \
            list_stations_pref[i]["date_recolte"],\
            list_stations_pref[i]["adresse_station"], \
            list_stations_pref[i]["lat"], \
            list_stations_pref[i]["lon"], \
            list_stations_pref[i]["disponible"], \
            list_stations_pref[i]["occupe"],\
            list_stations_pref[i]['en_maintenance'],\
            list_stations_pref[i]['inconnu'],\
            list_stations_pref[i]['supprime'],\
            list_stations_pref[i]['reserve'],\
            list_stations_pref[i]['en_cours_mes'],\
            list_stations_pref[i]['mes_planifiee'],\
            list_stations_pref[i]['non_implemente']

# -----------------------------------------------------------------------------              
def create_connection(db_file):
    conn = None
    try:
        conn = sqlite3.connect(db_file)
    except Error as e:
        print(e)

    return conn

# -----------------------------------------------------------------------------
def update_global():

    http = urllib3.PoolManager()

    url_req = \
        "https://parisdata.opendatasoft.com/api/explore/v2.1/catalog/datasets/"+\
        "belib-points-de-recharge-pour-vehicules-electriques-disponibilite-temps-reel/"+\
        "exports/json?lang=fr&timezone=Europe%2FParis"

    resp = http.request("GET", url_req)
    
    raw_data_all_bornes = ujson.loads(resp.data) 
    nb_bornes = len(raw_data_all_bornes) 
    
    wanted_keys = ["last_updated", "id_pdc", "statut_pdc", "adresse_station", "lon", "lat"]
    
    conn = sqlite3.connect("belib_data.db")
    cur = conn.cursor()
    table = "Bornes"
    insert_query = f"INSERT INTO {table} ("+", ".join(wanted_keys)+") VALUES ("+\
                                ", ".join(len(wanted_keys)*['?']) + ");"
    
    cur.executemany(insert_query, iterator_data_global(nb_bornes, raw_data_all_bornes))
    
    conn.commit()
    conn.close()

    return

# -----------------------------------------------------------------------------
def get_lon_lat_from_coordxy(str_coordxy):
    #'[48.84621396660805, 2.27988394908607]'
    
    lon, lat = str_coordxy.split(",")
    lon = float(lon[1:])
    lat = float(lat[:-1])
    
    return lon, lat


# -----------------------------------------------------------------------------
def def_station(daterecolte_, adr, lon, lat, n_dispo, n_occup, n_inc, n_main, \
            n_del=0, n_res=0, n_encours_mes=0, n_mes_plan=0, n_nonimp=0):

    return {"date_recolte": daterecolte_, "adresse_station" : adr,
            "lon" : lon, "lat" : lat, "disponible": n_dispo, "occupe": n_occup,
            "inconnu" : n_inc, "en_maintenance" : n_main, "supprime" : n_del, 
            "reserve" : n_res, "en_cours_mes" : n_encours_mes, 
            "mes_planifiee" : n_mes_plan, "non_implemente" : n_nonimp,
        }

# -----------------------------------------------------------------------------
def transform_dict_station(list_records):

    dict_labels = {
        'Disponible'                  :   "disponible"          ,
        'Occupé (en charge)'          :   "occupe"              ,
        'Inconnu'                     :   "inconnu"             ,
        'En maintenance'              :   "en_maintenance"      ,
        'Supprimée'                   :   "supprime"            ,
        'Réservée'                    :   "reserve"             ,
        'En cours de mise en service' :   "en_cours_mes"        ,
        'Mise en service planifiée'   :   "mes_planifiee"       ,
        'Pas implémentée'             :   "non_implemente"      ,
    }

    now = datetime.now()
    date_recolte = now.strftime("%Y-%m-%dT%H-%MTZD")

    # check si liste non vide

    list_dict_station = []

    # 1ere station : initialisation 1ere adresse
    rec0 = list_records[0]["record"]["fields"]
    adresse_record0 = rec0["adresse_station"] 
    lon0,lat0 = get_lon_lat_from_coordxy(rec0["coordonneesxy"])
    dict_station = def_station(date_recolte, adresse_record0, lon0, lat0, 0, 0, 0, 0)
    statut_record0 = dict_labels[rec0['statut_pdc']]
    dict_station[statut_record0] = rec0["nb_bornes"]

    # Saut du 1er record
    for dico_record in list_records[1:]:
        rec = dico_record["record"]["fields"]

        if (dict_station["adresse_station"] == rec["adresse_station"]) :
            statut_record = dict_labels[rec['statut_pdc']]
            dict_station[statut_record] = rec["nb_bornes"]
        else :
            list_dict_station.append(dict_station)

            adresse_record = rec["adresse_station"] 
            lon,lat = get_lon_lat_from_coordxy(rec["coordonneesxy"])
            dict_station = def_station(date_recolte, adresse_record, lon, lat, 0, 0, 0, 0)

            statut_record = dict_labels[rec['statut_pdc']]
            dict_station[statut_record] = rec["nb_bornes"]

    return list_dict_station  

# -----------------------------------------------------------------------------
def update_bornes_around_pos(table, pos_lat, pos_lon, dist):

    http = urllib3.PoolManager()

    date_du_jour = date.today().strftime("%Y-%m-%d")
    date_veille = (date.today() - timedelta(days=1)).strftime("%Y-%m-%d")
    
    url_req = f"https://parisdata.opendatasoft.com/api/v2/catalog/datasets/"+\
        "belib-points-de-recharge-pour-vehicules-electriques-disponibilite-temps-reel/"+\
            "records?select=count%28id_pdc%29%20as%20nb_bornes&where=last_updated"+\
                f"%20%3E%20date%27{date_veille}%27%20AND%20last_updated%20%3C%3D%20date"+\
                    f"%27{date_du_jour}%27%20AND%20distance%28coordonneesxy%2C%20GEOM"+\
                        f"%27POINT%28{pos_lon}%20{pos_lat}%29%27%2C%20{dist}%29&group_by"+\
                            "=adresse_station%2C%20coordonneesxy%2Cstatut_pdc&limit=100"+\
                                "&offset=0&timezone=UTC"

    resp = http.request("GET", url_req)
    raw_data_stations_pref = ujson.loads(resp.data)["records"] 

    list_stations = transform_dict_station(raw_data_stations_pref)
    nb_stations = len(list_stations)

    wanted_keys = list(list_stations[0].keys())
    
    conn = sqlite3.connect("belib_data.db")
    cur = conn.cursor()
    insert_query = f"INSERT INTO {table} ("+", ".join(wanted_keys)+") VALUES ("+\
                                ", ".join(len(wanted_keys)*['?']) + ");"
    
    cur.executemany(insert_query, iterator_data_stations(nb_stations, list_stations))
    
    conn.commit()

    search_query = f"SELECT MIN(lon),MIN(lat),MAX(lon),MAX(lat) FROM {table}"
    cur.execute(search_query)
    test = list(cur.fetchall()[0])
    print(test)

    conn.close()

    return 

# -----------------------------------------------------------------------------
def adresse_to_lon_lat(adr):

    http = urllib3.PoolManager()

    adr_ = adr.replace(" ", "+")
    centre_paris_lat = 48.52
    centre_paris_lon = 2.19
    url_req = "https://api-adresse.data.gouv.fr/search/?q=" +\
            f"{adr_}&lat={centre_paris_lat}&lon={centre_paris_lon}"

    resp = http.request("GET", url_req)
    raw_data = ujson.loads(resp.data)
    lon, lat = raw_data["features"][0]["geometry"]["coordinates"]

    # test = ujson.dumps(raw_data, indent=4)
    # print(test)

    return lon, lat

# -----------------------------------------------------------------------------
def clean_table(table):

    conn = create_connection("belib_data.db")
    sql_req = f"DELETE FROM {table}"
    with conn :
        cur = conn.cursor()
        cur.execute(sql_req)
        conn.commit()

# -----------------------------------------------------------------------------
def update_bornes_around_adresse_live(adr, dist):

    lon_adr, lat_adr = adresse_to_lon_lat(adr)

    table="Stations_live"
    clean_table(table)
    update_bornes_around_pos(table, lat_adr, lon_adr, dist)
    
    return

# =============================================================================
if __name__ == "__main__":

    parser = argparse.ArgumentParser(
        prog = 'GetBelibData',
        description = 'Ce script recupere les donnees Belib en OpenData a Paris'+\
            ' et stocke les donnees qui nous interessent dans une base de donnees'+\
            ' sqlite.',
        epilog = '-- Juba Hamma, 2023.')
    
    parser.add_argument('-g', '--all', action = 'store_true',
        help = "Mise a jour de l'ensemble des donnees dans la table 'Bornes'.")
    parser.add_argument('-f', '--favoris', action = 'store_true',\
        help = "Mise a jour des donnees relatives aux stations en favoris dans "+\
                "la table 'Stations_fav'.")
    parser.add_argument('-l', '--live', action = 'store_true', 
        help = "Mise a jour des donnees relatives aux stations autour d'une "+\
                "position specifiee via une adresse et le rayon de recherche, "+\
                "dans la table 'Stations_live'.")
    parser.add_argument('-a', '--adresse', type=str, nargs=1, default="",
        help ="Adresse a entrer dans le cas de l'option --live.")    
    parser.add_argument('-d', '--distance', type=str, nargs=1, default="",
        help ="Rayon de recherche a entrer sous la forme '0.5km' dans le cas "+\
            "de l'option --live.")      

    args = parser.parse_args()
    glob = args.all
    fav = args.favoris
    live = args.live

    if (glob and not fav and not live) :
        update_global()

    if (not glob and fav and not live) :
        pos_lat, pos_lon=48.84, 2.28
        dist="0.5km"
        table = "Stations_fav"
        update_bornes_around_pos(table, pos_lat, pos_lon, dist)

    if (not glob and not fav and live) :
        adresse_live = args.adresse[0]
        dist_live = args.distance[0]
        if (adresse_live and dist_live) :
            update_bornes_around_adresse_live(adresse_live, dist_live)
