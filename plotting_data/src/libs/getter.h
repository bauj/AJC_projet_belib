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
/**
 * @brief Enumeration contenant les statuts récupérés de la db belib
 * 
 */
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
 * @brief Recupere le nombre de lignes de données par station. Cette fonction 
 * compte le nombre de date_recolte distinctes. 
 * 
 * @param db_belib Pointeur type sqlite3 vers la base de donnée
 * @note La bdd est construite de maniere a ce que l'on ait le meme nombre de 
 * date de récolte pour chaque station, et donc le meme nombre de lignes de 
 * données.
 * @return int Nombre de lignes de données par station
 */

/**
 * @brief Recupere le nombre de lignes de données par station. Cette fonction 
 * compte le nombre de date_recolte distinctes. 
 * 
 * @param db_belib Pointeur type sqlite3 vers la base de donnée
 * @note La bdd est construite de maniere a ce que l'on ait le meme nombre de 
 * date de récolte pour chaque station, et donc le meme nombre de lignes de 
 * données.
 * @param table Nom de la table dans la bdd
 * @return int int Nombre de lignes de données par station
 */
int Get_nb_rows_par_station(sqlite3 *db_belib, char *table);

/* --------------------------------------------------------------------------- */


/**
 * @brief Recupere les adresses uniques dans la table spécifiée
 * 
 * @param db_belib Pointeur type sqlite3 vers la base de donnée
 * @param table Nom de la table dans la bdd
 * @param tableau_adresses Tableau des adresses uniques
 * @param nb_stations Nombre de stations associées
 */
void Get_adresses(sqlite3 *db_belib, char *table, char **tableau_adresses, int nb_stations);


/* --------------------------------------------------------------------------- */

/**
 * @brief Récupère les dates de récolte pour l'ensemble des stations de la table spécifiée
 * 
 * @param db_belib Pointeur type sqlite3 vers la base de donnée
 * @param table Nom de la table dans la bdd
 * @param tableau_date_recolte Tablea des dates de récolte
 * @param nb_rows_par_station Nombre de lignes de date par station
 */
void Get_date_recolte(sqlite3 *db_belib, char *table, Date *tableau_date_recolte, int nb_rows_par_station);


/* --------------------------------------------------------------------------- */

/**
 * @brief Construit le tableau contenant l'ensemble des informations qui concernent les stations Belib
 * 
 * @param db_belib Pointeur type sqlite3 vers la base de donnée
 * @param table Nom de la table dans la bdd (Fav ou Live)
 * @param tableau_adresses Tableau des adresses uniques
 * @param nb_stations Nombre de stations
 * @param nb_rows_par_station Nombre de date de recolte par station (data par station)
 * @param nb_statuts Nombre de statuts récupérés.
 * @warning Enum "statuts devant etre mis à jour si on veut plus de statuts"
 * @param tableau_statuts Tableau contenant l'ensemble des informations souhaitées, de dim : nb_station x nb_rows_par_station x nb_statuts
 */
void Get_statuts_station(sqlite3 *db_belib, char *table, char **tableau_adresses, int nb_stations,int nb_rows_par_station, int nb_statuts, int tableau_statuts[nb_stations][nb_rows_par_station][nb_statuts]);


/* --------------------------------------------------------------------------- */
/**
 * @brief Fonction renvoyant la requete sql permettant de récupérer les nombres 
 * de bornes au statut disponible, occupe, en_maintenance ou inconnu pour une 
 * station donnée, en se basant sur l'unicité de son adresse.
 * 
 * @param table Nom de la table
 * @param station Index de la station favorite dans le tableau des adresses des 
 * stations 
 * @param tableau_adresses Tableau des adresses des stations
 * @return char* Requete SQLite
 */
char *Construct_req_station_statuts(char* table, int station, char **tableau_adresses);



/* --------------------------------------------------------------------------- */
/**
 * @brief Fonction renvoyant la requete sql permettant de récupérer la moyenne 
 * horaire des bornes disponibles pour une station donnée, en se basant sur 
 * l'unicité de son adresse.
 * 
 * @param station Index de la station favorite dans le tableau des adresses des 
 * stations favorites
 * @param tableau_adresses_fav Tableau des adresses des stations favorites
 * @warning Utilisé que pour la table Fav
 * @return char* Requete SQLite
 */
char *Construct_req_station_avg_dispo(int station, char **tableau_adresses_fav);
           

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
 * @warning Fonction différente de Get_statuts_station. Permet de récupérer l'info sur une station dans le tableau complet rempli par Get_statuts_station. Peut porter à confusion
 */
void Get_statut_station(int nb_stations, int nb_rows, int nb_statuts,int vect_statut[nb_rows],int tableau_statuts_fav[nb_stations][nb_rows][nb_statuts],int station, int statut);

/**
 * @brief Recupere le nombre de date de récolte pour une station spécifique. Permet de gérer l'arrivée de nouvelles stations dans le périmètre en favori.
 * 
 * @param db_belib Pointeur type sqlite3 vers la base de donnée
 * @param table Nom de la table dans la bdd
 * @param station Index de la station dont on veut récupérer l'information
 * @param tableau_adresses Tableau des adresses des stations (pour recuperer l'adresse a partir de l'index)
 * @return int 
 */
int Get_nb_rows_par_station_unique(sqlite3 *db_belib, char* table,int station, char **tableau_adresses);

/**
 * @brief Recupere le nombre de stations en se basant sur l'unicité de l'adresse
 * 
 * @param db_belib Pointeur type sqlite3 vers la base de donnée
 * @param table Nom de la table dans la bdd
 * @return int Nombre de stations unique
 */
int Get_nb_stations(sqlite3 *db_belib, char* table);

/**
 * @brief Construit le tableau des moyennes horaires de diponibilité des bornes pour chaque station favorite
 * 
 * @param db_belib Pointeur type sqlite3 vers la base de donnée
 * @param tableau_adresses_fav Tableau des adresses des stations favorites
 * @param nb_stations_fav Nombre de stations favorites
 * @param nb_rows_hours Nombre de ligne de données par station
 * @param tableau_avg_dispo_station Tableau des moyennes horaires de disponibilité pour chaque station favorite
 */
void Get_avg_dispo_station(sqlite3 *db_belib,char **tableau_adresses_fav,int nb_stations_fav, int nb_rows_hours, float tableau_avg_dispo_station[nb_stations_fav][nb_rows_hours]);

/**
 * @brief 
 * 
 * @param db_belib Pointeur type sqlite3 vers la base de donnée
 * @param nb_rows_hours Nombre d'heures collectées pour calcul de la moyenne horaire(fonction Get_nb_avg_hours)
 * @param tableau_avg_hours Tableau contenant les horaires où les moyennes horaires de disponibilité sont calculées
 */
void Get_avg_hours(sqlite3 *db_belib, int nb_rows_hours,int tableau_avg_hours[nb_rows_hours]);

/**
 * @brief Recupère le nombre d'heures collectées pour calcul de la moyenne horaire. Correspond à la taille du tableau tableau_avg_hours.
 * 
 * @param db_belib Pointeur type sqlite3 vers la base de donnée
 * @return int Nombre d'heures collectées pour calcul de la moyenne horaire
 */
int Get_nb_avg_hours(sqlite3 *db_belib);


/**
 * @brief Construit un tableau de temps en secondes à partir de la date de début de récolte. Permet d'avoir un référentiel "entier" pour l'affichage en pixel.
 * 
 * @param nb_rows Nombre de dates unique
 * @param vect_time Tableau d'entier rempli avec les temps en secondes depuis la date de récolte initiale
 * @param tableau_date_recolte Tableau d'objet Date contenant les dates de recolte
 */
void Get_time_vect(int nb_rows, int vect_time[nb_rows], Date tableau_date_recolte[nb_rows]);

/* --------------------------------------------------------------------------- */
/**
 * @brief Fonction de debug permettant d'afficher le contenu de la bdd
 * 
 * @param nb_station Nombre de stations
 * @param nb_date Nombre de date de recolte (rows)
 * @param nb_statuts Nombre de statuts
 * @param tab Tableau des statuts (complet) à afficher
 * @param tableau_date_recolte Tableau des date de récolte
 * @param tableau_adresses Tableau contenant les adresses des stations
 */
void Print_tableau_stations(int nb_station, int nb_date, int nb_statuts, int tab[nb_station][nb_date][nb_statuts],Date *tableau_date_recolte, char** tableau_adresses);

/**
 * @brief Fonction de debug permettant d'afficher le contenu d'un vecteur de float
 * 
 * @param len_tab Taille du tableau
 * @param tab Tableau de float à afficher
 * @param col Caractère 'y' ou 'n' pour l'affichage en colonne plutot qu'en ligne
 */
void print_farr1D(int len_tab, float tab[len_tab], char col);

/**
 * @brief Fonction de debug permettant d'afficher le contenu d'un vecteur d'entiers
 * 
 * @param len_tab Taille du tableau
 * @param tab Tableau d'entiers' à afficher
 * @param col Caractère 'y' ou 'n' pour l'affichage en colonne plutot qu'en ligne
 */
void print_arr1D(int len_tab, int tab[len_tab], char col);

/* --------------------------------------------------------------------------- */
// Definition des fonctions
/* --------------------------------------------------------------------------- */


/* --------------------------------------------------------------------------- */
void Get_time_vect(int nb_rows, int vect_time[nb_rows],\
                Date tableau_date_recolte[nb_rows])
{
    for (int i = 0; i < nb_rows; i++)
    {
        Datetick tick_i;
        Init_Datetick(&tick_i,\
                &tableau_date_recolte[i], &tableau_date_recolte[0]);

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
char *Construct_req_station_statuts(char* table, int station, char **tableau_adresses)
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
    strcat(req, tableau_adresses[station]);
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
    char **tableau_adresses, int nb_stations, \
    int nb_rows_par_station, int nb_statuts, \
    int tableau_statuts[nb_stations][nb_rows_par_station][nb_statuts])
{
    
    for (int station = 0; station < nb_stations; station++)
    {

        int nb_date_station = \
                        Get_nb_rows_par_station_unique(db_belib, table, \
                            station, tableau_adresses);

        // Declaration statement
        sqlite3_stmt *stmt_station;

        // Construction requete
        char *req_sql = Construct_req_station_statuts(table, station, tableau_adresses);

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
                        tableau_statuts[station][t][statut] = 0;
                }
            }

            // Application du statement : on recupere les statuts
            for (int t = 0; t < nb_rows_par_station; t++) {
                int step = sqlite3_step(stmt_station);
                if (step == SQLITE_ROW) 
                {
                    for (int statut = disponible; statut <= inconnu; statut++) {
                        tableau_statuts[station][t][statut] = \
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
                        tableau_statuts[station][t][statut] = 0;
                }
            }

            // On remplit avec ce qui est disponible dans la db
            for (int t = 0; t < nb_date_station; t++) {
                int step = sqlite3_step(stmt_station);
                if (step == SQLITE_ROW) 
                {
                    for (int statut = disponible; statut <= inconnu; statut++) {
                        tableau_statuts[station][nb_rows_par_station-nb_date_station+t][statut] = \
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
                Date *tableau_date_recolte, int nb_rows_par_station)
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
            tableau_date_recolte[i] = date_i;
        }
        // ELIF STOP
    }

    // Reset du stmt
    sqlite3_finalize(stmt);
}


/* --------------------------------------------------------------------------- */
void Get_adresses(sqlite3 *db_belib, char* table,\
                char **tableau_adresses, int nb_stations)
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
    for (int i = 0; i < nb_stations; i++) {
        int step = sqlite3_step(stmt);
        if (step == SQLITE_ROW) 
        {
            tableau_adresses[i] = (char *)malloc(len_max*sizeof(char));
            // On supprime " Paris" en fin de chaine
            strcpy(tableau_adresses[i],\
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
            int station, char **tableau_adresses)
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
    strcat(req, "\"");
    strcat(req, tableau_adresses[station]);
    strcat(req, "\";");

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
void Print_tableau_stations(int nb_station, int nb_date, int nb_statuts,\
            int tab[nb_station][nb_date][nb_statuts],\
            Date *tableau_date_recolte, char** tableau_adresses)
{
    for (int s = 0; s < nb_station; s++) {
        printf("--------------------------------------------------------\n");
        printf("> Adresse de la station : %s\n", tableau_adresses[s]);
        printf("--------------------------------------------------------\n");
        for (int t = 0; t < nb_date; t++) {
            printf("|   %s -> %d disponible |", tableau_date_recolte[t].datestr,\
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
