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
 * @brief Liberation de la memoire allouée pour un tableau de strings
 * de dimension 1
 * 
 * @param tableau_str Tableau de strings
 * @param len_tab Nombre de lignes du tableau 1D
 */
void free_tab_char1(char **tableau_str, int len_tab);


/* --------------------------------------------------------------------------- */
/**
 * @brief Fonction ouvrant la bdd et vérifiant si tout se passe bien à 
 * l'ouverture (notamment l'existence du fichier)
 * 
 * @param bdd_filename Chemin vers la base de données 
 * @param db_belib Pointeur de pointeur type sqlite3 vers la bdd
 */
void Sqlite_open_check(char *bdd_filename, sqlite3 **db_belib);

/* --------------------------------------------------------------------------- */
/**
 * @brief Fonction permettant de récupérer le nombre des stations (uniques) 
 * dans la table "Stations_fav"
 * 
 * @param db_belib Pointeur type sqlite3 vers la bdd
 * @return int Nombre de stations favorites
 */
int Get_nb_stations_fav(sqlite3 *db_belib);


/* --------------------------------------------------------------------------- */
/**
 * @brief Recupere le nombre de lignes de données par station. Cette fonction 
 * compte le nombre de date_recolte distinctes. 
 * 
 * @param db_belib Pointeur type sqlite3 vers la base de donnée
 * @note La bdd est construite de maniere a ce que l'on ait le meme nombre de 
 * date de récolte pour chaque station, et donc le meme nombre de lignes de 
 * données.
 * @return int Nombre de lignes de données par station
 */
int Get_nb_rows_par_station(sqlite3 *db_belib, char *table);

/* --------------------------------------------------------------------------- */



void Get_adresses(sqlite3 *db_belib, char *table, \
            char **tableau_adresses_fav, int nb_stations_fav);


/* --------------------------------------------------------------------------- */



void Get_date_recolte(sqlite3 *db_belib, char *table,\
            Date *tableau_date_recolte_fav, int nb_rows_par_station);


/* --------------------------------------------------------------------------- */



void Get_statuts_station(sqlite3 *db_belib, char *table, char **tableau_adresses_fav,\
     int nb_stations_fav,int nb_rows_par_station, int nb_statuts,\
     int tableau_statuts_fav[nb_stations_fav][nb_rows_par_station][nb_statuts]);


/* --------------------------------------------------------------------------- */
/**
 * @brief Fonction renvoyant la requete sql permettant de récupérer les nombres 
 * de bornes au statut disponible, occupe, en_maintenance ou inconnu pour une 
 * station donnée, en se basant sur l'unicité de son adresse.
 * 
 * @param station Index de la station favorite dans le tableau des adresses des 
 * stations favorites
 * @param tableau_adresses_fav Tableau des adresses des stations favorites 
 * @return char* Requete SQLite
 */
char *Construct_req_station_statuts(char* table, int station, char **tableau_adresses_fav);



/* --------------------------------------------------------------------------- */
/**
 * @brief Fonction renvoyant la requete sql permettant de récupérer la moyenne 
 * horaire des bornes disponibles pour une station donnée, en se basant sur 
 * l'unicité de son adresse.
 * 
 * @param station Index de la station favorite dans le tableau des adresses des 
 * stations favorites
 * @param tableau_adresses_fav Tableau des adresses des stations favorites 
 * @return char* Requete SQLite
 */
char *Construct_req_station_avg_dispo(int station, char **tableau_adresses_fav);



/* --------------------------------------------------------------------------- */
/**
 * @brief Fonction de debug permettant d'afficher le contenu de la bdd
 * 
 * @param nb_station 
 * @param nb_date 
 * @param nb_statuts 
 * @param tab 
 * @param tableau_date_recolte_fav 
 * @param tableau_adresses_fav 
 */
void Print_tableau_fav(int nb_station, int nb_date, int nb_statuts, int tab[nb_station][nb_date][nb_statuts],Date *tableau_date_recolte_fav, char** tableau_adresses_fav);
            

/* --------------------------------------------------------------------------- */
/**
 * @brief Construction d'un vecteur 1D contenant le nombre de bornes ayant un 
 * statut spécifié, pour une station donnée
 * 
 * @param nb_stations Nombre de stations favorites
 * @param nb_rows Nombre de lignes de données pour chaque station
 * @param nb_statuts Nombre de statuts récupérés
 * @param vect_statut Vecteur qui contiendra le nombre de bornes ayant un statut 
 * particulier pour une station donnée
 * @param tableau_statuts_fav Tableau contenant l'ensemble des données récupérées
 *  de la base de données
 * @param station Station prise en compte dans la construction du vecteur 
 * vect_statut
 * @param statut Les bornes ayant ce statut seront comptabilisées
 */
void Get_statut_station(int nb_stations, int nb_rows, int nb_statuts,int vect_statut[nb_rows],int tableau_statuts_fav[nb_stations][nb_rows][nb_statuts],int station, int statut);

int Get_nb_rows_par_station_unique(sqlite3 *db_belib, char* table, \
            int station, char **tableau_adresses_fav);

/* --------------------------------------------------------------------------- */
// Definition des fonctions
/* --------------------------------------------------------------------------- */


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
    for (int i = 0; i < nb_rows; i++) {
        vect_statut[i] = tableau_statuts_fav[station][i][statut];
    }
}

/* --------------------------------------------------------------------------- */
int Get_nb_avg_hours(sqlite3 *db_belib)
{
        // Declaration statement
    sqlite3_stmt *stmt;

    int nb_avg_hours = 0;

    char *query_nb_avg_hours = \
            "SELECT COUNT(DISTINCT(strftime(\'%H\', date_recolte))) as Hour FROM Stations_fav;";

    // Test de la requete
    if (sqlite3_prepare_v2(db_belib, query_nb_avg_hours,-1, &stmt, NULL))
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
        nb_avg_hours = sqlite3_column_int(stmt, 0);
    }
    // printf("Nb station favs : %d \n",sqlite3_column_int(stmt, 0));

    // Reset du stmt
    sqlite3_finalize(stmt);

    return nb_avg_hours;
}

/* --------------------------------------------------------------------------- */
void Get_avg_hours(sqlite3 *db_belib, int nb_rows_hours,\
                        int tableau_avg_hours[nb_rows_hours])
{
    // Declaration statement
    sqlite3_stmt *stmt;

    char *query_avg_hours = \
        "SELECT (strftime(\'%H\', date_recolte)) as Hour FROM Stations_fav GROUP BY (strftime(\'%H\', date_recolte));";

    // Test de la requete
    if (sqlite3_prepare_v2(db_belib, query_avg_hours, -1, &stmt, NULL))
    {
        printf("Erreur SQL :\n");
        printf("%s : %s\n", sqlite3_errstr(sqlite3_extended_errcode(db_belib)),\
                         sqlite3_errmsg(db_belib));
        sqlite3_close(db_belib);
        exit(EXIT_FAILURE);
    }

    // Application du statement et fermeture de la db
    for (int i = 0; i < nb_rows_hours; i++) {
        int step = sqlite3_step(stmt);
        if (step == SQLITE_ROW) 
        {
            tableau_avg_hours[i] = (int) strtol( (char *)sqlite3_column_text(stmt, 0), NULL, 10);
        }
        // ELIF STOP
    }

    // Reset du stmt
    sqlite3_finalize(stmt);    
}

/* --------------------------------------------------------------------------- */
char *Construct_req_station_statuts(char* table, int station, char **tableau_adresses_fav)
{
    char query_statuts_stations[250] = \
            "SELECT disponible, occupe, en_maintenance, inconnu FROM ";

    strcat(query_statuts_stations, table);
    strcat(query_statuts_stations, " WHERE adresse_station = ");  

    int len_query_base = strlen(query_statuts_stations);
    int len_max_adresse = 200;
    int len_query_station = len_query_base + len_max_adresse;

    char *req = malloc(len_query_station*sizeof(char));

    strcpy(req, query_statuts_stations);
    strcat(req, "\"");
    strcat(req, tableau_adresses_fav[station]);
    strcat(req, "\";");

    return req;
}


/* --------------------------------------------------------------------------- */
char *Construct_req_station_avg_dispo(int station, char **tableau_adresses_fav)
{
    char *query_statuts_stations_avg_dispo = \
            "SELECT AVG(disponible) as Avg_dispo FROM Stations_fav WHERE adresse_station = "; 


    int len_query_base = strlen(query_statuts_stations_avg_dispo);
    int len_max_adresse = 200; // comprend le reste de la req plus bas
    int len_query_station = len_query_base + len_max_adresse;

    char *req = malloc(len_query_station*sizeof(char));

    strcpy(req, query_statuts_stations_avg_dispo);
    strcat(req, "\'");
    strcat(req, tableau_adresses_fav[station]);
    strcat(req, "\' ");
    strcat(req, "GROUP BY strftime(\'%H\', date_recolte);");

    return req;
}


/* --------------------------------------------------------------------------- */
void Get_avg_dispo_station(sqlite3 *db_belib,\
                        char **tableau_adresses_fav,\
                        int nb_stations_fav, int nb_rows_hours, \
                        float tableau_avg_dispo_station[nb_stations_fav][nb_rows_hours])
{
    for (int station = 0; station < nb_stations_fav; station++)
    {
        // Declaration statement
        sqlite3_stmt *stmt_station;

        // Construction requete
        char *req_sql = Construct_req_station_avg_dispo(station, tableau_adresses_fav);
        // printf("Req : %s \n", req_sql);

        // Test de la requete
        if (sqlite3_prepare_v2(db_belib, req_sql, -1, &stmt_station, NULL))
        {
            printf("Erreur SQL :\n");
            printf("%s : %s\n", sqlite3_errstr(sqlite3_extended_errcode(db_belib)),\
                            sqlite3_errmsg(db_belib));
            sqlite3_close(db_belib);
            exit(EXIT_FAILURE);
        }

        // Initialisation du tableau : utile lorsque de nouvelles stations pop
        for (int h = 0; h < nb_rows_hours; h++) {
            tableau_avg_dispo_station[station][h] = 0.;
        }

        // Application du statement : on recupere les statuts
        for (int h = 0; h < nb_rows_hours; h++) {
            int step = sqlite3_step(stmt_station);
            if (step == SQLITE_ROW) {
                // printf("SQL Avg dispo Station %d à %02d:00 : %.1f \n", station,+1 h, (float)sqlite3_column_double(stmt_station, 0));
                tableau_avg_dispo_station[station][h] = \
                        (float)sqlite3_column_double(stmt_station, 0);
            }
        }

        // Reset du stmt
        sqlite3_finalize(stmt_station);

        free(req_sql);
    }
}

/* --------------------------------------------------------------------------- */
void Get_statuts_station(sqlite3 *db_belib, char *table,\
    char **tableau_adresses_fav, int nb_stations_fav, \
    int nb_rows_par_station, int nb_statuts, \
    int tableau_statuts_fav[nb_stations_fav][nb_rows_par_station][nb_statuts])
{
    
    for (int station = 0; station < nb_stations_fav; station++)
    {

        int nb_date_station = \
                        Get_nb_rows_par_station_unique(db_belib, table, \
                            station, tableau_adresses_fav);

        // Declaration statement
        sqlite3_stmt *stmt_station;

        // Construction requete
        char *req_sql = Construct_req_station_statuts(table, station, tableau_adresses_fav);

        // Test de la requete
        if (sqlite3_prepare_v2(db_belib, req_sql, -1, &stmt_station, NULL))
        {
            printf("Erreur SQL :\n");
            printf("%s : %s\n", sqlite3_errstr(sqlite3_extended_errcode(db_belib)),\
                            sqlite3_errmsg(db_belib));
            sqlite3_close(db_belib);
            exit(EXIT_FAILURE);
        }

        
        if (nb_date_station == nb_rows_par_station) {
            // Initialisation
            for (int t = 0; t < nb_rows_par_station; t++) {
                for (int statut = disponible; statut <= inconnu; statut++) {
                        tableau_statuts_fav[station][t][statut] = 0;
                }
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
            }
        } else {
            // Gestion des nouvelles stations qui pop
            printf("nb recolte       : %d \n", nb_date_station);
            printf("nb total recolte : %d \n", nb_rows_par_station);
            // Initialisation a 0 avant le debut de recolte
            for (int t = 0; t < nb_rows_par_station-nb_date_station; t++) {
                for (int statut = disponible; statut <= inconnu; statut++) {
                        tableau_statuts_fav[station][t][statut] = 0;
                }
            }

            // On remplit avec ce qui est disponible dans la db
            for (int t = 0; t < nb_date_station; t++) {
                int step = sqlite3_step(stmt_station);
                if (step == SQLITE_ROW) 
                {
                    for (int statut = disponible; statut <= inconnu; statut++) {
                        tableau_statuts_fav[station][nb_rows_par_station-nb_date_station+t][statut] = \
                        sqlite3_column_int(stmt_station, statut);
                    }
                }
            }

        }

        // Reset du stmt
        sqlite3_finalize(stmt_station);

        free(req_sql);
    }
}

/* --------------------------------------------------------------------------- */
void Get_date_recolte(sqlite3 *db_belib, char *table,\
                Date *tableau_date_recolte_fav, int nb_rows_par_station)
{
    // Declaration statement
    sqlite3_stmt *stmt;

    char query_date_recolte_fav[250] = \
            "SELECT  DISTINCT(date_recolte) FROM ";
    
    strcat(query_date_recolte_fav, table);
    strcat(query_date_recolte_fav, ";");
    
    // Test de la requete
    if (sqlite3_prepare_v2(db_belib, query_date_recolte_fav, -1, &stmt, NULL))
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
void Get_adresses(sqlite3 *db_belib, char* table,\
                char **tableau_adresses_fav, int nb_stations_fav)
{

    int len_max = 100;
    // Declaration statement
    sqlite3_stmt *stmt;

    char query_adresse_stations_fav[250] = \
            "SELECT  DISTINCT(adresse_station) FROM ";
    
    strcat(query_adresse_stations_fav, table);
    strcat(query_adresse_stations_fav, ";");

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
            strcpy(tableau_adresses_fav[i],\
                (char *)sqlite3_column_text(stmt, 0)); 
                //, strlen((char *)sqlite3_column_text(stmt, 0))-6);
        }
        // ELIF STOP
    }

    // Reset du stmt
    sqlite3_finalize(stmt);

}


/* --------------------------------------------------------------------------- */
int Get_nb_rows_par_station_unique(sqlite3 *db_belib, char* table, \
            int station, char **tableau_adresses_fav)
{

    // Declaration statement
    sqlite3_stmt *stmt;

    int nb_rows_par_station = 0;

    char query_nb_row_par_station_unique[250] = \
            "SELECT COUNT(DISTINCT date_recolte) FROM ";
    
    strcat(query_nb_row_par_station_unique, table);
    strcat(query_nb_row_par_station_unique, " WHERE adresse_station = ");  

    int len_query_base = strlen(query_nb_row_par_station_unique);
    int len_max_adresse = 200;
    int len_query_station = len_query_base + len_max_adresse;

    char *req = malloc(len_query_station*sizeof(char));

    strcpy(req, query_nb_row_par_station_unique);
    strcat(req, "\'");
    strcat(req, tableau_adresses_fav[station]);
    strcat(req, "\';");

    // Test de la requete
    if (sqlite3_prepare_v2(db_belib,req,-1, &stmt, NULL))
    {
        printf("Erreur SQL unique:\n");
        printf("%s : %s\n", sqlite3_errstr(sqlite3_extended_errcode(db_belib)),\
                         sqlite3_errmsg(db_belib));
        sqlite3_close(db_belib);
        exit(EXIT_FAILURE);
    }
    // Application du statement et fermeture de la db
    int step = sqlite3_step(stmt);

    if (step == SQLITE_ROW) {
        nb_rows_par_station = sqlite3_column_int(stmt, 0);
    }

    // Reset du stmt
    sqlite3_finalize(stmt);

    free(req);

    return nb_rows_par_station;
}


/* --------------------------------------------------------------------------- */
int Get_nb_rows_par_station(sqlite3 *db_belib, char* table)
{

    // Declaration statement
    sqlite3_stmt *stmt;

    int nb_rows_par_station = 0;

    char query_nb_row_par_station[250] = \
            "SELECT COUNT(DISTINCT date_recolte) FROM ";
    
    strcat(query_nb_row_par_station, table);
    strcat(query_nb_row_par_station, ";");
    


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
int Get_nb_stations(sqlite3 *db_belib, char* table)
{

    // Declaration statement
    sqlite3_stmt *stmt;

    int nb_stations_favs = 0;

    char query_nb_stations[250] = \
            "SELECT COUNT(DISTINCT adresse_station) FROM ";
    
    strcat(query_nb_stations, table);
    strcat(query_nb_stations, ";");

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
void print_arr1D(int len_tab, int tab[len_tab], char col)
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
void print_farr1D(int len_tab, float tab[len_tab], char col)
{
    if (col=='y')
    {
        for (int i=0; i< len_tab; i++ )
            printf("%.1f\n", tab[i]);
    } else if (col=='n') {
        for (int i=0; i< len_tab; i++ )
            printf("%.1f, ", tab[i]);
    } else {
        printf("\n");
    }
    printf("\n");
}

#endif /* GETTER_H */
