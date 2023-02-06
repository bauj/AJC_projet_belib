/* ----------------------------------------------------------------------------
*  Programme permettant d'ouvrir la bdd SQLite contenant les donnees belib et
*  de plot les infos interessantes.
*  
*  Author : Juba Hamma. 2023.
* ---------------------------------------------------------------------------- 
*/

#include <stdlib.h>
#include <sqlite3.h>
#include "libs/consts.h"
#include "libs/traitement.h"
#include "libs/getter.h"
#include "libs/plotter.h"


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
    // Print_tableau_fav(nb_stations_fav, nb_rows_par_station, nb_statuts,\
    //         tableau_statuts_fav, tableau_date_recolte_fav, tableau_adresses_fav);
    
    // Fermeture db
    sqlite3_close(db_belib);

    // ========================================================================
    // Creation de la figure 1 : evolution temporelle disponibilite belib fav
    // ========================================================================

    // Parametres generaux
    char *dir_figures= "./figures/"; /**< Path folder save fig*/
    int figsize[2] = {800, 600};     /**< Dimension figure */
    int pad[2] = {90,90};            /**< pad zone de dessin*/
    int margin[2] = {10,10};         /**< margin zone de dessin*/
    int labelSize = 16;              /**< taille de police labels*/
    int tickSize = 12;               /**< taille de police des ticks*/
    int w_lines = 2;                 /**< epaisseur des traits*/
    int ms = 3;                      /**< marker size */
    char* fontLabels = "/usr/share/fonts/dejavu-sans-mono-fonts/DejaVuSansMono.ttf"; /**< chemin vers police 1 */

    // Creation de la figure
    Figure fig1;
    Init_figure(&fig1, figsize, pad, margin);

    /* Make ylabel  ----------  A mettre apres update fig */
    int decalx_Y = 10, decaly_Y = 0;    
    char *ylabel = "Bornes disponibles";
    // char *ylabel = "Bornes occupées";
    Make_ylabel(&fig1, ylabel, fontLabels, labelSize, white, decalx_Y, decaly_Y);


    /* Make xlabel */
    char *xlabel = "Date";
    int decalx_X = 0, decaly_X = 0;
    Make_xlabel(&fig1, xlabel, fontLabels, labelSize, white, decalx_X, decaly_X);


    // Recup vecteur temps
    int vect_time[nb_rows_par_station];
    Get_time_vect(nb_rows_par_station, vect_time, tableau_date_recolte_fav);
    print_arr1D(nb_rows_par_station, vect_time, 'n');

    // Recup vecteurs Y
    int vect_nb_dispo[nb_stations_fav][nb_rows_par_station]; /**< vecteur nb _disponible 
    par station*/
    char style_trait;
    LineData lines[nb_stations_fav]; /**< vecteur de linedata pour chaque station*/
    LineStyle linestyles[nb_stations_fav];  /**< vecteur de linestyle pour chaque station*/

    for (int st = 0; st < nb_stations_fav; st ++)
    {
        Get_statut_station(nb_stations_fav, nb_rows_par_station, nb_statuts,\
                    vect_nb_dispo[st], \
                    tableau_statuts_fav,
                    st, occupe);

        if (st % 2 != 0) {
            printf("st     : %d \n", st);
            printf("modulo : %d \n", st % 2);
            style_trait = ':';
        } else {
            style_trait = '-';
        }
        Init_linestyle(&(linestyles[st]), style_trait, color_lines[st], w_lines,'o', ms);
        Init_linedata(&(lines[st]), nb_rows_par_station, \
                    vect_time, \
                    vect_nb_dispo[st], tableau_adresses_fav[st], &(linestyles[st]));
        Update_fig(&fig1, &(lines[st]));
    }

    // for (int st = 0; st < nb_stations_fav; st++)
    //     Print_debug_ld(fig1.linedata[st], 'y');

    /* Make X ticks and grid line*/
    Make_xticks_xgrid(&fig1, fontLabels, tickSize, tableau_date_recolte_fav[0]);

    /* Make Y ticks and grid line*/
    Make_yticks_ygrid(&fig1, fontLabels, tickSize);

    /* Make legend */
    int decalx_leg = 0, decaly_leg = 0, ecart = 2;
    Make_legend(&fig1, fontLabels, 10, decalx_leg, decaly_leg, ecart);

    /* Plot lines */
    for (int st = 0; st < nb_stations_fav; st++)
    {
        PlotLine(&fig1, &(lines[st]));
    }
    PlotLine(&fig1, &(lines[0]));
    PlotLine(&fig1, &(lines[0]));
    PlotLine(&fig1, &(lines[0]));
    PlotLine(&fig1, &(lines[0]));
    PlotLine(&fig1, &(lines[0]));
    PlotLine(&fig1, &(lines[0]));
    PlotLine(&fig1, &(lines[0]));
    PlotLine(&fig1, &(lines[0]));
    PlotLine(&fig1, &(lines[0]));
    PlotLine(&fig1, &(lines[0]));
    PlotLine(&fig1, &(lines[0]));
    PlotLine(&fig1, &(lines[0]));
    PlotLine(&fig1, &(lines[0]));
    PlotLine(&fig1, &(lines[0]));
    PlotLine(&fig1, &(lines[0]));

     /* Sauvegarde du fichier png */
    // const char *filename_fig1= "fig1_occupe.png";
    const char *filename_fig1= "fig1_disponible.png";
    Save_to_png(&fig1, dir_figures, filename_fig1);

    /* Destroy the image in memory. */
    gdImageDestroy(fig1.img);

    
    // Clean alloc
    free_tab_char1(tableau_adresses_fav, nb_stations_fav);

    return 0;
}