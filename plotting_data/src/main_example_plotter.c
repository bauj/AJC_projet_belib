/* ----------------------------------------------------------------------------
*  Programme permettant de plotter l'evolution de donnees fictives a l'aide de 
* la bibliothèque Plotter.h developpee.
*  
*  Author : Juba Hamma. 2023.
* ---------------------------------------------------------------------------- 
*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libs/Plotter.h"

/* =========================================================================== */
int main(int argc, char* argv[]) 
{
    // data = get_data_from_db(db_path);

    // Change_fig_bg(&fig1, coul_trait);
    // Change_fig_cvs_bg(&fig1, coul_trait2);
    // Change_fig_axes_color(&fig1, coul_trait3);

    // Parametres generaux
    int figsize[2] = {800, 600};
    char *dir_figures= "./figures/";
    int w_lines = 3;
    int ms = 8;
    int pad[2] = {90,80}; /**< pad zone de dessin*/
    int margin[2] = {10,10}; /**< margin zone de dessin*/
    // char* fontLabels = "/home/jhamma/DejaVuSansMono.ttf";
    char* fontLabels = "/usr/share/fonts/dejavu-sans-mono-fonts/DejaVuSansMono.ttf";
    // char* fontLabels = "/usr/share/fonts/truetype/lato/Lato-Light.ttf";
    const int labelSize = 16;

    // char* errStringFT;
    int vert_fonce[3]  = {51, 160,  44};   //vert_fonce
    int orange_clair[3] = {253, 191, 111};  //orange_clair
    int bleu_fonce[3] = {31, 120, 180};   //bleu_fonce
    int orange_fonce[3] = {255, 127,   0};   //orange_fonce
    // int violet_clair[3] = {202, 178, 214};   //violet_clair
    int rouge_fonce[3] = {227,  26,  28};   //rouge_fonce
    int white[3]  = {255, 255, 255};   //white
    int bleu_clair[3] = {166, 206, 227};

    Figure fig1;
    Init_figure(&fig1, figsize, pad, margin);

    int ptx[]  = {0, 100, 200, 300, 400};
    int pty[]  = {5,    9,  0,   7,   1};
    int pty2[] = {1,    4,  7,   0,   6};
    int pty3[] = {5,    1,  4,   9,   6};
    int pty4[] = {1,    6,  1,   2,   1};
    int pty5[] = {10,   5,  4,   6,   7};
    int pty6[] = {10,  9,  3,   8,   5};

    size_t len_data = sizeof(ptx)/sizeof(ptx[0]);

    LineStyle linestyle1;
    Init_linestyle(&linestyle1, ':', vert_fonce, w_lines,'o', ms);
    LineData line1;
    char* label1 = "Rue de la station 1";
    Init_linedata(&line1, len_data, ptx, pty, label1, &linestyle1);
    Update_fig(&fig1, &line1);

    LineStyle linestyle2;
    Init_linestyle(&linestyle2, '-', orange_clair, w_lines,'o', ms);
    LineData line2;
    char* label2 = "Rue de la station 2";
    Init_linedata(&line2, len_data, ptx, pty2, label2, &linestyle2);
    Update_fig(&fig1, &line2);

    LineStyle linestyle3;
    Init_linestyle(&linestyle3, '-', bleu_fonce, w_lines,'o', ms);
    LineData line3;
    char* label3 = "Rue de la station 3";
    Init_linedata(&line3, len_data, ptx, pty3, label3, &linestyle3);    
    Update_fig(&fig1, &line3);

    LineStyle linestyle4;
    Init_linestyle(&linestyle4, '-', orange_fonce, w_lines,'o', ms);
    LineData line4;
    char* label4 = "Rue de la station 4";
    Init_linedata(&line4, len_data, ptx, pty4, label4, &linestyle4);
    Update_fig(&fig1, &line4);

    LineStyle linestyle5;
    Init_linestyle(&linestyle5, '-', rouge_fonce, w_lines,'o', ms);
    LineData line5;
    char* label5 = "Rue de la station 5";
    Init_linedata(&line5, len_data, ptx, pty5, label5, &linestyle5);
    Update_fig(&fig1, &line5);

    LineStyle linestyle6;
    Init_linestyle(&linestyle6, '-', bleu_clair, w_lines,'o', ms);
    LineData line6;
    char* label6 = "Rue de la station 6";
    Init_linedata(&line6, len_data, ptx, pty6, label6, &linestyle6);
    Update_fig(&fig1, &line6);



    PlotLine(&fig1, &line1);
    PlotLine(&fig1, &line2);
    PlotLine(&fig1, &line3);
    PlotLine(&fig1, &line4);
    PlotLine(&fig1, &line5);
    PlotLine(&fig1, &line6);

    /* Make xlabel */
    char *xlabel = "Date";
    int decalx_X = 0, decaly_X = 0;
    Make_xlabel(&fig1, xlabel, fontLabels, labelSize, white, decalx_X, decaly_X);

    /* Make ylabel */
    int decalx_Y = 10, decaly_Y = 0;    
    char *ylabel = "Bornes disponibles";
    Make_ylabel(&fig1, ylabel, fontLabels, labelSize, white, decalx_Y, decaly_Y);

    /* Make yticks */
    Make_yticks_ygrid(&fig1, fontLabels, 12);
    /* Make xticks <- Avant ca, recup data belib, transformer time sur l'axe X*/ 

    /* Make grid */

    /* Make legende !! <- Ajouter label aux linedatas !*/
    int decalx_leg = 0, decaly_leg = 0, ecart = 2;
    Make_legend(&fig1, fontLabels, 10, decalx_leg, decaly_leg, ecart);

    /* Ajouter date de generation de l'image */

    /* Ajouter lien github */

    /* Ajouter titre du graphique */


    /* Sauvegarde du fichier png */
    const char *filename_fig1= "fig1.png";
    Save_to_png(&fig1, dir_figures, filename_fig1);

    /* Destroy the image in memory. */
    gdImageDestroy(fig1.img);

    return 0;
}