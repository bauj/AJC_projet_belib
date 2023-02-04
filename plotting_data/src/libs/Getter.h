/* ----------------------------------------------------------------------------
*  Bibliotheque gerant les requetes sql a la db belib_data.
*  
*  Author : Juba Hamma. 2023.
* ---------------------------------------------------------------------------- 
*/
#ifndef GETTER_H
#define GETTER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sqlite3.h>


#endif /* GETTER_H */

/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param bdd_filename 
 * @param db_belib 
 */
void Sqlite_open_check(char *bdd_filename, sqlite3 **db_belib);

/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param stmt 
 * @param db_belib 
 * @return int 
 */
int Get_nb_stations_fav(sqlite3_stmt *stmt, sqlite3 *db_belib);


/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param stmt 
 * @param db_belib 
 * @return int 
 */
int Get_nb_rows_par_station(sqlite3_stmt *stmt, sqlite3 *db_belib);


/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param stmt 
 * @param db_belib 
 * @param tableau_adresses_fav 
 * @param nb_stations_fav 
 */
void Get_adresses_fav(sqlite3_stmt *stmt, sqlite3 *db_belib, \
            char **tableau_adresses_fav, int nb_stations_fav);


/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param stmt 
 * @param db_belib 
 * @param tableau_date_recolte_fav 
 * @param nb_rows_par_station 
 */
void Get_date_recolte_fav(sqlite3_stmt *stmt, sqlite3 *db_belib, \
            char **tableau_date_recolte_fav, int nb_rows_par_station);


/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param stmt 
 * @param db_belib 
 * @param nb_stations_fav 
 * @param nb_rows_par_station 
 * @param nb_statuts 
 * @return int*** 
 */
int*** Get_statuts_station_fav(sqlite3_stmt stmt, sqlite3 *db_belib,\
        int nb_stations_fav, int nb_rows_par_station, int nb_statuts);

/* --------------------------------------------------------------------------- */
// Definition des fonctions
/* --------------------------------------------------------------------------- */

void Get_date_recolte_fav(sqlite3_stmt *stmt, sqlite3 *db_belib, \
                char **tableau_date_recolte_fav, int nb_rows_par_station)
{
    const char *query_adresse_stations_fav = \
                "SELECT DISTINCT(date_recolte) FROM 'Stations_fav';";

    // Reset du stmt
    sqlite3_reset(stmt);
    // Test de la requete
    if (sqlite3_prepare_v2(db_belib, query_adresse_stations_fav, -1, &stmt, NULL))
    {
        printf("Erreur SQL :\n");
        printf("%s : %s\n", sqlite3_errstr(sqlite3_extended_errcode(db_belib)),\
                         sqlite3_errmsg(db_belib));
        sqlite3_close(db_belib);
        exit(EXIT_FAILURE);
    }

    // Application du statement et fermeture de la db

    for (int i = 0; i < nb_rows_par_station; i++) {
        int step = sqlite3_step(stmt);
        if (step == SQLITE_ROW) 
        {
            tableau_date_recolte_fav[i] = \
                (char *)malloc(strlen((char *)sqlite3_column_text(stmt, 0))*sizeof(char));
            strcpy(tableau_date_recolte_fav[i], (char *)sqlite3_column_text(stmt, 0));
        }
    }
}


/* --------------------------------------------------------------------------- */
void Get_adresses_fav(sqlite3_stmt *stmt, sqlite3 *db_belib, \
                char **tableau_adresses_fav, int nb_stations_fav)
{
    const char *query_adresse_stations_fav = \
                "SELECT DISTINCT(adresse_station) FROM 'Stations_fav';";
            
    // Reset du stmt
    sqlite3_reset(stmt);
    // Test de la requete
    if (sqlite3_prepare_v2(db_belib, query_adresse_stations_fav, -1, &stmt, NULL))
    {
        printf("Erreur SQL :\n");
        printf("%s : %s\n", sqlite3_errstr(sqlite3_extended_errcode(db_belib)),\
                         sqlite3_errmsg(db_belib));
        sqlite3_close(db_belib);
        exit(EXIT_FAILURE);
    }

    // Application du statement et fermeture de la db

    for (int i = 0; i < nb_stations_fav; i++) {
        int step = sqlite3_step(stmt);
        if (step == SQLITE_ROW) 
        {
            tableau_adresses_fav[i] = \
                (char *)malloc(strlen((char *)sqlite3_column_text(stmt, 0))*sizeof(char));
            strcpy(tableau_adresses_fav[i], (char *)sqlite3_column_text(stmt, 0));
            // printf("Adresse %d : %s \n", i+1, tableau_adresses_fav[i]);
        }
    }

}

/* --------------------------------------------------------------------------- */
int Get_nb_rows_par_station(sqlite3_stmt *stmt, sqlite3 *db_belib)
{
    int nb_rows_par_station = 0;

    const char *query_nb_row_par_station = \
            "SELECT COUNT(DISTINCT date_recolte) FROM 'Stations_fav';";

    // Reset du stmt
    sqlite3_reset(stmt);
    // Test de la requete
    if (sqlite3_prepare_v2(db_belib, query_nb_row_par_station, -1, &stmt, NULL))
    {
        printf("Erreur SQL :\n");
        printf("%s : %s\n", sqlite3_errstr(sqlite3_extended_errcode(db_belib)),\
                         sqlite3_errmsg(db_belib));
        sqlite3_close(db_belib);
        exit(EXIT_FAILURE);
    }
    // Application du statement et fermeture de la db
    int step = sqlite3_step(stmt);

    if (step == SQLITE_ROW) 
    {
        nb_rows_par_station = sqlite3_column_int(stmt, 0);
    }

    return nb_rows_par_station;
}

/* --------------------------------------------------------------------------- */
int Get_nb_stations_fav(sqlite3_stmt *stmt, sqlite3 *db_belib)
{
    int nb_stations_favs = 0;

    const char *query_nb_stations = \
            "SELECT COUNT(DISTINCT adresse_station) FROM Stations_fav;";
    

    // Reset du stmt
    sqlite3_reset(stmt);
    // Test de la requete
    if (sqlite3_prepare_v2(db_belib, query_nb_stations, -1, &stmt, NULL))
    {
        printf("Erreur SQL :\n");
        printf("%s : %s\n", sqlite3_errstr(sqlite3_extended_errcode(db_belib)),\
                         sqlite3_errmsg(db_belib));
        sqlite3_close(db_belib);
        exit(EXIT_FAILURE);
    }

    // Application du statement et fermeture de la db
    int step = sqlite3_step(stmt);

    // int nb_col = sqlite3_column_count(stmt);

    if (step == SQLITE_ROW) 
    {
        nb_stations_favs = sqlite3_column_int(stmt, 0);
    }
    // printf("Nb station favs : %d \n",sqlite3_column_int(stmt, 0));

    return nb_stations_favs;
}

/* --------------------------------------------------------------------------- */
void Sqlite_open_check(char *bdd_filename, sqlite3 **db_belib)
{
    int rc = sqlite3_open(bdd_filename, db_belib);

    // Test d'ouverture de la db
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Err: %s\n", sqlite3_errmsg(*db_belib));
        sqlite3_close(*db_belib);
        exit(EXIT_FAILURE);
    }
}



