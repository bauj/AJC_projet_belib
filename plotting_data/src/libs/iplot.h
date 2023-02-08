/* ----------------------------------------------------------------------------
*  Bibliotheque de plot pour donnees entieres.
*  
*  Author : Juba Hamma. 2023.
* ---------------------------------------------------------------------------- 
*/

#ifndef IPLOT_H
#define IPLOT_H

#include "plotter.h"

/* --------------------------------------------------------------------------- */
/**
 * @brief Structure associant un vecteur de donnees (int) labelisé à un LineStyle
 * 
 */
typedef struct iLineData_s {
    size_t len_data;      /**< Taille du vecteur de data */
    int *x;               /**< Vecteur de data X */
    int *y;               /**< Vecteur de data Y */
    int max_X;            /**< Maximum des valeurs X */
    int max_Y;            /**< Maximum des valeurs Y */
    char* label;
    LineStyle *linestyle;  /**< LineStyle */
} iLineData; 


/* --------------------------------------------------------------------------- */
/**
 * @brief Structure stockant un ensemble de LineData et de données relatives à la
 * création d'une figure
 * 
 */
typedef struct FigureILinePlot_s {
    Figure *fig;        /**< Pointeur sur objet gdImage */
    size_t nb_ilinedata;  /**< Nombre de linedata dans la figure */
    iLineData **ilinedata;  /**< Vecteur de LineData */
    int max_Y;        /**< max de l'ensemble des max_Y (float) de linedata[] */
    int max_X;        /**< max de l'ensemble des max_Y (float) de linedata[] */
} FigureILinePlot;

/* --------------------------------------------------------------------------- */
void Print_debug_ild(iLineData *ilinedata, char w_xy);

/* --------------------------------------------------------------------------- */
void Init_fig_ILinePlot(FigureILinePlot *figIL, int figsize[2],\
                int padX[2], int padY[2], int margin[2], char wAxes);


/* --------------------------------------------------------------------------- */
void Free_figILdata(FigureILinePlot *figIL);

/* --------------------------------------------------------------------------- */
void PlotILine(FigureILinePlot *figIL, iLineData *ilinedata);

/* --------------------------------------------------------------------------- */
void Transform_idataX_to_plot(FigureILinePlot *figIL, size_t len_pts, const int pts[], \
            int* pts_dessin);

/* --------------------------------------------------------------------------- */
void Transform_idataY_to_plot(FigureILinePlot *figIL, size_t len_pts, const int pts[], \
            int* pts_dessin);

/* --------------------------------------------------------------------------- */
int *Transform_idata_to_plot(FigureILinePlot *figIL, size_t len_pts, const int pts[], \
            char direction);

/* --------------------------------------------------------------------------- */
void Add_Iline_to_fig(FigureILinePlot *figIL, iLineData *ilinedata);
/* --------------------------------------------------------------------------- */
void Make_Ilegend(FigureILinePlot *figIL, int decal_X, int decal_Y, int ecart);

/* --------------------------------------------------------------------------- */
void Make_Ixticks_xgrid_time(FigureILinePlot *figIL, Date date_init);

/* --------------------------------------------------------------------------- */
void Make_Iyticks_ygrid(FigureILinePlot *figIL, char wTicks);

/* --------------------------------------------------------------------------- */
void Init_ilinedata(iLineData *ilinedata, int len_data, int ptx[], int pty[], \
            char* label, LineStyle *linestyle);

/* --------------------------------------------------------------------------- */
// Définition des fonctions
/* --------------------------------------------------------------------------- */



/* --------------------------------------------------------------------------- */
void Free_figILdata(FigureILinePlot *figIL)
{

    /* Destroy the image in memory. */
    gdImageDestroy(figIL->fig->img);

    // Destroy fig
    free(figIL->fig);

    if (figIL->nb_ilinedata > 0) {
        // for (int i = 0; i < figIL->nb_ilinedata; i++) {
        //     free(figIL->ilinedata[i]);
        // }
        free(figIL->ilinedata);
    }

}
/* --------------------------------------------------------------------------- */
void PlotILine(FigureILinePlot *figIL, iLineData *ilinedata)
{
    int *x_plot = Transform_idata_to_plot(figIL, ilinedata->len_data, ilinedata->x, 'x');
    int *y_plot = Transform_idata_to_plot(figIL, ilinedata->len_data, ilinedata->y, 'y');   

    for (int i=0; i < ilinedata->len_data-1; i++) 
    {
        ImageLineEpaisseur(figIL->fig->img,\
                x_plot[i]   + figIL->fig->orig[0],   y_plot[i] + figIL->fig->orig[1],\
                x_plot[i+1] + figIL->fig->orig[0], y_plot[i+1] + figIL->fig->orig[1],\
                ilinedata->linestyle);

        
        /* PlotPoint(figIL->fig,\
            x_plot[i] + figIL->fig->orig[0], y_plot[i] + figIL->fig->orig[1], ilinedata->linestyle);

        if (i == ilinedata->len_data-2) {
            PlotPoint(figIL->fig,\
                            x_plot[i+1] + figIL->fig->orig[0],\
                            y_plot[i+1] + figIL->fig->orig[1],\
                            ilinedata->linestyle);
        } */

    }

    free(x_plot);
    free(y_plot);
}

/* --------------------------------------------------------------------------- */
void Transform_idataY_to_plot(FigureILinePlot *figIL, size_t len_pts, \
                                    const int pts[], int* pts_dessin)
{
    // Taile de la zone de dessin
    // Orig - padding haut (0) - margin Y (1)
    const int h_dessin = figIL->fig->orig[1] - figIL->fig->padY[0] - figIL->fig->margin[1];   
    for (int i = 0; i < len_pts; i++){
        pts_dessin[i] = -(pts[i] * h_dessin) / figIL->max_Y;
        // printf("Point : %d, %d \n", i, pts_dessin[i]);
    }

}

/* --------------------------------------------------------------------------- */
int *Transform_idata_to_plot(FigureILinePlot *figIL, size_t len_pts,\
                                  const int pts[], char direction)
{
    int *pts_dessin = malloc(len_pts * sizeof(int));

    if (pts_dessin == NULL) {
        printf("Erreur : Pas assez de memoire.\n");
        free(pts_dessin);
        exit(EXIT_FAILURE);
    }    

    // Taile de la zone de dessin
    if (direction == 'x') {
        Transform_idataX_to_plot(figIL, len_pts, pts, pts_dessin);
    } else if (direction == 'y')
    {
        Transform_idataY_to_plot(figIL, len_pts, pts, pts_dessin);
    }
    else 
    {
        printf("Erreur : Direction de tracé inconnue.\n");
        free(pts_dessin);
        exit(EXIT_FAILURE);
    }
    
    return pts_dessin;
}


/* --------------------------------------------------------------------------- */
void Init_fig_ILinePlot(FigureILinePlot *figIL, int figsize[2],\
                int padX[2], int padY[2], int margin[2], char wAxes)
{

    figIL->fig = (Figure *)malloc(sizeof(Figure));

    Init_figure(figIL->fig, figsize, padX, padY, margin, wAxes);

    figIL->nb_ilinedata = 0;
    figIL->ilinedata = NULL;
    figIL->max_X = 0;
    figIL->max_Y = 0;
}

/* --------------------------------------------------------------------------- */
void Transform_idataX_to_plot(FigureILinePlot *figIL, size_t len_pts,\
                                    const int pts[], int* pts_dessin)
{
    // Taile de la zone de dessin
    // (Nx-1) - orig X (0) - margin X (0) - padX droite (1)
    const int w_dessin = (figIL->fig->img->sx-1) - figIL->fig->orig[0] - figIL->fig->margin[0] - figIL->fig->padX[1];

    for (int i = 0; i < len_pts; i++)
    {
        pts_dessin[i] = (pts[i] * w_dessin) / figIL->max_X;
        // printf("Point : %d, %d \n", i, pts_dessin[i]);
    }

}
                           
/* --------------------------------------------------------------------------- */
void Add_Iline_to_fig(FigureILinePlot *figIL, iLineData *ilinedata)
{
    figIL->nb_ilinedata++;
    figIL->ilinedata = (iLineData **)realloc(figIL->ilinedata, figIL->nb_ilinedata*sizeof(iLineData));

    if (figIL->ilinedata == NULL) {
        printf("Erreur : Pas assez de memoire.\n");
        free(figIL->ilinedata);
        exit(EXIT_FAILURE);
    }

    // Index = nb line - 1 : on remplit les linedata de la figure
    figIL->ilinedata[figIL->nb_ilinedata-1] = ilinedata;

    // Print_debug_ld(fig->ilinedata[fig->nb_ilinedata]);

    figIL->max_X = Max_int(figIL->max_X, ilinedata->max_X);
    figIL->max_Y = Max_int(figIL->max_Y, ilinedata->max_Y);


}
/* --------------------------------------------------------------------------- */
void Make_Ilegend(FigureILinePlot *figIL,\
                    int decal_X, int decal_Y, int ecart)
{
    // Position des 8 legendes possibles
    int w_img = figIL->fig->img->sx;
    int pos_midX = w_img/2 + 5;

    int pos_X[8] = {figIL->fig->orig[0] + 5 + decal_X, figIL->fig->orig[0] + 5  + decal_X,\
                    figIL->fig->orig[0] + 5  + decal_X, figIL->fig->orig[0] + 5  + decal_X,\
                    pos_midX + decal_X, pos_midX + decal_X,\
                    pos_midX + decal_X, pos_midX + decal_X};

    int ecart_base = 8;
    int ecart_Y = ecart_base + ecart;
    int pos_Y[8] = {  (figIL->fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    2*(figIL->fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    3*(figIL->fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    4*(figIL->fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                      (figIL->fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    2*(figIL->fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    3*(figIL->fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    4*(figIL->fig->fonts[leg_f].size + ecart_Y) - decal_Y};

    // Longueur petit trait
    int long_trait_leg = 30;
    // Decalage apres trait
    int decalage_trait_leg = long_trait_leg + 10;

    // Print des petits traits de legende pour chaque plot
    for (int i = 0; i < figIL->nb_ilinedata; i++) { 
        ImageLineEpaisseur(figIL->fig->img,\
                    pos_X[i],\
                    pos_Y[i] - figIL->fig->fonts[leg_f].size/2,\
                    pos_X[i] + long_trait_leg,\
                    pos_Y[i] - figIL->fig->fonts[leg_f].size/2,\
                    figIL->ilinedata[i]->linestyle);
    }

    char *errStringFT;
    int brect[8] = {0};

    for (int i = 0; i < figIL->nb_ilinedata; i++)
    {
        errStringFT= gdImageStringFT(figIL->fig->img, brect,\
                            GetCouleur(figIL->fig->img,\
                                figIL->fig->fonts[leg_f].color),\
                            figIL->fig->fonts[leg_f].path,\
                            figIL->fig->fonts[leg_f].size,\
                            0.,\
                            decalage_trait_leg + pos_X[i],\
                            pos_Y[i],\
                            figIL->ilinedata[i]->label);        
    }

    if (errStringFT != NULL) {
        printf("%s\n", errStringFT);
    }

    //Avoid memory leaks
    gdFontCacheShutdown();
}
/* --------------------------------------------------------------------------- */
void Make_Ixticks_xgrid_time(FigureILinePlot *figIL, Date date_init)
{
    if (figIL->max_X == 0)
    {
        printf("Erreur : pas de ticks pour un max a 0.");
        exit(EXIT_FAILURE);
    }
    
    int nb_ticks = 7;
    int nb_itv = nb_ticks-1;

    // Intervalles entre 2 ticks en sec
    int itv_sec = figIL->max_X / nb_itv;

    // Largeur canvas
    int l_canvas = (figIL->fig->img->sx-1) - figIL->fig->orig[0] - figIL->fig->padX[1];
    // On pense a enlever la marge Y pour calculer l'ecart entre tick maj
    int l_max = l_canvas - figIL->fig->margin[0];

    // Intervalle entre 2 ticks en pix
    int itv_pixels = (itv_sec*l_max) / figIL->max_X;
    // printf("Itv X en px = %d \n", itv_pixels);

    // Style tick
    int w_tick = 2;
    int w_linegrid = 0.5;
    int long_tick = 9;

    LineStyle style_tick;
    Init_linestyle(&style_tick, '-', figIL->fig->color_axes, w_tick, ' ', ' ');

    LineStyle style_linegrid;
    Init_linestyle(&style_linegrid, '-', gris_grid, w_linegrid, ' ', ' ');

    // Plot ticks
    // printf("Nb ticks : %d \n", nb_ticks);
    for (int i = 0; i < nb_ticks; i++)
    {
        // printf("Tick : %d \n", i);

        // tick
        ImageLineEpaisseur(figIL->fig->img,\
                        figIL->fig->orig[0] + i*itv_pixels,\
                        figIL->fig->orig[1]+2,\
                        figIL->fig->orig[0] + i*itv_pixels,\
                        figIL->fig->orig[1] + long_tick +2,\
                        &style_tick);

        //gridline
        ImageLineEpaisseur(figIL->fig->img,\
                        figIL->fig->orig[0] + i*itv_pixels,\
                        figIL->fig->orig[1],\
                        figIL->fig->orig[0] + i*itv_pixels,\
                        figIL->fig->padY[0],\
                        &style_linegrid);        

        time_t t_tick = date_init.ctime + i*itv_sec;
        struct tm *tm_tick;
        gmtime(&t_tick);
        tm_tick = gmtime(&t_tick);

        char tickdate[6];
        strftime(tickdate, 6, "%d/%m", tm_tick);
        // printf("%s \n", tickdate);

        char tickhour[6];
        strftime(tickhour, 6, "%H:%M", tm_tick);
        // printf("%s \n", tickhour);


        int xlabel_date = figIL->fig->padX[0]/2 -10 + i*itv_pixels;
        int ylabel_date = figIL->fig->orig[1] + (long_tick+2) + figIL->fig->fonts[ticklabel_f].size;
        // tick label date
        gdImageStringFT(figIL->fig->img, NULL,\
                            GetCouleur(figIL->fig->img,\
                                figIL->fig->fonts[ticklabel_f].color),\
                            figIL->fig->fonts[ticklabel_f].path,\
                            figIL->fig->fonts[ticklabel_f].size,\
                            0.,\
                            xlabel_date, ylabel_date, tickdate);

        int xlabel_hour = figIL->fig->padX[0]/2 -10 + i*itv_pixels + figIL->fig->fonts[ticklabel_f].size/2;
        int ylabel_hour = figIL->fig->orig[1] + 2*((long_tick) + figIL->fig->fonts[ticklabel_f].size);
        // tick label heure
        gdImageStringFT(figIL->fig->img, NULL,\
                            GetCouleur(figIL->fig->img,\
                                figIL->fig->fonts[ticklabel_f].color),\
                            figIL->fig->fonts[ticklabel_f].path,\
                            figIL->fig->fonts[ticklabel_f].size,\
                            0.,xlabel_hour, ylabel_hour, tickhour);
                                
        // printf("%d\n", i);
    }

    //Avoid memory leaks
    gdFontCacheShutdown();

}

/* --------------------------------------------------------------------------- */
void Make_Iyticks_ygrid(FigureILinePlot *figIL, char wTicks)
{
    if (figIL->max_Y == 0)
    {
        printf("Erreur : pas de ticks pour un max a 0.");
        exit(EXIT_FAILURE);
    }

    printf("MAX y : %d \n", figIL->max_Y);

    // int nb_ticks = Min_int(figIL->max_Y, 10);
    // printf("Nb ticks  = %d \n", nb_ticks);

    // int itv = figIL->max_Y / nb_ticks;
    // printf("Itv       = %d \n", itv);

    int itv=0, itv_minor=0;
    if (figIL->max_Y <= 10) {
        itv = 1;
        itv_minor = 0;
    } else if (figIL->max_Y <= 20)
    {
        itv = 2;
        itv_minor = 1;
    } else if (figIL->max_Y <= 50)
    {
        itv = 5;
        itv_minor = 1;
    } else if (figIL->max_Y <= 100)
    {
        itv = 10;
        itv_minor = 2;
    }
    
    // printf("Itv       = %d \n", itv);
    // printf("Itv minor = %d \n", itv_minor);
    int nb_ticks = figIL->max_Y / itv;

    int nb_ticks_min = 0;    
    if (itv_minor != 0) {
        nb_ticks_min= (figIL->max_Y / itv_minor) / nb_ticks;
        // printf("Nb ticks min = %d \n", nb_ticks_min);
    }    
    // printf("Nb ticks  = %d \n", nb_ticks);

    int h_canvas = figIL->fig->img->sy - figIL->fig->padY[0] - figIL->fig->padY[1];
    // On pense a enlever la marge Y pour calculer l'ecart entre tick maj
    // Pos de maxY repere figure = h_canvas - figIL->fig->margin[1]
    int h_max = h_canvas - figIL->fig->margin[1];
    // printf("h max     = %d \n", h_max);

    // int ecart_major = ( h_canvas - figIL->fig->margin[1] ) / (nb_interv)  ; 

    int itv_pixels = (itv*h_max) / figIL->max_Y;
    // printf("Itv px     = %d \n", itv_pixels);

    // Style tick
    int w_tick = 2;
    int w_linegrid = 0.5;
    LineStyle style_tick;
    Init_linestyle(&style_tick, '-', figIL->fig->color_axes, w_tick, ' ', ' ');
    int long_tick = 0;
    int long_tick_min = 0;
    LineStyle style_linegrid;
    Init_linestyle(&style_linegrid, '-', gris_grid, w_linegrid, ' ', ' ');

    // Plot ticks
    char tickVal[12];

    for (int i = 1; i <= nb_ticks; i++)
    {
        sprintf(tickVal, "%d", i*itv);


        // gridline
        ImageLineEpaisseur(figIL->fig->img,\
                        figIL->fig->orig[0],\
                        figIL->fig->orig[1] - i*(itv_pixels+0.5),\
                        (figIL->fig->img->sx-1) - figIL->fig->padX[1],\
                        figIL->fig->orig[1] - i*(itv_pixels+0.5),\
                        &style_linegrid);        

        if (wTicks == 'y') {
            long_tick = 9;
            long_tick_min = 5;
            // tick : facteur 0.5 ajouté pour gerer les nombres pairs/impais de px
            ImageLineEpaisseur(figIL->fig->img,\
                            figIL->fig->orig[0]-long_tick-2,\
                            figIL->fig->orig[1] - i*(itv_pixels+0.5),\
                            figIL->fig->orig[0]-2,\
                            figIL->fig->orig[1] - i*(itv_pixels+0.5),\
                            &style_tick);

            if (itv_minor != 0) {

                for (int j = 1; j < nb_ticks_min; j++)
                {
                    int itv_ytickmin = figIL->fig->orig[1] - (i-1)*itv_pixels - j*(itv_pixels/(nb_ticks_min));
                    
                    ImageLineEpaisseur(figIL->fig->img,\
                                    figIL->fig->orig[0]-long_tick_min-2,\
                                        itv_ytickmin,\
                                    figIL->fig->orig[0]-2,\
                                        itv_ytickmin,\
                                    &style_tick);            
                }
            }
        }

        int posX_ticklab = figIL->fig->orig[0]-(long_tick+2) \
                            - strlen(tickVal) * figIL->fig->fonts[ticklabel_f].size;
        int posY_ticklab = figIL->fig->orig[1] - i*itv_pixels \
                            + figIL->fig->fonts[ticklabel_f].size / 2;

        gdImageStringFT(figIL->fig->img, NULL,\
                            GetCouleur(figIL->fig->img,\
                                    style_tick.color),\
                            figIL->fig->fonts[ticklabel_f].path,\
                            figIL->fig->fonts[ticklabel_f].size,\
                            0., posX_ticklab, posY_ticklab, tickVal);

    }

    //Avoid memory leaks
    gdFontCacheShutdown();
}


/* --------------------------------------------------------------------------- */
void Init_ilinedata(iLineData *ilinedata, int len_data, int ptx[], int pty[],
                    char* label, LineStyle *linestyle)
{
    // int *ptx_plot = Transform_data_to_plot(fig, len_data, ptx, 'x');
    // int *pty_plot = Transform_data_to_plot(fig, len_data, pty, 'y');
    
    ilinedata->len_data = len_data;
    ilinedata->x = ptx; 
    ilinedata->y = pty;
    ilinedata->max_X = iMaxval_array(ilinedata->x, ilinedata->len_data);
    ilinedata->max_Y = iMaxval_array(ilinedata->y, ilinedata->len_data);

    ilinedata->label = label;
    ilinedata->linestyle = linestyle;
    // print_arr1D(len_data, linedata->y, 'n');
}

/* --------------------------------------------------------------------------- */
void Print_debug_ild(iLineData *ilinedata, char w_xy)
{
    printf("*** Debug ilinedata --------------------------\n");
    printf("* Label    = %s \n", ilinedata->label);
    printf("* Len data = %ld \n", ilinedata->len_data);
    printf("* Max X    = %d \n", ilinedata->max_X);
    printf("* Max Y    = %d \n", ilinedata->max_Y);
    if (w_xy == 'y') {
        printf("* Vecteur X : \n");
        print_Iarr1D(ilinedata->len_data, ilinedata->x, 'n');
        printf("* Vecteur Y : \n");
        print_Iarr1D(ilinedata->len_data, ilinedata->y, 'n');
    }    
    Print_debug_ls(ilinedata->linestyle);
    printf("*** End linedata ----------------------------\n\n");
}



#endif /* IPLOT_H */