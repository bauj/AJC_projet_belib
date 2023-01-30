#!/usr/bin/python3

# ===========================================================================
# Script de plotting de figures illustrant les donnees de la db sqlite belib.
# Author : Juba Hamma.
# ===========================================================================

import matplotlib.pyplot as plt
import sqlite3 as sql3
from datetime import datetime

# ------------------------------------------------------------------------------
def sqlitedb_to_dict(cur, dict_labels):
    """ Recuperation des donnees de la db sqlite3 vers un dictionnaire 
        (en colonnes)

    Args:
        cur (sqlite3 cursorClass): Curseur sqlite3
        dict_labels (dict): Dictionnaire contenant de plus jolis labels que 
        ceux des colonnes de la db

    Returns:
        dict: Dictionnaire des données récupérées de la db
    """
    ## Initialisation dictionnaire data
    dict_data = dict()

    ## Parcours du dictionnaire des labels pour renommer les colonnes
    for k, v in dict_labels.items():
        ## Construction de la requete SQL colonne par colonne
        query_sql = f"SELECT {k} as '{v}' FROM Records"
        cur.execute(query_sql)

        ## Recuperation de la colonne dans le dictionnaire des donnees
        dict_data[v] = [val[0] for val in cur.fetchall()]

    return dict_data

# ------------------------------------------------------------------------------
def dict_factory(cursor, row):
    d = {}
    for idx, col in enumerate(cursor.description):
        d[col[0]] = row[idx]
    return d

# ------------------------------------------------------------------------------
def get_dict_last_line(cursor):
    
    query_last_line = "SELECT * FROM Records WHERE DATERECOLTE IN (SELECT max(DATERECOLTE) FROM Records)"
    cur.execute(query_last_line)

    ## Recuperation des statuts des bornes (saut de ID et DATERECOLTE)
    data_last_line = list(cur.fetchone())[2:] 

    ## Clean None to 0
    data_last_line = [0 if val==None else val for val in data_last_line]

    ## Construction dict last_line en enlevant les 0
    dict_last_line = {statuts_labels[i]: data_last_line[i] 
                            for i in range(len(statuts_labels)) 
                                if data_last_line[i] != 0}

    return dict_last_line

# ------------------------------------------------------------------------------
def get_last_date(cursor):
    
    query_last_date = "SELECT DATERECOLTE FROM Records WHERE DATERECOLTE IN (SELECT max(DATERECOLTE) FROM Records)"
    cur.execute(query_last_date)
    last_date = cur.fetchone()[0]

    last_date = datetime.strptime(last_date, '%Y-%m-%dT%H-%MTZD').strftime('%d/%m/%Y à %H:%M')
    return last_date

# ------------------------------------------------------------------------------
def print_dict(dico):

    print(50*'-')
    for k,v in dico.items():
        print(k.ljust(30),v)
    print(50*'-');print()

    return
# ============================================================================
if __name__ == "__main__":

    ## Parametres generaux ---------------------------------------------------

    ## Parametres acces db
    path_to_db = "../traitement_data/sqlitedb/"
    db_filename = "records_belib.db"

    ## Initialisation dict des labels
    dict_labels = {
        'DATERECOLTE' : 'Date',
        'DISPONIBLE' : 'Disponible',
        'OCCUPE' : 'Occupé',
        'INCONNU ' : 'Inconnu',
        'MAINTENANCE' : 'En maintenance',
        'RESERVE' : 'Réservée',
        'NONIMPLEMENTE' : 'Pas implémentée',
        'SUPPRIME' : 'Supprimée',
        'ENCOURSMISESERVICE' : 'En cours de mise en service',
        'MISESERVICEPLANIFIEE' : 'Mise en service planifiée',
    }

    ## Initialisation dict des couleurs par statut
    dict_colors = {
        'Disponible' : '#a6cee3',
        'Occupé' : '#ff7f00',
        'Inconnu' : '#b2df8a',
        'En maintenance' : '#e31a1c',
        'Réservée' : '#cab2d6',
        'Pas implémentée' : '#33a02c',
        'Supprimée' : '#1f78b4',
        'En cours de mise en service' : '#fdbf6f',
        'Mise en service planifiée' : '#fb9a99',
    }


    ## Liste des statuts de bornes
    statuts_labels = list(dict_labels.values())[1:]

    ## Definition cm par rapport aux inches
    cm = 1/2.54

    ## -----------------------------------------------------------------------

    ## Connexion à la db et recuperation de l'ensemble des data
    con = sql3.connect(path_to_db+db_filename)
    cur = con.cursor()
    
    # ## Recuperation des donnees en colonne dans un dictionnaire
    # dict_data = sqlitedb_to_dict(cur, dict_labels)
    # print_dict(dict_data)

    ## Recup derniere ligne de data pour donut : max des date
    dict_last_line = get_dict_last_line(cur)
    print_dict(dict_last_line)
    colors_last_line = [dict_colors[key] for key in dict_last_line.keys()]
        
    ## Plotting

    # Fig 2 : Repartition des statuts des points de charge du jour
    ## Parametres

    ## Derniere date de recolte
    last_date_recolte = get_last_date(cur)

    ## Toutes les sections sont eclatees
    explode = [0.03] * len(dict_last_line) 

    fig2, ax2 = plt.subplots(figsize=(18*cm,18*cm),
                            )
    _, _, autotexts= ax2.pie( \
            dict_last_line.values(),
            wedgeprops=dict(width=0.5),  startangle=-20,
            explode=explode,
            colors=colors_last_line,
            radius=1.,
            autopct='%1.1f%%',
            pctdistance=1.10,
            )

    fig2.set_facecolor("black")
    ax2.set_facecolor("black")

    for idx,autotext in enumerate(autotexts):
        autotext.set_color(colors_last_line[idx])
        autotext.set_fontsize(13)
        autotext.set_fontweight("bold")

    legend2 = ax2.legend(dict_last_line.keys(),
                         loc="center",
                        )

    frame2 = legend2.get_frame()
    frame2.set_color("white")
    frame2.set_alpha(0.05)
    texts_legend2 = legend2.get_texts()
    for text in texts_legend2:
        text.set_color("white")
        text.set_fontsize(13)
        text.set_ha("right")


    centre_circle = plt.Circle((0,0),0.70,fc='black')
    fig = plt.gcf()
    fig.gca().add_artist(centre_circle)

    text_last_maj = f"(Dernière mise à jour : {last_date_recolte})"
    ax2.annotate(text_last_maj, xy= (1.2,-1.2),
                 ha='right', va='bottom',
                 fontsize=11, fontstyle='oblique', color='white')
    plt.tight_layout()
    plt.savefig("test_figure2.png")

