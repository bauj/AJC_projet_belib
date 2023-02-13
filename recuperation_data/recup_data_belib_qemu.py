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


# Definitions des chemins en fonction des machines utilisées
global figure_dir, db_dir

## AJC / LENOVO
# figure_dir = "./"
# db_dir = "../db_sqlite/"
# mapbox_token_path = "../.mapbox_token"

# QEMU
figure_dir = "/var/www/html/figures/"
db_dir = "/var/db_belib/"
mapbox_token_path = "/etc/plot_belib/.mapbox_token"

# -----------------------------------------------------------------------------
# Parametres globaux
# -----------------------------------------------------------------------------

## Dictionnaire associant les Statuts récupérés aux noms des statuts de la bdd
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
    """Iterateur permettant de renvoyer le contenu de data_general

    Args:
        data_general (dict): Dictionnaire contenant les infos des stations favorites ou live

    Yields:
        value : Valeur contenue dans la colonne du dictionnaire spécifiée, renvoyée à la volée (generateur)
    """
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
    """Iterateur permettant de renvoyer le contenu de raw_data_all_bornes pour l'ensemble
    des bornes

    Args:
        n (int): Nombre de bornes
        raw_data_all_bornes (list): Liste de Dictionnaires contenant les données brutes récupérées pour l'ensemble des bornes

    Yields:
        value : Valeur contenue dans la colonne du dictionnaire spécifiée, renvoyée à la volée (générateur)
    """
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
    """Iterateur permettant de renvoyer le contenu de list_stations_pref pour l'ensemble
    des bornes 

    Args:
        n (int): Nombre de stations
        list_stations_pref (list): Liste de Dictionnaires contenant les données relatives aux stations (fav ou live)

    Yields:
        value : Valeur contenue dans la colonne du dictionnaire spécifiée, renvoyée à la volée (générateur)
    """
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
    """Etablit la connexion avec la base de données SQLite3

    Args:
        path_db (string): Chemin vers la base de données SQLite3

    Returns:
        conn: Objet sqlite3 représentant une connexion à une db
    """
    
    conn = None
    try:
        conn = sqlite3.connect(path_db)
    except NameError as e:
        print(e)

    return conn

# -----------------------------------------------------------------------------
def update_all_bornes(path_db):
    """Mise à jour de la table Bornes de la bdd SQLite3

    Args:
        path_db (string): Chemin vers la bdd SQLite3
    """

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
    """Transforme le contenu de la colonne coordonnees_xy des data raw en lon, lat

    Args:
        str_coordxy (string): Chaine de caractere du type "(lon,lat)"

    Returns:
        tuple : Tuple de taille 2, contenant la longitude et la latitude
    """
    
    lon, lat = str_coordxy.split(",")
    lon = float(lon[1:])
    lat = float(lat[:-1])
    
    return lon, lat


# -----------------------------------------------------------------------------
def def_station(daterecolte_, adr, lon, lat, n_dispo, n_occup, n_inc, n_main, \
            n_del=0, n_res=0, n_encours_mes=0, n_mes_plan=0, n_nonimp=0):
    """Construit un dictionnaire contenant l'ensemble des données d'une station belib

    Args:
        daterecolte_ (string): Chaine de caracteres correspondant à la date de recolte
        adr (string): Chaine de caracteres correspondant à l'adresse
        lon (float): Longitude
        lat (float): Latitude
        n_dispo (int): Nombre de bornes disponibles
        n_occup (int): Nombre de bornes occupées
        n_inc (int): Nombre de bornes au statut inconnu
        n_main (int): Nombre de bornes en maintenance
        n_res (int, optional): Nombre de bornes réservées. Defaults to 0.
        n_encours_mes (int, optional): Nombre de bornes en cours de mise en service. Defaults to 0.
        n_mes_plan (int, optional): Nombre de bornes avec mise en service planifiée. Defaults to 0.
        n_nonimp (int, optional): Nombre de bornes au statut non implémentée. Defaults to 0.

    Returns:
        dict: Dictionnaire regroupant les infos de la station
    """

    return {"date_recolte": daterecolte_, "adresse_station" : adr,
            "lon" : lon, "lat" : lat, "disponible": n_dispo, "occupe": n_occup,
            "inconnu" : n_inc, "en_maintenance" : n_main, "supprime" : n_del, 
            "reserve" : n_res, "en_cours_mes" : n_encours_mes, 
            "mes_planifiee" : n_mes_plan, "non_implemente" : n_nonimp,
        }

# -----------------------------------------------------------------------------
def def_row_general(daterecolte_, n_dispo, n_occup, n_inc, n_main, \
            n_del=0, n_res=0, n_encours_mes=0, n_mes_plan=0, n_nonimp=0):
    """Construit un dictionnaire contenant l'ensemble des données des bornes belib, utiles pour faire
    des stats globales sur l'ensemble du réseau belib.

    Args:
        daterecolte_ (string): Chaine de caracteres correspondant à la date de recolte
        n_dispo (int): Nombre de bornes disponibles
        n_occup (int): Nombre de bornes occupées
        n_inc (int): Nombre de bornes au statut inconnu
        n_main (int): Nombre de bornes en maintenance
        n_res (int, optional): Nombre de bornes réservées. Defaults to 0.
        n_encours_mes (int, optional): Nombre de bornes en cours de mise en service. Defaults to 0.
        n_mes_plan (int, optional): Nombre de bornes avec mise en service planifiée. Defaults to 0.
        n_nonimp (int, optional): Nombre de bornes au statut non implémentée. Defaults to 0.

    Returns:
        dict: Dictionnaire contenant les statuts de l'ensemble des bornes belib sur le réseau.
    """

    return {"date_recolte": daterecolte_, "disponible": n_dispo, 
        "occupe": n_occup, "inconnu" : n_inc, "en_maintenance" : n_main, 
        "supprime" : n_del, "reserve" : n_res, "en_cours_mes" : n_encours_mes, 
        "mes_planifiee" : n_mes_plan, "non_implemente" : n_nonimp,
        }

# -----------------------------------------------------------------------------
def transform_dict_station(list_records):
    """Fonction transformant les données brutes récupérées sous forme de dictionnaire en une 
    liste de dictionnaire de stations (avec tous les statuts des bornes par station)

    Args:
        list_records (list): Liste des dictionnaires de chaque station+statut_unique (data json brute prétraitée)

    Returns:
        list: Liste des dictionnaires des stations avec statuts concaténés
    """

    now = datetime.now()
    date_recolte = now.strftime("%Y-%m-%dT%H:%MZ")

    # check si liste non vide

    list_dict_station = []

    # 1ere station : initialisation 1ere adresse
    try :
        rec0 = list_records[0]["record"]["fields"]
    except IndexError: # Pas de stations trouvees
        # print("> Pas de station trouvée.")
        with open(figure_dir+"station_non_trouve.png", "rb") as fstationintrouvable:
            data = fstationintrouvable.read()
        with open(figure_dir+"mapbox_Stations_live.png", "wb") as foutput:
            foutput.write(data)
        with open(figure_dir+"fig2_barplot_live.png", "wb") as foutput:
            foutput.write(data)
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
           
        else :
            list_dict_station.append(dict_station)
            if (len(list_dict_station) == 8):
                # print("> 8 stations trouvees. On n'affichera pas plus de stations.") 
                break

            adresse_record = rec["adresse_station"] 
            lon,lat = get_lon_lat_from_coordxy(rec["coordonneesxy"])
            dict_station = def_station(date_recolte, adresse_record, lon, lat, 0, 0, 0, 0)

            statut_record = dict_labels_statuts[rec['statut_pdc']]
            dict_station[statut_record] = rec["nb_bornes"]

        # Ajout de la derniere station si on finit de parcourir la liste
        # un while serait surement + judicieux pour cette tache
        if (idx+1 == len(list_records)-1):
            list_dict_station.append(dict_station)
        


    return list_dict_station  


# -----------------------------------------------------------------------------
def update_bornes_around_pos(path_db, table, pos_lat, pos_lon, dist):
    """Update de la table de la db SQLite3 avec les données des stations autour d'une position GPS

    Args:
        path_db (string): Chemin vers la db SQLite3
        table (string): Nom de la table à compléter
        pos_lat (float): Latitude de la position de recherche
        pos_lon (float): Longitude de la position de recherche
        dist (float): Rayon de recherche en km
    """

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

    # print(ujson.dumps(raw_data_stations_pref,indent=3))

    list_stations = transform_dict_station(raw_data_stations_pref)

    nb_stations = len(list_stations)

    # print(2*"\n")
    # print("APRES TRANSFO")
    # print(2*"\n")
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
    """Fonctionnant générant une image statique Mapbox contenant des pins au niveau 
    des stations trouvées, de la couleur des plots obtenus en C.

    Args:
        table (string): Nombre de la table lue
        list_stations (list): Liste des dictionnaires de stations
        http (PoolManager): PoolManager urllib3
        pos_lat (float): Latitude du point de recherche (point central)
        pos_lon (float): Longitude du point de recherche (point central)
        dist (float): Rayon de recherche spécifié. Permet de créer une bounding box de manière grossière.
    """

    colors=['33a02c', 'ff7f00', '1f78b4',\
            'e31a1c', '984ea3', 'b2df8a',\
            'fdbf6f', 'a6cee3', 'fb9a99',\
            'cab2d6']

    unit_dist = "km"
    str_dist = f"{dist:.2f}{unit_dist}"

    # str_lat = f"{pos_lat:.3f}".replace(".","_")
    # str_lon = f"{pos_lon:.3f}".replace(".","_")
    # str_dist_ = str_dist.replace(".","-")
    # # filename_output = "mapbox_lat_"+str_lat+"_lon_"+str_lon+"_d_"+str_dist_+".png"

    filename_output = f"mapbox_{table}.png"
    
    nb_stations = len(list_stations)

    ## Construction url api mapbox
    # ## Construction des pins sur la carte
    pins = ""
    for idx,st in enumerate(list_stations):
        lat_station = list_stations[idx]["lon"]
        lon_station = list_stations[idx]["lat"]
        pins += f"pin-l-charging-station+{colors[idx]}({lon_station:.4f},{lat_station:.4f}),"
    ## 1er pin : position entree
    pins += f"pin-l-home+111({pos_lon:.4f},{pos_lat:.4f})"

    with open(mapbox_token_path) as ftoken:
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
    width ="300"
    height ="200"
    padding = "padding=10,10,10"
    x2 = "@2x"
    # x2 = ""

    url_req = "https://api.mapbox.com/styles/v1/"
    url_req += f"{username}/{style_id}/{mode}/{overlay}/{bbox}/{width}x{height}{x2}?{padding}&{token}"

    ## Execution requete et enregistrement en format png
    with open(figure_dir+filename_output, 'wb') as foutput:
        foutput.write(http.request('GET', url_req).data)

    return

# -----------------------------------------------------------------------------
def update_general(path_db):
    """Mise a jour de la table General de la bdd SQLite3 avec le statut de l'ensemble des bornes

    Args:
        path_db (string): Chemin vers la db SQLite3
    """
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
    """Transformation de l'adresse entrée en live en position lon,lat à l'aide de 
    l'API adresse.gouv

    Args:
        adr (string): Adresse postale française

    Returns:
        tuple: Tuple de taille 2 contenant la lon et la lat
    """
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
        # print("> Adresse non trouvee.")
        with open(figure_dir+"adresse_introuvable.png", "rb") as fadresseintrouvable:
            data = fadresseintrouvable.read()
        with open(figure_dir+"mapbox_Stations_live.png", "wb") as foutput:
            foutput.write(data)
        with open(figure_dir+"fig2_barplot_live.png", "wb") as foutput:
            foutput.write(data)
        sys.exit(1)

    # test = ujson.dumps(raw_data, indent=4)
    # print(test)

    return lon, lat

# -----------------------------------------------------------------------------
def clean_table(path_db, table):
    """Fonction permettant de vider une table d'une db SQLite3

    Args:
        path_db (string): Chemin vers la bdd SQLite3
        table (string): Nom de la table à traiter
    """
    conn = create_connection(path_db)

    sql_req = f"DELETE FROM {table}"

    with conn :
        cur = conn.cursor()
        cur.execute(sql_req)
        conn.commit()

# -----------------------------------------------------------------------------
def update_bornes_around_adresse_live(path_db, adr, dist):
    """Mise a jour de la table "Stations_live" de la db SQLite3. On la vide avant de la compléter.

    Args:
        path_db (string): Chemin vers la bdd SQLite3
        adr (string): Adresse postale française
        dist (float): Rayon de recherche en km
    """
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

    filename_db = "belib_data.db"
    path_db = db_dir + filename_db

    if (bornes and not general and not fav and not live) :
        update_all_bornes(path_db)

    if (not bornes and general and not fav and not live) :
        update_general(path_db)

    if (not bornes and not general and fav and not live) :
        pos_lat, pos_lon = 48.8401, 2.2780
        dist = 0.5
        table = "Stations_fav"
        update_bornes_around_pos(path_db,table, pos_lat, pos_lon, dist)

    if (not bornes and not general and not fav and live) :
        adresse_live = args.adresse[0]
        dist_live = args.distance[0]
        if (adresse_live and dist_live) :
            update_bornes_around_adresse_live(path_db, adresse_live, dist_live)
