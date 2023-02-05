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
    int pad[2] = {90,80};            /**< pad zone de dessin*/
    int margin[2] = {10,10};         /**< margin zone de dessin*/
    int labelSize = 16;              /**< taille de police labels*/
    int tickSize = 12;               /**< taille de police des ticks*/
    int w_lines = 3;                 /**< epaisseur des traits*/
    int ms = 8;                      /**< marker size */
    char* fontLabels = "/usr/share/fonts/dejavu-sans-mono-fonts/DejaVuSansMono.ttf"; /**< chemin vers police 1 */

    // Creation de la figure
    Figure fig1;
    Init_figure(&fig1, figsize, pad, margin);

    
    int ptx[]  = {0, 100, 200, 300, 400};
    int pty[]  = {5,    9,  0,   7,   1,5,    9,  0,   7,   1,5,    9,  0,   7,   1,5,    9,  0,   7,   1,5,    9,  0,   7,   1};
    int pty2[] = {1,    4,  7,   0,   6,1,    4,  7,   0,   6,1,    4,  7,   0,   6,1,    4,  7,   0,   6,1,    4,  7,   0,   6};
    int pty3[] = {5,    1,  4,   9,   6,5,    1,  4,   9,   6,5,    1,  4,   9,   6,5,    1,  4,   9,   6,5,    1,  4,   9,   6};
    int pty4[] = {1,    6,  1,   2,   1,1,    6,  1,   2,   1,1,    6,  1,   2,   1,1,    6,  1,   2,   1,1,    6,  1,   2,   1};
    int pty5[] = {10,   5,  4,   6,   7,10,   5,  4,   6,   7,10,   5,  4,   6,   7,10,   5,  4,   6,   7,10,   5,  4,   6,   7};
    int pty6[] = {10,  9,  3,   8,    5, 10,  9,  3,   8,    5, 10,  9,  3,   8,    5,10,  9,  3,   8,    5,10,  9,  3,   8,    5};


    // Courbe station 1

    // Recup vecteur temps
    int vect_time[nb_rows_par_station];
    Get_time_vect(nb_rows_par_station, vect_time, tableau_date_recolte_fav);

    print_arr1D(nb_rows_par_station, vect_time, 'n');

    // Recup vecteur data (Y)
    int vect_nb_dispo[nb_stations_fav][nb_rows_par_station];

    Get_statut_station(nb_stations_fav, nb_rows_par_station, nb_statuts,\
            vect_nb_dispo[0], \
            tableau_statuts_fav,
            0, disponible);

    print_arr1D(nb_rows_par_station, vect_nb_dispo, 'n');

    size_t len_data = nb_rows_par_station; //sizeof(ptx)/sizeof(ptx[0]);

    LineStyle linestyle1;
    Init_linestyle(&linestyle1, ':', vert_fonce, w_lines,'o', ms);
    LineData line1;
    char* label1 = "Rue de la station 1";
    Init_linedata(&line1, len_data, vect_time,pty, label1, &linestyle1);
    Update_fig(&fig1, &line1);

    LineStyle linestyle2;
    Init_linestyle(&linestyle2, '-', orange_clair, w_lines,'o', ms);
    LineData line2;
    char* label2 = "Rue de la station 2";
    Init_linedata(&line2, len_data, vect_time, pty2, label2, &linestyle2);
    Update_fig(&fig1, &line2);

    LineStyle linestyle3;
    Init_linestyle(&linestyle3, '-', bleu_fonce, w_lines,'o', ms);
    LineData line3;
    char* label3 = "Rue de la station 3";
    Init_linedata(&line3, len_data, vect_time, pty3, label3, &linestyle3);    
    Update_fig(&fig1, &line3);

    LineStyle linestyle4;
    Init_linestyle(&linestyle4, '-', orange_fonce, w_lines,'o', ms);
    LineData line4;
    char* label4 = "Rue de la station 4";
    Init_linedata(&line4, len_data, vect_time, pty4, label4, &linestyle4);
    Update_fig(&fig1, &line4);

    LineStyle linestyle5;
    Init_linestyle(&linestyle5, '-', rouge_fonce, w_lines,'o', ms);
    LineData line5;
    char* label5 = "Rue de la station 5";
    Init_linedata(&line5, len_data, vect_time, pty5, label5, &linestyle5);
    Update_fig(&fig1, &line5);

    LineStyle linestyle6;
    Init_linestyle(&linestyle6, '-', bleu_clair, w_lines,'o', ms);
    LineData line6;
    char* label6 = "Rue de la station 6";
    Init_linedata(&line6, len_data, vect_time, pty6, label6, &linestyle6);
    Update_fig(&fig1, &line6);

    
    // print_arr1D(nb_rows_par_station, vect_time, 'n');

    // // Recup vecteur data (Y)

    // // LineData lines[nb_stations_fav];
    // // LineStyle linestyles[nb_stations_fav];

    // int vect_nb_dispo[nb_stations_fav][nb_rows_par_station];

    // // Update fig data : Don't plot inside

    
    // int s;
    // s = 0;
    // LineStyle style1;
    // Init_linestyle(&style1, '-', white, w_lines,'o', ms);

    // LineData line1;
    // Get_statut_station(nb_stations_fav, nb_rows_par_station, nb_statuts,\
    //         vect_nb_dispo[s], \
    //         tableau_statuts_fav,
    //         s, disponible);

    // Init_linedata(&line1, nb_rows_par_station, \
    //             vect_time, \
    //             vect_nb_dispo[s], tableau_adresses_fav[s], &style1);
                

    // Update_fig(&fig1, &line1);
  
    // // Plotting after updating fig data
    // // for (station = 0; station < nb_stations_fav; station ++)
    // // {
    // //     PlotLine(&fig1, fig1.linedata[station]);
    // // }


    PlotLine(&fig1, &line1);
    PlotLine(&fig1, &line1);
    PlotLine(&fig1, &line1);
    PlotLine(&fig1, &line1);
    PlotLine(&fig1, &line1);
    // PlotLine(&fig1, &line1);
    // PlotLine(&fig1, &line1);
    // PlotLine(&fig1, &line2);
    // PlotLine(&fig1, &line3);
    // PlotLine(&fig1, &line4);
    // PlotLine(&fig1, &line5);
    // PlotLine(&fig1, &line6);
    // PlotLine(&fig1, &line6);
    // PlotLine(&fig1, &line6);
    // PlotLine(&fig1, &line6);
    // PlotLine(&fig1, &line6);
    // PlotLine(&fig1, &line6);
    // PlotLine(&fig1, &line6);
    // PlotLine(&fig1, &line6);
    // PlotLine(&fig1, &line6);
    // PlotLine(&fig1, &line6);
    // PlotLine(&fig1, &line6);
    // PlotLine(&fig1, &line6);
    // PlotLine(&fig1, &line6);
    // PlotLine(&fig1, &line6);
    // PlotLine(&fig1, &line6);
    // PlotLine(&fig1, &line6);

    // PlotLine(&fig1, &line1);
    // PlotLine(&fig1, &line1);
    // PlotLine(&fig1, &line1);
    // PlotLine(&fig1, &line1);


    /* Make xlabel */
    char *xlabel = "Date";
    int decalx_X = 0, decaly_X = 0;
    Make_xlabel(&fig1, xlabel, fontLabels, labelSize, white, decalx_X, decaly_X);

    /* Make ylabel */
    int decalx_Y = 10, decaly_Y = 0;    
    char *ylabel = "Bornes disponibles";
    Make_ylabel(&fig1, ylabel, fontLabels, labelSize, white, decalx_Y, decaly_Y);
    
    Make_yticks_ygrid(&fig1, fontLabels, tickSize);

    int decalx_leg = 0, decaly_leg = 0, ecart = 2;
    Make_legend(&fig1, fontLabels, 10, decalx_leg, decaly_leg, ecart);



    
    


     /* Sauvegarde du fichier png */
    const char *filename_fig1= "fig1.png";
    Save_to_png(&fig1, dir_figures, filename_fig1);

    /* Destroy the image in memory. */
    gdImageDestroy(fig1.img);

    
    // Clean alloc
    free_tab_char1(tableau_adresses_fav, nb_stations_fav);

    return 0;
}