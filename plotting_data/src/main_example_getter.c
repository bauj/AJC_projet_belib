/* ----------------------------------------------------------------------------
*  Programme permettant d'ouvrir la bdd SQLite contenant les donnees belib et
*  de plot les infos interessantes.
*  
*  Author : Juba Hamma. 2023.
* ---------------------------------------------------------------------------- 
*/

#include <stdlib.h>
#include <sqlite3.h>
#include "libs/traitement.h"
#include "libs/getter.h"
// #include "libs/plotter.h"


/* =========================================================================== */
int main(int argc, char* argv[]) 
{
    // ========================================================================
    // Récupération des données de la db sqlite
    // ========================================================================

    // Recuperation du filepath de la db sqlite
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
    
    // Recuperation du nombre de stations en favoris
    int nb_stations_fav = Get_nb_stations_fav(db_belib);

    // Recuperation du nombre de lignes par station
    int nb_rows_par_station = Get_nb_rows_par_station(db_belib);

    // Recuperation des adresses des stations en favoris
    char *tableau_adresses_fav[nb_stations_fav];
    Get_adresses_fav(db_belib, tableau_adresses_fav, nb_stations_fav);

    // Recuperation des date de recolte de chaque station (same for all)
    Date tableau_date_recolte_fav[nb_rows_par_station];
    Get_date_recolte_fav(db_belib, tableau_date_recolte_fav, nb_rows_par_station);

    // Datetick datetick;
    // Init_Datetick(&datetick, &tableau_date_recolte_fav[1], &tableau_date_recolte_fav[0]);
    // Print_debug_date(&tableau_date_recolte_fav[1], 'n');
    // Print_debug_datetick(&datetick);

    // Recuperation des statuts par station fav
    int nb_statuts = 4; /**< disponible occupe en_maintenance inconnu*/
    int tableau_statuts_fav[nb_stations_fav][nb_rows_par_station][nb_statuts];
    
    // Remplissage du tableau avec la bdd belib_data
    Get_statuts_station_fav(db_belib, \
                                tableau_adresses_fav, nb_stations_fav,\
                                 nb_rows_par_station, nb_statuts,
                                 tableau_statuts_fav);

    // Verif que ca colle avec la db
    Print_tableau_fav(nb_stations_fav, nb_rows_par_station, nb_statuts,\
            tableau_statuts_fav, tableau_date_recolte_fav, tableau_adresses_fav);
    
    // Fermeture db
    sqlite3_close(db_belib);


    // Clean alloc
    free_tab_char1(tableau_adresses_fav, nb_stations_fav);
 

    return 0;
}