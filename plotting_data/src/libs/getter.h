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
#include "traitement.h"

/* --------------------------------------------------------------------------- */
typedef enum {disponible, occupe, en_maintenance, inconnu} statuts;


/* --------------------------------------------------------------------------- */
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
 * @warning MALLOC 
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
 * @warning MALLOC 
 */
void Get_date_recolte_fav(sqlite3 *db_belib, \
            Date *tableau_date_recolte_fav, int nb_rows_par_station);


/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param db_belib 
 * @param tableau_adresses_fav 
 * @param nb_stations_fav 
 * @param nb_rows_par_station 
 * @param nb_statuts 
 * @param tableau_statuts_fav 
 */
void Get_statuts_station_fav(sqlite3 *db_belib,char **tableau_adresses_fav,\
     int nb_stations_fav,int nb_rows_par_station, int nb_statuts,\
     int tableau_statuts_fav[nb_stations_fav][nb_rows_par_station][nb_statuts]);


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
/**
 * @brief 
 * 
 * @param nb_station 
 * @param nb_date 
 * @param nb_statuts 
 * @param tab 
 * @param tableau_date_recolte_fav 
 * @param tableau_adresses_fav 
 */
void Print_tableau_fav(int nb_station, int nb_date, int nb_statuts,\
            int tab[nb_station][nb_date][nb_statuts],\
            Date *tableau_date_recolte_fav, char** tableau_adresses_fav);
            

/* --------------------------------------------------------------------------- */
void Get_statut_station(int nb_stations, int nb_rows, int nb_statuts,\
            int vect_statut[nb_rows], \
            int tableau_statuts_fav[nb_stations][nb_rows][nb_statuts],\
            int station, int statut);

/* --------------------------------------------------------------------------- */
void Make_dateticks_vect(int nb_rows, Datetick vect_dateticks[nb_rows], Date tableau_date_recolte_fav[nb_rows]);

/* --------------------------------------------------------------------------- */
// Definition des fonctions
/* --------------------------------------------------------------------------- */

void Make_dateticks_vect(int nb_rows, Datetick vect_dateticks[nb_rows], Date tableau_date_recolte_fav[nb_rows])
{
    for (int i = 0; i < nb_rows; i++)
    {
        Datetick tick_i;
        Init_Datetick(&tick_i,\
                &tableau_date_recolte_fav[i], &tableau_date_recolte_fav[0]);

        vect_dateticks[i] =tick_i;
    }      
}


/* --------------------------------------------------------------------------- */
void Get_time_vect(int nb_rows, int vect_time[nb_rows],\
                Date tableau_date_recolte_fav[nb_rows])
{
    for (int i = 0; i < nb_rows; i++)
    {
        Datetick tick_i;
        Init_Datetick(&tick_i,\
                &tableau_date_recolte_fav[i], &tableau_date_recolte_fav[0]);

        vect_time[i] = tick_i.ecart_init;
    }  
}


/* --------------------------------------------------------------------------- */
void Get_statut_station(int nb_stations, int nb_rows, int nb_statuts,\
            int vect_statut[nb_rows], \
            int tableau_statuts_fav[nb_stations][nb_rows][nb_statuts],
            int station, int statut)
{
    for (int i = 0; i < nb_rows; i++)
    {
        vect_statut[i] = tableau_statuts_fav[station][i][statut];
        printf("%d \n", vect_statut[i]);
    }
}



/* --------------------------------------------------------------------------- */
void Get_adresses_fav(sqlite3 *db_belib, \
                char **tableau_adresses_fav, int nb_stations_fav)
{

    int len_max = 100;
    // Declaration statement
    sqlite3_stmt *stmt;

    const char *query_adresse_stations_fav = \
                "SELECT DISTINCT(adresse_station) FROM Stations_fav;";

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
            // On supprime " Paris" en fin de chaine
            
            strncpy(tableau_adresses_fav[i],\
                (char *)sqlite3_column_text(stmt, 0),\
                  strlen((char *)sqlite3_column_text(stmt, 0))-6);
            tableau_adresses_fav[i][sizeof(tableau_adresses_fav[i])-1] = '\0';
        }
        // ELIF STOP
    }

    // Reset du stmt
    sqlite3_finalize(stmt);

}

/* --------------------------------------------------------------------------- */
char *Construct_req_station_statuts(int station, char **tableau_adresses_fav)
{
    const char *query_statuts_stations_fav = \
            "SELECT disponible, occupe, en_maintenance, inconnu FROM \
                Stations_fav WHERE adresse_station = ";

    int len_query_base = strlen(query_statuts_stations_fav);
    int len_max_adresse = 100;
    int len_query_station = len_query_base + len_max_adresse;

    char *req = (char *)malloc(len_query_station*sizeof(char));

    strcpy(req, query_statuts_stations_fav);

    strcat(req, "\'");

    strcat(req, tableau_adresses_fav[station]);

    strcat(req, " Paris"); // on l'ajoute vu qu'on le supprime dans Get_adresse

    strcat(req, "\';");

    return req;
}

/* --------------------------------------------------------------------------- */
void Get_statuts_station_fav(sqlite3 *db_belib,\
    char **tableau_adresses_fav, int nb_stations_fav, \
    int nb_rows_par_station, int nb_statuts, \
    int tableau_statuts_fav[nb_stations_fav][nb_rows_par_station][nb_statuts])
{
    
    for (int station = 0; station < nb_stations_fav; station++)
    {
        // Declaration statement
        sqlite3_stmt *stmt_station;

        // Construction requete
        char *req_sql = Construct_req_station_statuts(station, tableau_adresses_fav);
        
        // Test de la requete
        if (sqlite3_prepare_v2(db_belib, req_sql, -1, &stmt_station, NULL))
        {
            printf("Erreur SQL :\n");
            printf("%s : %s\n", sqlite3_errstr(sqlite3_extended_errcode(db_belib)),\
                            sqlite3_errmsg(db_belib));
            sqlite3_close(db_belib);
            exit(EXIT_FAILURE);
        }

        // Application du statement : on recupere les statuts
        for (int t = 0; t < nb_rows_par_station; t++) {
            int step = sqlite3_step(stmt_station);
            if (step == SQLITE_ROW) 
            {
                for (int statut = disponible; statut <= inconnu; statut++) {
                    tableau_statuts_fav[station][t][statut] = \
                    sqlite3_column_int(stmt_station, statut);
                }
            }
            // ELIF STOP
        }

        // Reset du stmt
        sqlite3_finalize(stmt_station);

        free(req_sql);
    }
}

/* --------------------------------------------------------------------------- */
void Get_date_recolte_fav(sqlite3 *db_belib, \
                Date *tableau_date_recolte_fav, int nb_rows_par_station)
{
    // Declaration statement
    sqlite3_stmt *stmt;

    const char *query_adresse_stations_fav = \
                "SELECT DISTINCT(date_recolte) FROM Stations_fav;";

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
            Date date_i;
            Init_Date(&date_i, (char *)sqlite3_column_text(stmt, 0));
            tableau_date_recolte_fav[i] = date_i;
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
            "SELECT COUNT(DISTINCT date_recolte) FROM Stations_fav;";


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
void Print_tableau_fav(int nb_station, int nb_date, int nb_statuts,\
            int tab[nb_station][nb_date][nb_statuts],\
            Date *tableau_date_recolte_fav, char** tableau_adresses_fav)
{
    for (int s = 0; s < nb_station; s++) {
        printf("--------------------------------------------------------\n");
        printf("> Adresse de la station : %s\n", tableau_adresses_fav[s]);
        printf("--------------------------------------------------------\n");
        for (int t = 0; t < nb_date; t++) {
            printf("|   %s -> %d disponible |", tableau_date_recolte_fav[t].datestr,\
             tab[s][t][disponible]);
            printf(" %d occupe | %d maintenance | %d inconnu |\n",\
            tab[s][t][occupe], tab[s][t][en_maintenance], tab[s][t][inconnu]);
        }
        printf("\n");
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
 

/* --------------------------------------------------------------------------- */
void print_Iarr1D(int len_tab, int tab[len_tab], char col)
{
    if (col=='y')
    {
        for (int i=0; i< len_tab; i++ )
            printf("%d\n", tab[i]);
    } else if (col=='n') {
        for (int i=0; i< len_tab; i++ )
            printf("%d, ", tab[i]);
    } else {
        printf("\n");
    }
    printf("\n");
}

/* --------------------------------------------------------------------------- */
void print_Farr1D(int len_tab, float tab[len_tab], char col)
{
    if (col=='y')
    {
        for (int i=0; i< len_tab; i++ )
            printf("%f\n", tab[i]);
    } else if (col=='n') {
        for (int i=0; i< len_tab; i++ )
            printf("%f, ", tab[i]);
    } else {
        printf("\n");
    }
    printf("\n");
}

 
#endif /* GETTER_H */
