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
    // printf(" > Nb rows : %d \n", nb_rows_par_station);

    // Recuperation des adresses des stations en favoris
    char *tableau_adresses_fav[nb_stations_fav];
    Get_adresses_fav(db_belib, tableau_adresses_fav, nb_stations_fav);

    // On retire "Paris" des adresses pour les labels fig
    char *adresse_label[nb_stations_fav];
    for (int i = 0; i < nb_stations_fav; i++) {
        char label_tmp[100];
        int len_adresse = strlen(tableau_adresses_fav[i]);
        slice_str(tableau_adresses_fav[i], label_tmp, 0, len_adresse-6);
        // Stockage des labels
        adresse_label[i] = strdup(label_tmp);
    }

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
    /*Print_tableau_fav(nb_stations_fav, nb_rows_par_station, nb_statuts,\
             tableau_statuts_fav, tableau_date_recolte_fav, tableau_adresses_fav);*/
    
    // Mean avg per hour
    int nb_rows_hours = Get_nb_avg_hours(db_belib);
    // printf("Nb d'heures pour calc moyenne : %d\n", nb_rows_hours);

    // Recuperation vecteur des heures
    int tableau_avg_hours[nb_rows_hours];
    Get_avg_hours(db_belib, nb_rows_hours, tableau_avg_hours);

    // for (int i=0; i < nb_rows_hours; i++)
    //     printf("avg hour %d : %d\n",i,tableau_avg_hours[i]);

    // Recuperation moyenne horaire dispo stations
    float tableau_avg_dispo_station[nb_stations_fav][nb_rows_hours];
    Get_avg_dispo_station(db_belib, \
                        tableau_adresses_fav, \
                        nb_stations_fav, nb_rows_hours, \
                        tableau_avg_dispo_station);

    // for (int station=0; station < nb_stations_fav; station++)
    //     for (int h=0; h < nb_rows_hours; h++)
    //         printf("Avg dispo Station %d à %02d:00 : %.1f \n",station, h, tableau_avg_dispo_station[station][h]);

    // Fermeture db
    sqlite3_close(db_belib);

    // ========================================================================
    // Parametres generaux des figures
    // ========================================================================

    // Parametres generaux
    char *dir_figures= "./figures/"; /**< Path folder save fig*/
    int figsize[2] = {900, 700};     /**< Dimension figure */
    int padX[2] = {90,0};            /**< pad zone de dessin gauche et droite*/
    int padY[2] = {120,160};          /**< pad zone de dessin haut et bas*/
    int margin[2] = {10,10};         /**< margin gauche droite zone de dessin*/

    int w_lines = 3;                 /**< epaisseur des traits*/
    int ms = 6;                      /**< marker size */
    char style_trait = '-';
    
    // ========================================================================
    // Creation de la figure 3 : Variation de la moyenne horaire de dispo
    // ========================================================================

    // Creation de la figure ------------------------------------------------------------
    Figure fig3;
    padY[0] = 120;
    padY[1] = 160;
   
    char wAxes = 'y';
    Init_figure(&fig3, figsize, padX, padY, margin, wAxes);

    /* Make ylabel  ----------  A mettre apres update fig */
    int decalx_Y = 20, decaly_Y = 0;    
    char *ylabel = "Moyenne horaire des bornes disponibles";
    Change_fontsize(&fig3, label_f, 14);    
    Make_ylabel(&fig3, ylabel, decalx_Y, decaly_Y);

    /* Make title */
    char *title = "\u00c9volution de la moyenne horaire des bornes Belib disponibles";
    int decalx_title = -30, decaly_title = 15;
    int *bbox_title;     /**< bbox : so, se, ne, no */
    bbox_title = Make_title(&fig3, title, decalx_title, decaly_title);

    /* Make subtitle */
    Date date_debut;
    Date date_fin;
    Init_Date(&date_debut, tableau_date_recolte_fav[0].datestr);
    Init_Date(&date_fin, tableau_date_recolte_fav[nb_rows_par_station-1].datestr);
        // Construction du sous titre "du .... au ... "
    char subtitle[25] = "";  
    Const_str_dudate1_audate2(&date_debut, &date_fin, subtitle);
    int decalx_subtitle = 0, decaly_subtitle = 0;
    Make_subtitle(&fig3, subtitle, bbox_title, decalx_subtitle, decaly_subtitle);


    // Data
    // Vecteur X = tableau_avg_hours

    // Vecteur Y
    LineStyle flinestyles[nb_stations_fav];  /**< vecteur de linestyle pour chaque station*/
    fLineData flines[nb_stations_fav];

    for (int st = 0; st < nb_stations_fav; st ++)
    {
        style_trait = '-';
        // if (st % 2 != 0) {
        //     style_trait = ':';

        Init_linestyle(&(flinestyles[st]), style_trait, color_lines[st], w_lines,'o', ms);
        Init_flinedata(&(flines[st]), nb_rows_hours, \
                    tableau_avg_hours, \
                    tableau_avg_dispo_station[st],\
                    adresse_label[st], &(flinestyles[st]));
        Add_fline_to_fig(&fig3, &(flines[st]));
    }

    Print_debug_fig(&fig3);

    /* Make Xticks and grid line*/
    Make_xticks_xgrid_time_avgH(&fig3, nb_rows_hours,tableau_avg_hours);

    /* Make Y ticks and grid line*/
    char wTicks = 'n';
    char *path_f_med = "fonts/Lato-Medium.ttf";
    Change_font(&fig3, ticklabel_f, path_f_med);
    Change_fontsize(&fig3, ticklabel_f, 14);    
    Make_fyticks_ygrid(&fig3, wTicks);


    /* Plot lines */
    for (int st = 0; st < nb_stations_fav; st++)
        PlotFLine(&fig3, &(flines[st]));

    /* Make legend */
    int decalx_leg = 0, decaly_leg = 0, ecart = 8;
    Make_legend(&fig3, decalx_leg, decaly_leg, ecart);


     /* Sauvegarde du fichier png */
    const char *filename_fig3= "fig3_avg_hour_dispo.png";
    Save_to_png(&fig3, dir_figures, filename_fig3);

    // Destroying img 
    gdImageDestroy(fig3.img);

    // Clean alloc
    free_tab_char1(tableau_adresses_fav, nb_stations_fav);
    free_tab_char1(adresse_label, nb_stations_fav);


    return 0;
}