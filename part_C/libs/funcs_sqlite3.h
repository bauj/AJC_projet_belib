#ifndef FUNC_SQLITE3_H
#define FUNC_SQLITE3_H

/** Recuperation de la struct record */
#include "parsing_json.h" 
#include <sqlite3.h>

// ----------------------------------------------------------------------------
// Fabrication d'un "dictionnaire" associant le statut donné par le json et la
// colonne de la bdd sqlite3
// ----------------------------------------------------------------------------

/** Saut de 2 colonnes dans l'enumeration : ID et DATERECOLTE*/
enum statuts_pdc 
{
    DISPONIBLE=2, OCCUPE, INCONNU, MAINTENANCE, RESERVE, NONIMPLEMENTE, SUPPRIME, ENCOURSMISESERVICE, MISESERVICEPLANIFIEE
};

/** Tableau de string qui permet d'associer le statut lu dans le json et la colonne de la db sqlite */
const char *const statuts_pdc_str[] = {
    "Disponible", "Occup\\u00e9 (en charge)","Inconnu", "En maintenance",\
    "R\\u00e9serv\\u00e9", "Pas impl\\u00e9ment\\u00e9", "Supprim\\u00e9",\
    "En cours de mise en service", "Mise en service planifi\\u00e9e"\
    };


/**
 * @brief Insertion du tableau de record dans la base de données sqlite
 * 
 * @param date_recolte Date de recolte des donnees
 * @param nb_record Nombre de records récupérés (dans le json)
 * @param records Tableau des records
 * @param db Descripteur de la db sqlite
 */
void insert_records(char* date_recolte, int nb_record, record *records, sqlite3 *db);



void insert_records(char* date_recolte, int nb_record, record *records, sqlite3 *db) {

    // Requete SQL d'insertion dans la db
    const char *query = "INSERT INTO Records \
            (DATERECOLTE, DISPONIBLE, OCCUPE, INCONNU, MAINTENANCE, RESERVE, NONIMPLEMENTE, SUPPRIME, ENCOURSMISESERVICE, MISESERVICEPLANIFIEE) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

    // Declaration stmt
    sqlite3_stmt *stmt;

    // Test de la requete
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL))
    {
        printf("Erreur SQL :\n");
        printf("%s : %s\n", sqlite3_errstr(sqlite3_extended_errcode  (db)), sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }
    
    // Bind date de recolte
    sqlite3_bind_text(stmt, 1, date_recolte, -1, NULL);

    // Bind des data lues dans le json (pour chaque record)
    for (int i = 0; i < nb_record; i++) {
        // Boucle sur les status possibles et insertion de ceux 
        for (int j = DISPONIBLE; j <= MISESERVICEPLANIFIEE; j++)
        {
            if (strcmp(records[i].statut_pdc,statuts_pdc_str[j-2])==0)
            {
                // Saut des deux premiers indices (ID et DATERECOLTE)
                sqlite3_bind_int(stmt, j, records[i].nb_bornes);
                break;
            }
        }
    }
    
    // Application du statement et fermeture de la db
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}


#endif  /* FUNC_SQLITE3_H */