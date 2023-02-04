/* ----------------------------------------------------------------------------
*  Programme permettant d'ouvrir la bdd SQLite contenant les donnees belib et
*  de plot les infos interessantes.
*  
*  Author : Juba Hamma. 2023.
* ---------------------------------------------------------------------------- 
*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "libs/Getter.h"
// #include "libs/Plotter.h"


/* =========================================================================== */
int main(int argc, char* argv[]) 
{
    // ------------------------------------------------------------------------
    // Lecture et recuperation de la base de donnees sqlite 
    // ------------------------------------------------------------------------

    // Recuperation du filepath de la bddsqlite
    char *bdd_filename = argv[1];

    // Test de presence d'un argument
    if (bdd_filename == NULL)
    {
        printf("Erreur : argument non spécifié. Le programme attend le nom d'un \
                        fichier en entrée. \n");
        exit(EXIT_FAILURE);
    }

    // Instanciation db sqlite
    sqlite3 *db_belib;

    // Connexion a la db sqlite
    Sqlite_open_check(bdd_filename, &db_belib);

    // Requete SQL de selection dans la db
    // 'Stations_fav'

    // Recuperation nb stations
    // SELECT COUNT(DISTINCT adresse_station) FROM 'Stations_fav';

    // Recuperation des adresses des stations
    // SELECT adresse_station FROM 'Stations_fav' GROUP BY adresse_station;

    // Recuperation nombre de lignes pour chaque station
    // SELECT COUNT(*) FROM 'Stations_fav' WHERE adresse_station = '108 Rue Balard 75015 Paris';


    // // Recuperation du nombre de stations en favoris
    int nb_stations_fav = Get_nb_stations_fav(db_belib);
    printf("> Nb stations en fav : %d \n", nb_stations_fav);

    // // Recuperation du nombre de lignes par station
    int nb_rows_par_station = Get_nb_rows_par_station(db_belib);
    printf("> Nb lignes par station : %d \n", nb_rows_par_station);

    // Recuperation des adresses des stations en favoris
    char *tableau_adresses_fav[nb_stations_fav];
    Get_adresses_fav(db_belib, tableau_adresses_fav, nb_stations_fav);
    printf("> Adresse %d : %s \n", 1, tableau_adresses_fav[0]);
    printf("> Adresse %d : %s \n", 2, tableau_adresses_fav[1]);             
    printf("> Adresse %d : %s \n", 3, tableau_adresses_fav[2]);
    printf("> Adresse %d : %s \n", 4, tableau_adresses_fav[3]);
    printf("> Adresse %d : %s \n", 5, tableau_adresses_fav[4]);

    // Recuperation des date de recolte de chaque station (same for all)
    char *tableau_date_recolte_fav[nb_rows_par_station];
    Get_date_recolte_fav(db_belib, tableau_date_recolte_fav, nb_rows_par_station);
    printf("> Date recolte %d : %s \n", 7, tableau_date_recolte_fav[6]);
    printf("> Date recolte %d : %s \n", 13, tableau_date_recolte_fav[12]);

    // Recuperation des statuts par station fav
    int nb_statuts = 4; /**< disponible occupe en_maintenance inconnu*/
    // int ***tableau_statuts_fav = (int ***)malloc(nb_stations_fav*nb_statuts*nb_rows_par_station*sizeof(int));

    Get_statuts_station_fav(db_belib, tableau_adresses_fav, nb_stations_fav, \
                    nb_rows_par_station, nb_statuts);

    // for (int station = 0; station < nb_stations_fav; station++)
    // {
    //     Construct_req_station_statuts(station, tableau_adresses_fav);
    // }

    // char *test = sqlite3_expanded_sql(stmt);
    // printf("%s\n",test);

    
    sqlite3_close(db_belib);

    free_tab_char1(tableau_adresses_fav, nb_stations_fav);
    free_tab_char1(tableau_date_recolte_fav, nb_rows_par_station);
    // free_tab_int3(tableau_statuts_fav, nb_stations_fav, nb_statuts, nb_rows_par_station);
 

    return 0;
}