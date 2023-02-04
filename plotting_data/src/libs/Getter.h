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

/**
 * @brief 
 * 
 * @param tableau_str 
 * @param len_tab 
 */
void free_tab_char1(char **tableau_str, int len_tab);


/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param tableau_int 
 * @param dim1 
 * @param dim2 
 * @param dim3 
 */

void free_tab_int3(int ***tableau_int, int dim1, int dim2, int dim3);


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
 * @param db_belib 
 * @return int 
 */
int Get_nb_stations_fav(sqlite3 *db_belib);


/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param db_belib 
 * @return int 
 */
int Get_nb_rows_par_station(sqlite3 *db_belib);


/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param db_belib 
 * @param tableau_adresses_fav 
 * @param nb_stations_fav 
 * @warning MEMALLOC 
 */
void Get_adresses_fav(sqlite3 *db_belib, \
            char **tableau_adresses_fav, int nb_stations_fav);


/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param db_belib 
 * @param tableau_date_recolte_fav 
 * @param nb_rows_par_station
 * @warning MEMALLOC 
 */
void Get_date_recolte_fav(sqlite3 *db_belib, \
            char **tableau_date_recolte_fav, int nb_rows_par_station);


/* --------------------------------------------------------------------------- */
void Get_statuts_station_fav(sqlite3 *db_belib,\
            char **tableau_adresses_fav, int nb_stations_fav, \
                int nb_rows_par_station, int nb_statuts);


/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param station 
 * @param tableau_adresses_fav 
 * @return char* 
 * @warning MALLOC
 */
char *Construct_req_station_statuts(int station, char **tableau_adresses_fav);


/* --------------------------------------------------------------------------- */
// Definition des fonctions
/* --------------------------------------------------------------------------- */


/* --------------------------------------------------------------------------- */
char *Construct_req_station_statuts(int station, char **tableau_adresses_fav)
{
    const char *query_statuts_stations_fav = \
            "SELECT disponible, occupe, en_maintenance, inconnu FROM \
                'Stations_fav' WHERE adresse_station = ";

    int len_query_base = strlen(query_statuts_stations_fav);
    int len_max_adresse = 70;
    int len_query_station = len_query_base + len_max_adresse;

    char *req = malloc(len_query_station*sizeof(char));

    strcpy(req, query_statuts_stations_fav);
    strcat(req, "\'");
    strcat(req, tableau_adresses_fav[station]);
    strcat(req, "\';");

    return req;
}


/* --------------------------------------------------------------------------- */
void Get_statuts_station_fav(sqlite3 *db_belib,\
            char **tableau_adresses_fav, int nb_stations_fav, \
                int nb_rows_par_station, int nb_statuts)
{
    // int*** statuts_table = \
    //     (int ***)calloc(nb_stations_fav*nb_rows_par_station*nb_statuts,sizeof(int));

    // ENUM DANS LE IF POUR STATUT !!
    for (int station = 0; station < nb_stations_fav; station++)
    {
        // Declaration statement
        sqlite3_stmt *stmt_station;

        // Construction requete
        char *req = Construct_req_station_statuts(station, tableau_adresses_fav);
        printf("> %s\n", req);

        // Test de la requete
        if (sqlite3_prepare_v2(db_belib, req, -1, &stmt_station, NULL))
        {
            printf("Erreur SQL :\n");
            printf("%s : %s\n", sqlite3_errstr(sqlite3_extended_errcode(db_belib)),\
                            sqlite3_errmsg(db_belib));
            sqlite3_close(db_belib);
            exit(EXIT_FAILURE);
        }


        // Application du statement : on recupere les statuts
        for (int i = 0; i < nb_rows_par_station; i++) {
            int step = sqlite3_step(stmt_station);
            if (step == SQLITE_ROW) 
            {
                printf("Disponible : %d, Occupe : %d, Maintenance : %d, Inconnu : %d \n", sqlite3_column_int(stmt_station, 0), sqlite3_column_int(stmt_station, 1), sqlite3_column_int(stmt_station, 2), sqlite3_column_int(stmt_station, 3));
            }
            // ELIF STOP
        }

        // Reset du stmt
        sqlite3_finalize(stmt_station);

        free(req);
    }
 
    // return statuts_table;
}

/* --------------------------------------------------------------------------- */
void Get_date_recolte_fav(sqlite3 *db_belib, \
                char **tableau_date_recolte_fav, int nb_rows_par_station)
{
    int len_max = 20;

    // Declaration statement
    sqlite3_stmt *stmt;

    const char *query_adresse_stations_fav = \
                "SELECT DISTINCT(date_recolte) FROM 'Stations_fav';";

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
            tableau_date_recolte_fav[i] = (char *)malloc(len_max);
            strcpy(tableau_date_recolte_fav[i], (char *)sqlite3_column_text(stmt, 0));
        }
        // ELIF STOP
    }

    // Reset du stmt
    sqlite3_finalize(stmt);
}


/* --------------------------------------------------------------------------- */
void Get_adresses_fav(sqlite3 *db_belib, \
                char **tableau_adresses_fav, int nb_stations_fav)
{

    int len_max = 70;
    // Declaration statement
    sqlite3_stmt *stmt;

    const char *query_adresse_stations_fav = \
                "SELECT DISTINCT(adresse_station) FROM 'Stations_fav';";

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
            tableau_adresses_fav[i] = (char *)malloc(len_max*sizeof(char));
            strcpy(tableau_adresses_fav[i], (char *)sqlite3_column_text(stmt, 0));
            // printf("Adresse %d : %s \n", i+1, tableau_adresses_fav[i]);
        }
        // ELIF STOP
    }

    // Reset du stmt
    sqlite3_finalize(stmt);

}

/* --------------------------------------------------------------------------- */
int Get_nb_rows_par_station(sqlite3 *db_belib)
{

    // Declaration statement
    sqlite3_stmt *stmt;

    int nb_rows_par_station = 0;

    const char *query_nb_row_par_station = \
            "SELECT COUNT(DISTINCT date_recolte) FROM 'Stations_fav';";


    // Test de la requete
    if (sqlite3_prepare_v2(db_belib, query_nb_row_par_station,-1, &stmt, NULL))
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

    // Reset du stmt
    sqlite3_finalize(stmt);

    return nb_rows_par_station;
}

/* --------------------------------------------------------------------------- */
int Get_nb_stations_fav(sqlite3 *db_belib)
{

    // Declaration statement
    sqlite3_stmt *stmt;

    int nb_stations_favs = 0;

    const char *query_nb_stations = \
            "SELECT COUNT(DISTINCT adresse_station) FROM Stations_fav;";
    
    // Test de la requete
    if (sqlite3_prepare_v2(db_belib, query_nb_stations,-1, &stmt, NULL))
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

    // Reset du stmt
    sqlite3_finalize(stmt);

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

/* --------------------------------------------------------------------------- */
void free_tab_int3(int ***tableau_int, int dim1, int dim2, int dim3)
{
    for (int i=0; i < dim1; i++ )
    {
        for (int j=0; j < dim2; j++ )
        {
            for (int k=0; i < dim3; k++ )
            {            
                free(&tableau_int[i][j][k]);
            }
        }
    }    
}



/* --------------------------------------------------------------------------- */
void free_tab_char1(char **tableau_str, int len_tab)
{
    for (int i=0; i< len_tab; i++ )
    {
        free(tableau_str[i]);
    }
}
 

