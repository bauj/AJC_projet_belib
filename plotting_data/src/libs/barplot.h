/* ----------------------------------------------------------------------------
*  Bibliotheque de plot pour donnees barplot.
*  
*  Author : Juba Hamma. 2023.
* ---------------------------------------------------------------------------- 
*/

#ifndef BPLOT_H
#define BPLOT_H

#include "plotter.h"

/* --------------------------------------------------------------------------- */
/**
 * @brief Structure stockant des donnees par categorie en les associant a des 
 * couleurs dans le but de tracer un BarPlot les représentant.
 * Dans le cadre de Belib : un BarData par station.
 * 
 */
typedef struct BarData_s {
    size_t nb_ctg;        /**< Nombre de categories*/
    char **ctg_names;     /**< Labels des categories*/
    size_t nb_tot;        /**< Nombre total d'éléments dans l'ensemble des ctg*/
    int *nb_in_ctg;        /**< Vecteur contenant le nombre d'elements par ctg */
    int (*colors)[3];        /**< Vecteur contenant une couleur pour chaque ctg*/
    char* label;           /**< Label associé au BarData */
    int idx;                /**< Index donné lorsqu'ajouté à la figure (pour posX)*/
} BarData; 


/* --------------------------------------------------------------------------- */
/**
 * @brief Structure stockant un ensemble de LineData et de données relatives à la
 * création d'une figure
 * 
 */
typedef struct FigureBarPlot_s {
    Figure *fig;        /**< Pointeur sur objet gdImage */
    size_t nb_bardata;  /**< Nombre de linedata dans la figure */
    BarData **bardata;  /**< Vecteur de LineData */
    int max_Y;        /**< max de l'ensemble des max_Y (float) de linedata[] */
    int max_X;        /**< max de l'ensemble des max_Y (float) de linedata[] */
} FigureBarPlot;



/* --------------------------------------------------------------------------- */
void Init_fig_BarPlot(FigureBarPlot *figB, int figsize[2],\
                int padX[2], int padY[2], int margin[2], char wAxes);


/* --------------------------------------------------------------------------- */
void Free_figBardata(FigureBarPlot *figB);

/* --------------------------------------------------------------------------- */
void PlotBarplot(FigureBarPlot *figB, BarData *bardata, char wlabels);


/* --------------------------------------------------------------------------- */
int *Transform_data_to_plot_bar(FigureBarPlot *figB, size_t nb_ctg, \
                                    const int pts[]);
/* --------------------------------------------------------------------------- */
void Add_Barplot_to_fig(FigureBarPlot *figB, BarData *bardata);

/* --------------------------------------------------------------------------- */
void Make_xticks_barplot(FigureBarPlot *figB, float angle_labels);

/* --------------------------------------------------------------------------- */
void Make_legend_barplot(FigureBarPlot *figB,\
                    int decal_X, int decal_Y, int ecart);


/* --------------------------------------------------------------------------- */
void Init_bardata(BarData *bardata, int nb_ctg, char *labels_ctg[nb_ctg],\
                    int nb_tot, int nb_in_ctg[nb_ctg], \
                        int colors[nb_ctg][3], char* label);



/* --------------------------------------------------------------------------- */
// Définition des fonctions
/* --------------------------------------------------------------------------- */


/* --------------------------------------------------------------------------- */
void Free_figBardata(FigureBarPlot *figB)
{
    free(figB->bardata);
}
/* --------------------------------------------------------------------------- */
void PlotBarplot(FigureBarPlot *figB, BarData *bardata, char wlabels)
{
    int *y_bars = \
        Transform_data_to_plot_bar(figB, bardata->nb_ctg, bardata->nb_in_ctg);   

    // largeur zone de dessin
    int w_dessin = (figB->fig->img->sx-1) - figB->fig->orig[0] - figB->fig->margin[0] - figB->fig->padX[1];

    // intervalle en px entre 2 centres de barplot selon X
    int itv_posX = w_dessin/(figB->nb_bardata+1); 
    
    // position des ticks/centre des barplot selon X
    int posX_center = figB->fig->orig[0] + bardata->idx * itv_posX;

    // Plot les rectangles
    int y1_rect = 0;
    int y2_rect = figB->fig->orig[1];

    for (int ctg = 0; ctg < bardata->nb_ctg; ctg++)
    {
        y1_rect = figB->fig->orig[1] + y_bars[ctg];

        // Dessins rect 
        if (y1_rect != y2_rect) {
            // Ajout des labels
            if (wlabels == 'y') {
                int posX_label = posX_center + itv_posX/3 - 6;
                int posY_label = y2_rect - (y2_rect-y1_rect)/2 \
                                + figB->fig->fonts[label_f].size / 2;

                char nb_in_ctg[figB->max_Y]; // pas de surprises
                sprintf(nb_in_ctg, "%d", bardata->nb_in_ctg[ctg]);

                gdImageStringFT(figB->fig->img, NULL,\
                            GetCouleur(figB->fig->img,\
                            bardata->colors[ctg]),\
                            figB->fig->fonts[title_f].path,\
                            figB->fig->fonts[label_f].size,\
                            0.,\
                            posX_label,\
                            posY_label,\
                            nb_in_ctg);  
            }

            // Plot rect
            gdImageFilledRectangle(figB->fig->img, \
                posX_center - itv_posX/4, y1_rect,\
                posX_center + itv_posX/4, y2_rect,\
                GetCouleur(figB->fig->img, bardata->colors[ctg]));
        }

        // Update y2=y1
        y2_rect = y1_rect;

    }
    free(y_bars);
}
/* --------------------------------------------------------------------------- */
int *Transform_data_to_plot_bar(FigureBarPlot *figB, size_t nb_ctg, \
                                    const int pts[])
{
    const int h_dessin = figB->fig->orig[1] - figB->fig->padY[0] - figB->fig->margin[1];   
    
    // MALLOC
    int *y_dessin_ctg = malloc(nb_ctg * sizeof(int));

    for (int i = 0; i < nb_ctg; i++) {
        if (i == 0)
            y_dessin_ctg[i] = -(pts[i] * h_dessin) / figB->max_Y;
        else
            y_dessin_ctg[i] = y_dessin_ctg[i-1] - (pts[i] * h_dessin) / figB->max_Y;
    }

    return y_dessin_ctg;
}      

/* --------------------------------------------------------------------------- */
void Init_fig_BarPlot(FigureBarPlot *figB, int figsize[2],\
                int padX[2], int padY[2], int margin[2], char wAxes)
{
    Init_figure(figB->fig, figsize, padX, padY, margin, wAxes);

    figB->nb_bardata = 0;
    figB->bardata = NULL;
    figB->max_X= 0;
    figB->max_Y = 0;
}

/* --------------------------------------------------------------------------- */
void Add_Barplot_to_fig(FigureBarPlot *figB, BarData *bardata)
{
    figB->nb_bardata++;
    figB->bardata = (BarData **)realloc(figB->bardata, figB->nb_bardata*sizeof(BarData));

    if (figB->bardata== NULL) {
        printf("Erreur : Pas assez de memoire.\n");
        free(figB->bardata);
        exit(EXIT_FAILURE);
    }

    // Index = nb line - 1 : on remplit les linedata de la figure
    figB->bardata[figB->nb_bardata-1] = bardata;

    // Print_debug_ld(fig->linedata[fig->nb_linedata]);

    figB->max_X = 0;
    figB->max_Y = Max_int(figB->max_Y, bardata->nb_tot);

    bardata->idx = figB->nb_bardata; // On update l'index du bardata, pour posX
}


/* --------------------------------------------------------------------------- */
void Make_legend_barplot(FigureBarPlot *figB,\
                    int decal_X, int decal_Y, int ecart)
{
    // Position des 4 legendes possibles
    int w_dessin = (figB->fig->img->sx-1) - figB->fig->orig[0] - figB->fig->margin[0] - figB->fig->padX[1];
    int pos_midX = figB->fig->orig[0] + w_dessin/2 + 5;
    
    int pos_X[4] = {\
                figB->fig->orig[0] + 5 + decal_X, figB->fig->orig[0] + 5 + decal_X,\
                pos_midX + decal_X, pos_midX + decal_X};

    int ecart_base = 20;
    int ecart_Y = ecart_base + ecart;

    int pos_Y[4] = {  (figB->fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    2*(figB->fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                      (figB->fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    2*(figB->fig->fonts[leg_f].size + ecart_Y) - decal_Y};

    LineStyle linestyle1;
    Init_linestyle(&linestyle1, ' ', color_lines[0], 0, 'o', 4);

    int h_rect = 15;
    int l_rect = 30;
    int posX_label = 0;
    int posY_label = 0;

    for (int i = 0; i < figB->bardata[0]->nb_ctg ; i++) {
        // Plot petits rectangles legendes
        gdImageFilledRectangle(figB->fig->img,\
                    pos_X[i], pos_Y[i]-h_rect,\
                    pos_X[i]+l_rect, pos_Y[i],\
                    GetCouleur(figB->fig->img, figB->bardata[0]->colors[i]));

        // Labels
        posX_label = pos_X[i]+l_rect+10;
        posY_label = pos_Y[i] ;
        gdImageStringFT(figB->fig->img, NULL,\
                            GetCouleur(figB->fig->img,\
                            figB->fig->fonts[leg_f].color),
                            figB->fig->fonts[leg_f].path,\
                            figB->fig->fonts[leg_f].size,\
                            0, posX_label, posY_label,\
                            figB->bardata[0]->ctg_names[i]);  
        
    }                 

}


/* --------------------------------------------------------------------------- */
void Make_xticks_barplot(FigureBarPlot *figB, float angle_labels)
{
    // largeur zone de dessin
    int w_dessin = (figB->fig->img->sx-1) - figB->fig->orig[0] - figB->fig->margin[0] - figB->fig->padX[1];

    // intervalle en px entre 2 centres de barplot selon X
    int itv_posX = w_dessin/(figB->nb_bardata+1); 
    
    // position des ticks/centre des barplot selon X
    int posX_center = 0;

    // position des labels
    int posX_label = 0;
    int posY_label = 0;

    // Recuperation de la longueur du label en px
    int brect[8] = {0};
    int len_label_px = 0;

    // Style tick
    int w_tick = 2;
    // int w_linegrid = 0.5;
    int long_tick = 9;
   
    LineStyle style_tick;
    Init_linestyle(&style_tick, '-', figB->fig->color_axes, w_tick, 'o', 8);

    for (int bp = 0; bp < figB->nb_bardata ; bp ++)
    {
        // pos centre de la barre
        posX_center = figB->fig->orig[0] + figB->bardata[bp]->idx * itv_posX;

        // plot tick
        ImageLineEpaisseur(figB->fig->img,\
                        posX_center,\
                        figB->fig->orig[1]+2,\
                        posX_center,\
                        figB->fig->orig[1] + long_tick +2,\
                        &style_tick);
        

        // tick label
        // Fake call : recuperation du brect sur le label horizontal
        // On ecrit en 0,0
        gdImageStringFT(figB->fig->img, brect,\
                            gdTransparent,\
                            figB->fig->fonts[ticklabel_f].path,\
                            figB->fig->fonts[ticklabel_f].size, 0.,\
                            0, 0,\
                            figB->bardata[bp]->label);   

        // Longueur de l'adresse en px                                
        len_label_px = brect[2] - brect[0];

        posX_label = (int)(len_label_px * cos(Deg2rad(angle_labels)));
        posY_label = (int)(len_label_px * sin(Deg2rad(angle_labels)));

        gdImageStringFT(figB->fig->img, NULL,\
                            GetCouleur(figB->fig->img,\
                                    color_lines[bp]),\
                            figB->fig->fonts[ticklabel_f].path,\
                            figB->fig->fonts[ticklabel_f].size,\
                            Deg2rad(angle_labels),\
                            posX_center - posX_label,\
                            figB->fig->orig[1] + (long_tick*3) + posY_label,\
                            figB->bardata[bp]->label);

    }

    //Avoid memory leaks
    gdFontCacheShutdown();

}

/* --------------------------------------------------------------------------- */
void Init_bardata(BarData *bardata, int nb_ctg, char *labels_ctg[nb_ctg],\
                    int nb_tot, int nb_in_ctg[nb_ctg], \
                        int colors[nb_ctg][3], char* label)
{
    bardata->nb_ctg = nb_ctg;
    bardata->ctg_names = labels_ctg;
    bardata->nb_tot = nb_tot;
    bardata->nb_in_ctg = nb_in_ctg;
    bardata->colors = colors;
    bardata->label = label;
    bardata->idx = 0;
}

/* --------------------------------------------------------------------------- */
void Print_debug_bd(BarData *bardata, char w_cat)
{
    printf("*** Debug bardata --------------------------\n");
    printf("* Label    = %s \n",  bardata->label);
    printf("* Len data = %ld \n", bardata->nb_tot);
    printf("* Nb cat   = %ld \n", bardata->nb_ctg);
    if (w_cat == 'y') {
        printf("* Vecteur nb_in_ctg : \n");
        for (int i = 0; i < bardata->nb_ctg; i++){
            printf("  * Cat %d : %d ; Couleur : %d, %d, %d\n",\
                    i, bardata->nb_in_ctg[i],
                    bardata->colors[i][0],\
                    bardata->colors[i][1],\
                    bardata->colors[i][2]);
        }
    }    
    printf("*** End bardata ----------------------------\n\n");
}



#endif /* BPLOT_H */