#!/usr/bin/python3

# ===========================================================================
# Script de plotting de figures illustrant les donnees de la db sqlite belib.
# Author : Juba Hamma.
# ===========================================================================

import matplotlib as mpl
import sqlite3 as sql3

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


# ============================================================================
if __name__ == "__main__":

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

    ## Connexion à la db et recuperation de l'ensemble des data
    con = sql3.connect(path_to_db+db_filename)
    cur = con.cursor()

    ## Recuperation des donnees dans un dictionnaire
    dict_data = sqlitedb_to_dict(cur, dict_labels)

    print(dict_data)



