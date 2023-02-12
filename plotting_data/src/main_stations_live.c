/* ----------------------------------------------------------------------------
*  Programme permettant d'ouvrir la bdd SQLite contenant les donnees belib et
*  de plot les infos interessantes concernant la table Stations_fav.
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

// #define AJC
#define QEMU
// #define LENOVO

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
    char* table = "Stations_live";
    int nb_stations_fav = Get_nb_stations(db_belib, table);

    if (nb_stations_fav == 0) {
        printf("> Pas de stations trouvées dans la table.\n");
        exit(EXIT_FAILURE);
    }

    // Recuperation du nombre de lignes par station
    int nb_rows_par_station = Get_nb_rows_par_station(db_belib, table);
    // printf(" > Nb rows : %d \n", nb_rows_par_station);

    // Recuperation des adresses des stations en favoris
    char *tableau_adresses_fav[nb_stations_fav];
    Get_adresses(db_belib, table, tableau_adresses_fav, nb_stations_fav);

        
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
    Get_date_recolte(db_belib, table, tableau_date_recolte_fav, nb_rows_par_station);

    // Datetick datetick;
    // Init_Datetick(&datetick, &tableau_date_recolte_fav[1], &tableau_date_recolte_fav[0]);
    // Print_debug_date(&tableau_date_recolte_fav[1], 'n');
    // Print_debug_datetick(&datetick);

    // Recuperation des statuts par station fav
    int nb_statuts = 4; /**< disponible occupe en_maintenance inconnu*/
    int tableau_statuts_fav[nb_stations_fav][nb_rows_par_station][nb_statuts];
    
    // Remplissage du tableau avec la bdd belib_data
    Get_statuts_station(db_belib, table,\
                                tableau_adresses_fav, nb_stations_fav,\
                                 nb_rows_par_station, nb_statuts,
                                 tableau_statuts_fav);

    // Verif que ca colle avec la db
    /* Print_tableau_fav(nb_stations_fav, nb_rows_par_station, nb_statuts,\
              tableau_statuts_fav, tableau_date_recolte_fav, tableau_adresses_fav);*/

    // for (int station=0; station < nb_stations_fav; station++)
    //     for (int h=0; h < nb_rows_hours; h++)
    //         printf("Avg dispo Station %d à %02d:00 : %.1f \n",station, h, tableau_avg_dispo_station[station][h]);

    // Fermeture db
    sqlite3_close(db_belib);

    // ========================================================================
    // Parametres generaux des figures
    // ========================================================================

    // Parametres generaux
    #if defined QEMU
        char *dir_figures= "/var/www/html/figures/"; /**< Path folder save fig*/
    #else
        char *dir_figures= "./figures/"; /**< Path folder save fig*/
    #endif
    
    // char *dir_figures= "./figures/"; /**< Path folder save fig*/
    int figsize[2] = {800, 700};     /**< Dimension figure */
    int padX[2] = {90,0};            /**< pad zone de dessin gauche et droite*/
    int padY[2] = {120,160};          /**< pad zone de dessin haut et bas*/
    int margin[2] = {10,10};         /**< margin gauche droite zone de dessin*/

    // ========================================================================
    // Creation de la figure : barplot des statuts des bornes par station
    // pour la derniere recolte
    // ========================================================================

    // Creation de la figure ------------------------------------------------------------
    Figure fig2;
    padY[0] = 90;
    padY[1] = 230;
    char wAxes = 'n';
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

    // for (int st_barplot = 0; st_barplot < nb_stations_fav; st_barplot++)
    //     printf("%s \n", adresse_label[st_barplot]);

    // // Ajout du ylabel
    // decalx_Y = 10, decaly_Y = 0;    
    // ylabel = "Bornes Belib";
    // Make_ylabel(&fig2, ylabel, decalx_Y, decaly_Y);

    // Ajout des yticks et des ygrid (avant plot pour eviter de plotter par dessus)
    char wTicks = 'n';
    char* path_f_med = fonts_fig[1];
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
    int decalx_leg = 0, decaly_leg = 0, ecart = 2;
    Make_legend_barplot(&fig2, decalx_leg, decaly_leg, ecart);

    /* Make github link */
    char *github = "https://github.com/bauj/AJC_projet_belib";
    int decalx_github = 0, decaly_github = 0;
    Make_annotation(&fig2, github, decalx_github, decaly_github);

    /* Make copyright */
    char *sign = "\u00a9 2023 by Juba Hamma";
    int decalx_sign = fig2.img->sx- strlen(sign)*7, decaly_sign = 0;

    Make_annotation(&fig2, sign, decalx_sign, decaly_sign);

    // Plot des barplots
    char wlabels = 'y';
    for (int st_barplot = 0; st_barplot < nb_stations_fav; st_barplot++) {
        // Print_debug_bd(fig2.bardata[st_barplot], 'y');
        PlotBarplot(&fig2, fig2.bardata[st_barplot], wlabels);
    }

    /* Make title */
    char *title = "Disponibilité des bornes Belib (stations live)";
    int decalx_title = 0, decaly_title = 0;
    int *bbox_title = Make_title(&fig2, title, decalx_title, decaly_title);

    /* Make subtitle */
        // Recuperation derniere date de recolte    
    Date last_date_recolte = tableau_date_recolte_fav[nb_rows_par_station-1];
    // Print_debug_date(&last_date_recolte, 'y');

    char subtitle2[70];
    #ifdef QEMU
        int hour_hack = last_date_recolte.tm.tm_hour+1;
    #else
        int hour_hack = last_date_recolte.tm.tm_hour;
    #endif

    sprintf(subtitle2, "le %02d/%02d/%02d à %02d:%02d",\
                     last_date_recolte.tm.tm_mday,\
                     last_date_recolte.tm.tm_mon+1,\
                     (last_date_recolte.tm.tm_year+1900)%2000,\
                     hour_hack,\
                     last_date_recolte.tm.tm_min);

    int decalx_subtitle = 0, decaly_subtitle = 0;
    Make_subtitle(&fig2, subtitle2, bbox_title, decalx_subtitle, decaly_subtitle);

     /* Sauvegarde du fichier png */
    const char *filename_fig2= "fig2_barplot_live.png";
    Save_to_png(&fig2, dir_figures, filename_fig2);

    // Destroying img 
    gdImageDestroy(fig2.img);


    // Clean alloc
    free_tab_char1(tableau_adresses_fav, nb_stations_fav);
    free_tab_char1(adresse_label, nb_stations_fav);

    return 0;
}