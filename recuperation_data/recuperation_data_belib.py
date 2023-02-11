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
import sys
from datetime import date, timedelta, datetime

# -----------------------------------------------------------------------------
# Parametres globaux
# -----------------------------------------------------------------------------

dict_labels_statuts = {
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

# -----------------------------------------------------------------------------
# Fonctions
# -----------------------------------------------------------------------------

def iterator_data_general(data_general):
    yield \
        data_general["date_recolte"],\
        data_general["disponible"], \
        data_general["occupe"],\
        data_general['en_maintenance'],\
        data_general['inconnu'],\
        data_general['supprime'],\
        data_general['reserve'],\
        data_general['en_cours_mes'],\
        data_general['mes_planifiee'],\
        data_general['non_implemente']

# -----------------------------------------------------------------------------
def iterator_data_bornes(n, raw_data_all_bornes):
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
            list_stations_pref[i]['inconnu'],\
            list_stations_pref[i]['en_maintenance'],\
            list_stations_pref[i]['supprime'],\
            list_stations_pref[i]['reserve'],\
            list_stations_pref[i]['en_cours_mes'],\
            list_stations_pref[i]['mes_planifiee'],\
            list_stations_pref[i]['non_implemente']

# -----------------------------------------------------------------------------              
def create_connection(path_db):
    conn = None
    try:
        conn = sqlite3.connect(path_db)
    except Error as e:
        print(e)

    return conn

# -----------------------------------------------------------------------------
def update_all_bornes(path_db):

    http = urllib3.PoolManager()

    url_req = \
        "https://parisdata.opendatasoft.com/api/explore/v2.1/catalog/datasets/"+\
        "belib-points-de-recharge-pour-vehicules-electriques-disponibilite-temps-reel/"+\
        "exports/json?lang=fr&timezone=Europe%2FParis"

    resp = http.request("GET", url_req)
    
    raw_data_all_bornes = ujson.loads(resp.data) 
    nb_bornes = len(raw_data_all_bornes) 
    
    wanted_keys = ["last_updated", "id_pdc", "statut_pdc", "adresse_station", "lon", "lat"]
    
    conn = create_connection(path_db)

    cur = conn.cursor()
    table = "Bornes"
    insert_query = f"INSERT INTO {table} ("+", ".join(wanted_keys)+") VALUES ("+\
                                ", ".join(len(wanted_keys)*['?']) + ");"
    
    cur.executemany(insert_query, iterator_data_bornes(nb_bornes, raw_data_all_bornes))
    
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
def def_row_general(daterecolte_, n_dispo, n_occup, n_inc, n_main, \
            n_del=0, n_res=0, n_encours_mes=0, n_mes_plan=0, n_nonimp=0):

    return {"date_recolte": daterecolte_, "disponible": n_dispo, 
        "occupe": n_occup, "inconnu" : n_inc, "en_maintenance" : n_main, 
        "supprime" : n_del, "reserve" : n_res, "en_cours_mes" : n_encours_mes, 
        "mes_planifiee" : n_mes_plan, "non_implemente" : n_nonimp,
        }

# -----------------------------------------------------------------------------
def transform_dict_station(list_records):

    now = datetime.now()
    date_recolte = now.strftime("%Y-%m-%dT%H:%MZ")

    # check si liste non vide

    list_dict_station = []

    # 1ere station : initialisation 1ere adresse
    try :
        rec0 = list_records[0]["record"]["fields"]
    except IndexError: # Pas de stations trouvees
        print("> Pas de station trouvée.")
        sys.exit(1)

    adresse_record0 = rec0["adresse_station"] 
    lon0,lat0 = get_lon_lat_from_coordxy(rec0["coordonneesxy"])
    dict_station = def_station(date_recolte, adresse_record0, lon0, lat0, 0, 0, 0, 0)
    statut_record0 = dict_labels_statuts[rec0['statut_pdc']]
    dict_station[statut_record0] = rec0["nb_bornes"]

    # Saut du 1er record
    for idx,dico_record in enumerate(list_records[1:]):
        rec = dico_record["record"]["fields"]
        if (dict_station["adresse_station"] == rec["adresse_station"]) :
            statut_record = dict_labels_statuts[rec['statut_pdc']]
            dict_station[statut_record] = rec["nb_bornes"]

            if (idx+1 == len(list_records)-1):
                list_dict_station.append(dict_station)    
        else :
            list_dict_station.append(dict_station)
            if (len(list_dict_station) == 8):
                print("> 8 stations trouvees. On n'aaffichera pas plus de stations.") 
                break

            adresse_record = rec["adresse_station"] 
            lon,lat = get_lon_lat_from_coordxy(rec["coordonneesxy"])
            dict_station = def_station(date_recolte, adresse_record, lon, lat, 0, 0, 0, 0)

            statut_record = dict_labels_statuts[rec['statut_pdc']]
            dict_station[statut_record] = rec["nb_bornes"]

    return list_dict_station  


# -----------------------------------------------------------------------------
def update_bornes_around_pos(path_db, table, pos_lat, pos_lon, dist):

    http = urllib3.PoolManager()

    date_du_jour = date.today().strftime("%Y-%m-%d")
    date_veille = (date.today() - timedelta(days=1)).strftime("%Y-%m-%d")
    
    unit_dist = "km"
    str_dist = f"{dist:.2f}{unit_dist}"
    url_req = f"https://parisdata.opendatasoft.com/api/v2/catalog/datasets/"+\
        "belib-points-de-recharge-pour-vehicules-electriques-disponibilite-temps-reel/"+\
            "records?select=count%28id_pdc%29%20as%20nb_bornes&where=last_updated"+\
                f"%20%3E%20date%27{date_veille}%27%20AND%20last_updated%20%3C%3D%20date"+\
                    f"%27{date_du_jour}%27%20AND%20distance%28coordonneesxy%2C%20GEOM"+\
                        f"%27POINT%28{pos_lon}%20{pos_lat}%29%27%2C%20{dist}{unit_dist}%29&group_by"+\
                            "=adresse_station%2C%20coordonneesxy%2Cstatut_pdc&limit=100"+\
                                "&offset=0&timezone=UTC"

    resp = http.request("GET", url_req)

    raw_data_stations_pref = ujson.loads(resp.data)["records"] 
    # for i in range(nb_stations):
    # print(ujson.dumps(raw_data_stations_pref,indent=3))

    list_stations = transform_dict_station(raw_data_stations_pref)

    nb_stations = len(list_stations)

    # for i in range(nb_stations):
    #     print(ujson.dumps(list_stations[i],indent=3))

    wanted_keys = list(list_stations[0].keys())
    
    conn = create_connection(path_db)

    cur = conn.cursor()
    insert_query = f"INSERT INTO {table} ("+", ".join(wanted_keys)+") VALUES ("+\
                                ", ".join(len(wanted_keys)*['?']) + ");"
    
    cur.executemany(insert_query, iterator_data_stations(nb_stations, list_stations))
    
    conn.commit()

    conn.close()

    make_mapbox(table, list_stations, http, pos_lat, pos_lon, dist)

    return 

# -----------------------------------------------------------------------------
def make_mapbox(table, list_stations, http, pos_lat, pos_lon, dist):
    
    colors=['33a02c', 'ff7f00', '1f78b4',\
            'e31a1c', '984ea3', 'b2df8a',\
            'fdbf6f', 'a6cee3', 'fb9a99',\
            'cab2d6']
    
    str_lat = f"{pos_lat:.3f}".replace(".","_")
    str_lon = f"{pos_lon:.3f}".replace(".","_")
    unit_dist = "km"
    str_dist = f"{dist:.2f}{unit_dist}"
    str_dist_ = str_dist.replace(".","-")
    output_dir = "./"
    filename_output = f"mapbox_{table}.png"
    # filename_output = "mapbox_lat_"+str_lat+"_lon_"+str_lon+"_d_"+str_dist_+".png"

    nb_stations = len(list_stations)

    ## Construction url api mapbox

    ## Construction des pins sur la carte

    # overlay = f"pin-s+000({lon_center},{lat_center}),pin-s+f74e4e({lon_max},{lat_center})/"

    pins = ""
    for idx,st in enumerate(list_stations):
        lat_station = list_stations[idx]["lon"]
        lon_station = list_stations[idx]["lat"]
        pins += f"pin-l-charging-station+{colors[idx]}({lon_station:.4f},{lat_station:.4f}),"
    ## 1er pin : position entree
    pins += f"pin-l-home+111({pos_lon:.4f},{pos_lat:.4f})"

    with open("../.mapbox_token") as ftoken:
        read_mapbox_token = ftoken.read().split('\n')[0]
    token = "access_token="+read_mapbox_token

    bbox_sw = [pos-0.009*dist for pos in [pos_lon, pos_lat]] 
    bbox_ne = [pos+0.009*dist for pos in [pos_lon, pos_lat]] 


    username = "mapbox"
    style_id = "dark-v11"
    mode = "static"
    overlay = pins
    bbox = f"[{bbox_sw[0]:.4f},{bbox_sw[1]:.4f},{bbox_ne[0]:.4f},{bbox_ne[1]:.4f}]"
    # bbox = "auto"
    width ="350"
    height ="250"
    padding = "padding=10,10,10"
    x2 = "@2x"
    # x2 = ""

    url_req = "https://api.mapbox.com/styles/v1/"
    url_req += f"{username}/{style_id}/{mode}/{overlay}/{bbox}/{width}x{height}{x2}?{padding}&{token}"

    ## Execution requete et enregistrement en format png
    with open(output_dir+filename_output, 'wb') as foutput:
        foutput.write(http.request('GET', url_req).data)

    return

# -----------------------------------------------------------------------------
def update_general(path_db):

    http = urllib3.PoolManager()

    date_du_jour = date.today().strftime("%Y-%m-%d")
    date_veille = (date.today() - timedelta(days=1)).strftime("%Y-%m-%d")

    url_req = "https://parisdata.opendatasoft.com/api/v2/catalog/datasets/"+\
        "belib-points-de-recharge-pour-vehicules-electriques-disponibilite-temps-reel/"\
            "records?select=count%28id_pdc%29%20as%20nb_bornes&where=last_updated"+\
                f"%20%3E%20date%27{date_veille}%27%20AND%20last_updated%20%3C%3D%20date"+\
                    f"%27{date_du_jour}%27&group_by=statut_pdc&limit=100&offset=0&timezone=UTC"
    
    resp = http.request("GET", url_req)
    raw_data_general = ujson.loads(resp.data)["records"] 

    now = datetime.now()
    date_recolte = now.strftime("%Y-%m-%dT%H:%MZ")

    data_general = def_row_general(date_recolte, *[0]*9)

    for dico_record in raw_data_general:
        dico_fields = dico_record["record"]["fields"]
        data_general[dict_labels_statuts[dico_fields["statut_pdc"]]] = dico_fields["nb_bornes"]

    print(data_general)
    
    wanted_keys = list(data_general.keys())


    conn = create_connection(path_db)

    cur = conn.cursor()
    table = "General"
    insert_query = f"INSERT INTO {table} ("+", ".join(wanted_keys)+") VALUES ("+\
                                ", ".join(len(wanted_keys)*['?']) + ");"
    
    cur.executemany(insert_query, iterator_data_general(data_general))
    
    conn.commit()
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
    while (resp.status != 200) :
        resp = http.request("GET", url_req)

    raw_data = ujson.loads(resp.data)
    try :
        lon, lat = raw_data["features"][0]["geometry"]["coordinates"]
    except IndexError:
        print("> Adresse non trouvee.")
        with open("adresse_introuvable.png", "rb") as fadresseintrouvable:
            data = fadresseintrouvable.read()
        with open("mapbox_Stations_live.png", "wb") as foutput:
            foutput.write(data)
        sys.exit(1)

    test = ujson.dumps(raw_data, indent=4)
    print(test)

    return lon, lat

# -----------------------------------------------------------------------------
def clean_table(path_db, table):

    conn = create_connection(path_db)

    sql_req = f"DELETE FROM {table}"

    with conn :
        cur = conn.cursor()
        cur.execute(sql_req)
        conn.commit()

# -----------------------------------------------------------------------------
def update_bornes_around_adresse_live(path_db, adr, dist):

    lon_adr, lat_adr = adresse_to_lon_lat(adr)

    table="Stations_live"

    clean_table(path_db, table)
    update_bornes_around_pos(path_db, table, lat_adr, lon_adr, dist)
    
    return

# -----------------------------------------------------------------------------
# Main
# -----------------------------------------------------------------------------

# =============================================================================
if __name__ == "__main__":

    parser = argparse.ArgumentParser(
        prog = 'GetBelibData',
        description = 'Ce script recupere les donnees Belib en OpenData a Paris'+\
            ' et stocke les donnees qui nous interessent dans une base de donnees'+\
            ' sqlite.',
        epilog = '-- Juba Hamma, 2023.')
    
    parser.add_argument('-b', '--bornes', action = 'store_true',
        help = "Mise a jour de l'ensemble des donnees dans la table 'Bornes'.")
    parser.add_argument('-g', '--general', action = 'store_true',
        help = "Mise a jour de l'ensemble des donnees dans la table 'General'.")
    parser.add_argument('-f', '--favoris', action = 'store_true',\
        help = "Mise a jour des donnees relatives aux stations en favoris dans "+\
                "la table 'Stations_fav'.")
    parser.add_argument('-l', '--live', action = 'store_true', 
        help = "Mise a jour des donnees relatives aux stations autour d'une "+\
                "position specifiee via une adresse et le rayon de recherche, "+\
                "dans la table 'Stations_live'.")
    parser.add_argument('-a', '--adresse', type=str, nargs=1, default="",
        help ="Adresse a entrer dans le cas de l'option --live.")
    parser.add_argument('-d', '--distance', type=float, nargs=1, default=0.5,
        help ="Rayon de recherche a entrer en km dans le cas "+\
            "de l'option --live.")

    args = parser.parse_args()
    bornes = args.bornes
    general = args.general
    fav = args.favoris
    live = args.live

    dir_db = "../db_sqlite/"
    filename_db = "belib_data.db"
    path_db = dir_db + filename_db

    if (bornes and not general and not fav and not live) :
        update_all_bornes(path_db)

    if (not bornes and general and not fav and not live) :
        update_general(path_db)

    if (not bornes and not general and fav and not live) :
        pos_lat, pos_lon = 48.84, 2.28
        dist=0.5
        table = "Stations_fav"
        update_bornes_around_pos(path_db,table, pos_lat, pos_lon, dist)

    if (not bornes and not general and not fav and live) :
        adresse_live = args.adresse[0]
        dist_live = args.distance[0]
        if (adresse_live and dist_live) :
            update_bornes_around_adresse_live(path_db, adresse_live, dist_live)
