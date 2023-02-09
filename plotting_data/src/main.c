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
    printf("Nb d'heures pour calc moyenne : %d\n", nb_rows_hours);

    // Recuperation vecteur des heures
    int tableau_avg_hours[nb_rows_hours];
    Get_avg_hours(db_belib, nb_rows_hours, tableau_avg_hours);

    for (int i=0; i < nb_rows_hours; i++)
        printf("avg hour %d : %d\n",i,tableau_avg_hours[i]);

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

    int w_lines = 5;                 /**< epaisseur des traits*/
    int ms = 6;                      /**< marker size */

    // ========================================================================
    // Creation de la figure 1 : evolution temporelle disponibilite belib fav
    // ========================================================================

    // Creation de la figure ------------------------------------------------------------
    Figure fig1;
    char wAxes = 'n';
    Init_figure(&fig1, figsize, padX, padY, margin, wAxes);

    // Recup vecteur temps
    int vect_time[nb_rows_par_station];
    Get_time_vect(nb_rows_par_station, vect_time, tableau_date_recolte_fav);
    // print_arr1D(nb_rows_par_station, vect_time, 'n');

    // Recup vecteurs Y dans les linedata de la figure 
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
                    st, disponible);

        style_trait = '-';
        // if (st % 2 != 0) {
        //     style_trait = ':';

        Init_linestyle(&(linestyles[st]), style_trait, color_lines[st], w_lines,'o', ms);
        Init_linedata(&(lines[st]), nb_rows_par_station, \
                    vect_time, \
                    vect_nb_dispo[st], adresse_label[st], &(linestyles[st]));
        Add_line_to_fig(&fig1, &(lines[st]));
    }
    
    /* Make ylabel  ----------  A mettre apres update fig */
    int decalx_Y = 20, decaly_Y = 0;    
    char *ylabel = "Bornes disponibles";
    // char *ylabel = "Bornes occupées";
    Change_fontsize(&fig1, label_f, 16);
    Make_ylabel(&fig1, ylabel, decalx_Y, decaly_Y);

    // /* Make xlabel */
    // char *xlabel = "Date";
    // int decalx_X = -5, decaly_X = 15;
    // Make_xlabel(&fig1, xlabel, decalx_X, decaly_X);

    /* Make title */
    char *title = "\u00c9volution du nombre de bornes Belib disponibles (stations favorites)";
    int decalx_title = -30, decaly_title = 15;
    int *bbox_title;     /**< bbox : so, se, ne, no */
    bbox_title = Make_title(&fig1, title, decalx_title, decaly_title);

    /* Make subtitle */
    Date date_debut;
    Date date_fin;
    Init_Date(&date_debut, tableau_date_recolte_fav[0].datestr);
    Init_Date(&date_fin, tableau_date_recolte_fav[nb_rows_par_station-1].datestr);

        // Construction du sous titre "du .... au ... "
    char subtitle[25] = "";  
    Const_str_dudate1_audate2(&date_debut, &date_fin, subtitle);
    
    int decalx_subtitle = 0, decaly_subtitle = 0;
    Make_subtitle(&fig1, subtitle, bbox_title, decalx_subtitle, decaly_subtitle);

    /* Make X ticks and grid line*/
    Make_xticks_xgrid_time(&fig1, tableau_date_recolte_fav[0]);

    /* Make Y ticks and grid line*/
    char wTicks = 'n';
    char *path_f_med = "fonts/Lato-Medium.ttf";
    Change_font(&fig1, ticklabel_f, path_f_med);
    Change_fontsize(&fig1, ticklabel_f, 14);    
    Make_yticks_ygrid(&fig1, wTicks);

    /* Make legend */
    int decalx_leg = 0, decaly_leg = 0, ecart = 8;
    Make_legend(&fig1, decalx_leg, decaly_leg, ecart);

    /* Make github link */
    char *github = "https://github.com/bauj/AJC_projet_belib";
    int decalx_github = 0, decaly_github = 0;
    Make_annotation(&fig1, github, decalx_github, decaly_github);

    /* Make copyright */
    char *sign = "\u00a9 2023 by Juba Hamma";
    int decalx_sign = fig1.img->sx- strlen(sign)*7, decaly_sign = 0;
    Make_annotation(&fig1, sign, decalx_sign, decaly_sign);


    /* Plot lines */
    for (int st = 0; st < nb_stations_fav; st++)
        PlotLine(&fig1, &(lines[st]));

     /* Sauvegarde du fichier png */
    const char *filename_fig1= "fig1_disponible.png";
    Save_to_png(&fig1, dir_figures, filename_fig1);

    /* printf("Résolution de l'img : %d x %d dpi\n", gdImageResolutionX(fig1.img),\
                             gdImageResolutionY(fig1.img) );                           
    */

    /* Destroy the image in memory. */
    gdImageDestroy(fig1.img);


    // ========================================================================
    // Creation de la figure 2 : barplot des statuts des bornes par station
    // pour la derniere recolte
    // ========================================================================

    // Creation de la figure ------------------------------------------------------------
    Figure fig2;
    padY[0] = 90;
    padY[1] = 230;
    wAxes = 'n';
    Init_figure(&fig2, figsize, padX, padY, margin, wAxes);
    
    int nb_tot_bornes;
    // Definition d'un vecteur de bardata pour chaque station
    BarData barplots[nb_stations_fav]; 
        
    // Initialisation de chaque bardata
    for (int st_barplot = 0; st_barplot < nb_stations_fav; st_barplot++) {
        nb_tot_bornes=0;

        for (int statut = disponible; statut <= inconnu; statut ++)
            nb_tot_bornes += tableau_statuts_fav[st_barplot][nb_rows_par_station-1][statut];
            
        // printf("%s \n", new_adresse_label[st_barplot]);

        Init_bardata(&(barplots[st_barplot]), nb_statuts, labels_ctg, nb_tot_bornes,\
             tableau_statuts_fav[st_barplot][nb_rows_par_station-1],\
              color_ctg, adresse_label[st_barplot]);

        // Update des data de l'objet figure (gestion des max, posX des barplot)
        Add_barplot_to_fig(&fig2, &(barplots[st_barplot]));
    }
    for (int st_barplot = 0; st_barplot < nb_stations_fav; st_barplot++)
        printf("%s \n", adresse_label[st_barplot]);

    // // Ajout du ylabel
    // decalx_Y = 10, decaly_Y = 0;    
    // ylabel = "Bornes Belib";
    // Make_ylabel(&fig2, ylabel, decalx_Y, decaly_Y);

    // Ajout des yticks et des ygrid (avant plot pour eviter de plotter par dessus)
    wTicks = 'n';
    Change_font(&fig2, ticklabel_f, path_f_med);
    Change_fontsize(&fig2, ticklabel_f, 14);
    Make_yticks_ygrid(&fig2, wTicks);

    // Ajout des xticks
    float angle_labels = 20.;
    Change_fontsize(&fig2, ticklabel_f, 13);
    Make_xticks_barplot(&fig2, angle_labels);

    /* Make legend */
    Change_font(&fig2, leg_f, path_f_med);
    Change_fontsize(&fig2, leg_f, 13);
    decalx_leg = 0, decaly_leg = 0, ecart = 2;
    Make_legend_barplot(&fig2, decalx_leg, decaly_leg, ecart);

    /* Make github link */
    decalx_github = 0, decaly_github = 0;
    Make_annotation(&fig2, github, decalx_github, decaly_github);

    /* Make copyright */
    Make_annotation(&fig2, sign, decalx_sign, decaly_sign);

    // Plot des barplots
    char wlabels = 'y';
    for (int st_barplot = 0; st_barplot < nb_stations_fav; st_barplot++) {
        // Print_debug_bd(fig2.bardata[st_barplot], 'y');
        PlotBarplot(&fig2, fig2.bardata[st_barplot], wlabels);
    }

    /* Make title */
    title = "Disponibilité des bornes Belib (stations favorites)";
    decalx_title = 0, decaly_title = 0;
    bbox_title = Make_title(&fig2, title, decalx_title, decaly_title);

    /* Make subtitle */
        // Recuperation derniere date de recolte    
    Date last_date_recolte = tableau_date_recolte_fav[nb_rows_par_station-1];
    // Print_debug_date(&last_date_recolte, 'y');

    char subtitle2[70];
    sprintf(subtitle2, "le %02d/%02d/%02d à %02d:%02d",\
                     last_date_recolte.tm.tm_mday,\
                     last_date_recolte.tm.tm_mon+1,\
                     (last_date_recolte.tm.tm_year+1900)%2000,\
                     last_date_recolte.tm.tm_hour,\
                     last_date_recolte.tm.tm_min);

    decalx_subtitle = 0, decaly_subtitle = 0;

    Make_subtitle(&fig2, subtitle2, bbox_title, decalx_subtitle, decaly_subtitle);

     /* Sauvegarde du fichier png */
    const char *filename_fig2= "fig2_barplot.png";
    Save_to_png(&fig2, dir_figures, filename_fig2);

    // Destroying img 
    gdImageDestroy(fig2.img);


    // ========================================================================
    // Creation de la figure 3 : Variation de la moyenne horaire de dispo
    // ========================================================================

    // Creation de la figure ------------------------------------------------------------
    Figure fig3;
    padY[0] = 120;
    padY[1] = 160;
   
    wAxes = 'n';
    Init_figure(&fig3, figsize, padX, padY, margin, wAxes);

    /* Make ylabel  ----------  A mettre apres update fig */
    decalx_Y = 20, decaly_Y = 0;    
    ylabel = "Moyenne horaire des bornes disponibles";
    Change_fontsize(&fig3, label_f, 14);    
    Make_ylabel(&fig3, ylabel, decalx_Y, decaly_Y);

    /* Make title */
    title = "\u00c9volution de la moyenne horaire des bornes Belib disponibles";
    decalx_title = -30, decaly_title = 15;
    bbox_title = Make_title(&fig3, title, decalx_title, decaly_title);

    /* Make subtitle */
        // Construction du sous titre "du .... au ... "
    Make_subtitle(&fig3, subtitle, bbox_title, decalx_subtitle, decaly_subtitle);

    // Data
    // Vecteur X = tableau_avg_hours

    // Vecteur Y
    LineStyle flinestyles[nb_stations_fav];  /**< vecteur de linestyle pour chaque station*/
    fLineData flines[nb_stations_fav];

    w_lines = 3;
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
    wTicks = 'n';
    path_f_med = "fonts/Lato-Medium.ttf";
    Change_font(&fig3, ticklabel_f, path_f_med);
    Change_fontsize(&fig3, ticklabel_f, 14);    
    Make_fyticks_ygrid(&fig3, wTicks);

    /* Plot lines */
    for (int st = 0; st < nb_stations_fav; st++)
        PlotFLine(&fig3, &(flines[st]));

    /* Make legend */
    decalx_leg = 0, decaly_leg = 0, ecart = 8;
    Make_legend(&fig3, decalx_leg, decaly_leg, ecart);

    /* Make github link */
    decalx_github = 0, decaly_github = 0;
    Make_annotation(&fig3, github, decalx_github, decaly_github);

    /* Make copyright */
    Make_annotation(&fig3, sign, decalx_sign, decaly_sign);

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