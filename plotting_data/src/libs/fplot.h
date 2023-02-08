/* ----------------------------------------------------------------------------
*  Bibliotheque de plot pour donnees float.
*  
*  Author : Juba Hamma. 2023.
* ---------------------------------------------------------------------------- 
*/

#ifndef FPLOT_H
#define FPLOT_H

#include "plotter.h"

/* --------------------------------------------------------------------------- */
/**
 * @brief Structure associant un vecteur de donnees (float) labelisé à un LineStyle
 * 
 */
typedef struct fLineData_s {
    size_t len_data;      /**< Taille du vecteur de data */
    float *x;               /**< Vecteur de data X */
    float *y;               /**< Vecteur de data Y */
    float fmax_X;            /**< Maximum des valeurs X */
    float fmax_Y;            /**< Maximum des valeurs Y */
    char* label;
    LineStyle *linestyle;  /**< LineStyle */
} fLineData; 

/* --------------------------------------------------------------------------- */
/**
 * @brief Structure stockant un ensemble de LineData et de données relatives à la
 * création d'une figure
 * 
 */
typedef struct FigureFLinePlot_s {
    Figure *fig;        /**< Pointeur sur objet gdImage */
    size_t nb_flinedata;  /**< Nombre de linedata dans la figure */
    fLineData **flinedata;  /**< Vecteur de LineData */
    float fmax_Y;        /**< max de l'ensemble des max_Y (float) de linedata[] */
    float fmax_X;        /**< max de l'ensemble des max_Y (float) de linedata[] */
} FigureFLinePlot;


/* --------------------------------------------------------------------------- */
void Init_fig_FLinePlot(FigureFLinePlot *figFL, int figsize[2],\
                int padX[2], int padY[2], int margin[2], char wAxes);

/* --------------------------------------------------------------------------- */
void Free_figFLdata(FigureFLinePlot *figFL);
/* --------------------------------------------------------------------------- */
void PlotFLine(FigureFLinePlot *figFL, fLineData *flinedata);
/* --------------------------------------------------------------------------- */
void Transform_fdataX_to_plot(FigureFLinePlot *figFL, size_t len_pts, const float pts[], \
             int* pts_dessin);

/* --------------------------------------------------------------------------- */
void Transform_fdataY_to_plot(FigureFLinePlot *figFL, size_t len_pts, const float pts[], \
            int* pts_dessin);

/* --------------------------------------------------------------------------- */
int *Transform_fdata_to_plot(FigureFLinePlot *figFL, size_t len_pts, const float pts[], \
            char direction);

/* --------------------------------------------------------------------------- */
void Add_Fline_to_fig(FigureFLinePlot *figFL, fLineData *flinedata);
/* --------------------------------------------------------------------------- */
void Make_Flegend(FigureFLinePlot *figFL, int decal_X, int decal_Y, int ecart);

/* --------------------------------------------------------------------------- */
void Make_Fxticks_xgrid_time(FigureFLinePlot *figFL, Date date_init);

/* --------------------------------------------------------------------------- */
void Make_Fyticks_ygrid(FigureFLinePlot *figFL, char wTicks);

/* --------------------------------------------------------------------------- */
void Init_flinedata(fLineData *flinedata, int len_data, float ptx[], float pty[], \
            char* label, LineStyle *linestyle);


/* --------------------------------------------------------------------------- */
void Print_debug_fld(fLineData *flinedata, char w_xy);





/* --------------------------------------------------------------------------- */
// Définition des fonctions
/* --------------------------------------------------------------------------- */


/* --------------------------------------------------------------------------- */
void Free_figFLdata(FigureFLinePlot *figFL)
{
    free(figFL->flinedata);
}
/* --------------------------------------------------------------------------- */
void PlotFLine(FigureFLinePlot *figFL, fLineData *flinedata)
{
    int *x_plot = Transform_fdata_to_plot(figFL, flinedata->len_data, flinedata->x, 'x');
    int *y_plot = Transform_fdata_to_plot(figFL, flinedata->len_data, flinedata->y, 'y');   

    for (int i=0; i < flinedata->len_data-1; i++) 
    {
        ImageLineEpaisseur(figFL->fig->img,\
                x_plot[i]   + figFL->fig->orig[0],   y_plot[i] + figFL->fig->orig[1],\
                x_plot[i+1] + figFL->fig->orig[0], y_plot[i+1] + figFL->fig->orig[1],\
                flinedata->linestyle);

        
        /* PlotPoint(figFL->fig,\
            x_plot[i] + figFL->fig->orig[0], y_plot[i] + figFL->fig->orig[1], flinedata->linestyle);

        if (i == flinedata->len_data-2) {
            PlotPoint(figFL->fig,\
                            x_plot[i+1] + figFL->fig->orig[0],\
                            y_plot[i+1] + figFL->fig->orig[1],\
                            flinedata->linestyle);
        } */

    }

    free(x_plot);
    free(y_plot);
}
/* --------------------------------------------------------------------------- */
void Transform_fdataX_to_plot(FigureFLinePlot *figFL, size_t len_pts,\
                                    const float pts[], int* pts_dessin)
{
    // Taile de la zone de dessin
    // (Nx-1) - orig X (0) - margin X (0) - padX droite (1)
    const int w_dessin = (figFL->fig->img->sx-1) - figFL->fig->orig[0] - figFL->fig->margin[0] - figFL->fig->padX[1];

    for (int i = 0; i < len_pts; i++)
    {
        pts_dessin[i] = (pts[i] * w_dessin) / figFL->fmax_X;
        // printf("Point : %d, %d \n", i, pts_dessin[i]);
    }

}
                           

/* --------------------------------------------------------------------------- */
void Transform_fdataY_to_plot(FigureFLinePlot *figFL, size_t len_pts, \
                                    const float pts[], int* pts_dessin)
{
    // Taile de la zone de dessin
    // Orig - padding haut (0) - margin Y (1)
    const int h_dessin = figFL->fig->orig[1] - figFL->fig->padY[0] - figFL->fig->margin[1];   
    for (int i = 0; i < len_pts; i++){
        pts_dessin[i] = -(pts[i] * h_dessin) / figFL->fmax_Y;
        // printf("Point : %d, %d \n", i, pts_dessin[i]);
    }

}

/* --------------------------------------------------------------------------- */
int *Transform_fdata_to_plot(FigureFLinePlot *figFL, size_t len_pts,\
                                  const float pts[], char direction)
{
    int *pts_dessin = malloc(len_pts * sizeof(int));

    if (pts_dessin == NULL) {
        printf("Erreur : Pas assez de memoire.\n");
        free(pts_dessin);
        exit(EXIT_FAILURE);
    }    

    // Taile de la zone de dessin
    if (direction == 'x') {
        Transform_fdataX_to_plot(figFL, len_pts, pts, pts_dessin);
    } else if (direction == 'y')
    {
        Transform_fdataY_to_plot(figFL, len_pts, pts, pts_dessin);
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
void Init_fig_FLinePlot(FigureFLinePlot *figFL, int figsize[2],\
                int padX[2], int padY[2], int margin[2], char wAxes)
{
    Init_figure(figFL->fig, figsize, padX, padY, margin, wAxes);

    figFL->nb_flinedata = 0;
    figFL->flinedata = NULL;
    figFL->fmax_X = 0;
    figFL->fmax_Y = 0;
}
/* --------------------------------------------------------------------------- */
void Add_Fline_to_fig(FigureFLinePlot *figFL, fLineData *flinedata)
{
    figFL->nb_flinedata++;
    figFL->flinedata = (fLineData **)realloc(figFL->flinedata, figFL->nb_flinedata*sizeof(fLineData));

    if (figFL->flinedata == NULL) {
        printf("Erreur : Pas assez de memoire.\n");
        free(figFL->flinedata);
        exit(EXIT_FAILURE);
    }

    // Index = nb line - 1 : on remplit les linedata de la figure
    figFL->flinedata[figFL->nb_flinedata-1] = flinedata;

    // Print_debug_ld(fig->ilinedata[fig->nb_ilinedata]);

    figFL->fmax_X = Max_float(figFL->fmax_X, flinedata->fmax_X);
    figFL->fmax_Y = Max_float(figFL->fmax_Y, flinedata->fmax_Y);
}

/* --------------------------------------------------------------------------- */
void Make_Fyticks_ygrid(FigureFLinePlot *figFL, char wTicks)
{
    if (figFL->fmax_Y == 0)
    {
        printf("Erreur : pas de ticks pour un max a 0.");
        exit(EXIT_FAILURE);
    }

    // int nb_ticks = Min_int(figFL->max_Y, 10);
    // printf("Nb ticks  = %d \n", nb_ticks);

    // int itv = figFL->max_Y / nb_ticks;
    // printf("Itv       = %d \n", itv);

    int itv=0, itv_minor=0;
    if (figFL->fmax_Y <= 10) {
        itv = 1;
        itv_minor = 0;
    } else if (figFL->fmax_Y <= 20)
    {
        itv = 2;
        itv_minor = 1;
    } else if (figFL->fmax_Y <= 50)
    {
        itv = 5;
        itv_minor = 1;
    } else if (figFL->fmax_Y <= 100)
    {
        itv = 10;
        itv_minor = 2;
    }
    
    // printf("Itv       = %d \n", itv);
    // printf("Itv minor = %d \n", itv_minor);
    int nb_ticks = figFL->fmax_Y / itv;

    int nb_ticks_min = 0;    
    if (itv_minor != 0) {
        nb_ticks_min= (figFL->fmax_Y / itv_minor) / nb_ticks;
        // printf("Nb ticks min = %d \n", nb_ticks_min);
    }    
    // printf("Nb ticks  = %d \n", nb_ticks);

    int h_canvas = figFL->fig->img->sy - figFL->fig->padY[0] - figFL->fig->padY[1];
    // On pense a enlever la marge Y pour calculer l'ecart entre tick maj
    // Pos de maxY repere figure = h_canvas - figFL->fig->margin[1]
    int h_max = h_canvas - figFL->fig->margin[1];
    // printf("h max     = %d \n", h_max);

    // int ecart_major = ( h_canvas - figFL->fig->margin[1] ) / (nb_interv)  ; 

    int itv_pixels = (itv*h_max) / figFL->fmax_Y;
    // printf("Itv px     = %d \n", itv_pixels);

    // Style tick
    int w_tick = 2;
    int w_linegrid = 0.5;
    LineStyle style_tick;
    Init_linestyle(&style_tick, '-', figFL->fig->color_axes, w_tick, ' ', ' ');
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
        ImageLineEpaisseur(figFL->fig->img,\
                        figFL->fig->orig[0],\
                        figFL->fig->orig[1] - i*(itv_pixels+0.5),\
                        (figFL->fig->img->sx-1) - figFL->fig->padX[1],\
                        figFL->fig->orig[1] - i*(itv_pixels+0.5),\
                        &style_linegrid);        

        if (wTicks == 'y') {
            long_tick = 9;
            long_tick_min = 5;
            // tick : facteur 0.5 ajouté pour gerer les nombres pairs/impais de px
            ImageLineEpaisseur(figFL->fig->img,\
                            figFL->fig->orig[0]-long_tick-2,\
                            figFL->fig->orig[1] - i*(itv_pixels+0.5),\
                            figFL->fig->orig[0]-2,\
                            figFL->fig->orig[1] - i*(itv_pixels+0.5),\
                            &style_tick);

            if (itv_minor != 0) {

                for (int j = 1; j < nb_ticks_min; j++)
                {
                    int itv_ytickmin = figFL->fig->orig[1] - (i-1)*itv_pixels - j*(itv_pixels/(nb_ticks_min));
                    
                    ImageLineEpaisseur(figFL->fig->img,\
                                    figFL->fig->orig[0]-long_tick_min-2,\
                                        itv_ytickmin,\
                                    figFL->fig->orig[0]-2,\
                                        itv_ytickmin,\
                                    &style_tick);            
                }
            }
        }

        int posX_ticklab = figFL->fig->orig[0]-(long_tick+2) \
                            - strlen(tickVal) * figFL->fig->fonts[ticklabel_f].size;
        int posY_ticklab = figFL->fig->orig[1] - i*itv_pixels \
                            + figFL->fig->fonts[ticklabel_f].size / 2;

        gdImageStringFT(figFL->fig->img, NULL,\
                            GetCouleur(figFL->fig->img,\
                                    style_tick.color),\
                            figFL->fig->fonts[ticklabel_f].path,\
                            figFL->fig->fonts[ticklabel_f].size,\
                            0., posX_ticklab, posY_ticklab, tickVal);

    }

    //Avoid memory leaks
    gdFontCacheShutdown();
}
/* --------------------------------------------------------------------------- */
void Make_Fxticks_xgrid_time(FigureFLinePlot *figFL, Date date_init)
{
    if (figFL->fmax_X == 0)
    {
        printf("Erreur : pas de ticks pour un max a 0.");
        exit(EXIT_FAILURE);
    }
    
    int nb_ticks = 7;
    int nb_itv = nb_ticks-1;

    // Intervalles entre 2 ticks en sec
    int itv_sec = figFL->fmax_X / nb_itv;

    // Largeur canvas
    int l_canvas = (figFL->fig->img->sx-1) - figFL->fig->orig[0] - figFL->fig->padX[1];
    // On pense a enlever la marge Y pour calculer l'ecart entre tick maj
    int l_max = l_canvas - figFL->fig->margin[0];

    // Intervalle entre 2 ticks en pix
    int itv_pixels = (itv_sec*l_max) / figFL->fmax_X;
    // printf("Itv X en px = %d \n", itv_pixels);

    // Style tick
    int w_tick = 2;
    int w_linegrid = 0.5;
    int long_tick = 9;

    LineStyle style_tick;
    Init_linestyle(&style_tick, '-', figFL->fig->color_axes, w_tick, ' ', ' ');

    LineStyle style_linegrid;
    Init_linestyle(&style_linegrid, '-', gris_grid, w_linegrid, ' ', ' ');

    // Plot ticks
    // printf("Nb ticks : %d \n", nb_ticks);
    for (int i = 0; i < nb_ticks; i++)
    {
        // printf("Tick : %d \n", i);

        // tick
        ImageLineEpaisseur(figFL->fig->img,\
                        figFL->fig->orig[0] + i*itv_pixels,\
                        figFL->fig->orig[1]+2,\
                        figFL->fig->orig[0] + i*itv_pixels,\
                        figFL->fig->orig[1] + long_tick +2,\
                        &style_tick);

        //gridline
        ImageLineEpaisseur(figFL->fig->img,\
                        figFL->fig->orig[0] + i*itv_pixels,\
                        figFL->fig->orig[1],\
                        figFL->fig->orig[0] + i*itv_pixels,\
                        figFL->fig->padY[0],\
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


        int xlabel_date = figFL->fig->padX[0]/2 -10 + i*itv_pixels;
        int ylabel_date = figFL->fig->orig[1] + (long_tick+2) + figFL->fig->fonts[ticklabel_f].size;
        // tick label date
        gdImageStringFT(figFL->fig->img, NULL,\
                            GetCouleur(figFL->fig->img,\
                                figFL->fig->fonts[ticklabel_f].color),\
                            figFL->fig->fonts[ticklabel_f].path,\
                            figFL->fig->fonts[ticklabel_f].size,\
                            0.,\
                            xlabel_date, ylabel_date, tickdate);

        int xlabel_hour = figFL->fig->padX[0]/2 -10 + i*itv_pixels + figFL->fig->fonts[ticklabel_f].size/2;
        int ylabel_hour = figFL->fig->orig[1] + 2*((long_tick) + figFL->fig->fonts[ticklabel_f].size);
        // tick label heure
        gdImageStringFT(figFL->fig->img, NULL,\
                            GetCouleur(figFL->fig->img,\
                                figFL->fig->fonts[ticklabel_f].color),\
                            figFL->fig->fonts[ticklabel_f].path,\
                            figFL->fig->fonts[ticklabel_f].size,\
                            0.,xlabel_hour, ylabel_hour, tickhour);
                                
        // printf("%d\n", i);
    }

    //Avoid memory leaks
    gdFontCacheShutdown();

}
/* --------------------------------------------------------------------------- */
void Make_Flegend(FigureFLinePlot *figFL,\
                    int decal_X, int decal_Y, int ecart)
{
    // Position des 8 legendes possibles
    int w_img = figFL->fig->img->sx;
    int pos_midX = w_img/2 + 5;

    int pos_X[8] = {figFL->fig->orig[0] + 5 + decal_X, figFL->fig->orig[0] + 5  + decal_X,\
                    figFL->fig->orig[0] + 5  + decal_X, figFL->fig->orig[0] + 5  + decal_X,\
                    pos_midX + decal_X, pos_midX + decal_X,\
                    pos_midX + decal_X, pos_midX + decal_X};

    int ecart_base = 8;
    int ecart_Y = ecart_base + ecart;
    int pos_Y[8] = {  (figFL->fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    2*(figFL->fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    3*(figFL->fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    4*(figFL->fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                      (figFL->fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    2*(figFL->fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    3*(figFL->fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    4*(figFL->fig->fonts[leg_f].size + ecart_Y) - decal_Y};

    // Longueur petit trait
    int long_trait_leg = 30;
    // Decalage apres trait
    int decalage_trait_leg = long_trait_leg + 10;

    // Print des petits traits de legende pour chaque plot
    for (int i = 0; i < figFL->nb_flinedata; i++) { 
        ImageLineEpaisseur(figFL->fig->img,\
                    pos_X[i],\
                    pos_Y[i] - figFL->fig->fonts[leg_f].size/2,\
                    pos_X[i] + long_trait_leg,\
                    pos_Y[i] - figFL->fig->fonts[leg_f].size/2,\
                    figFL->flinedata[i]->linestyle);
    }

    char *errStringFT;
    int brect[8] = {0};

    for (int i = 0; i < figFL->nb_flinedata; i++)
    {
        errStringFT= gdImageStringFT(figFL->fig->img, brect,\
                            GetCouleur(figFL->fig->img,\
                                figFL->fig->fonts[leg_f].color),\
                            figFL->fig->fonts[leg_f].path,\
                            figFL->fig->fonts[leg_f].size,\
                            0.,\
                            decalage_trait_leg + pos_X[i],\
                            pos_Y[i],\
                            figFL->flinedata[i]->label);        
    }

    if (errStringFT != NULL) {
        printf("%s\n", errStringFT);
    }

    //Avoid memory leaks
    gdFontCacheShutdown();
}


/* --------------------------------------------------------------------------- */
void Init_flinedata(fLineData *flinedata, int len_data, float ptx[], float pty[],
                    char* label, LineStyle *linestyle)
{
    // int *ptx_plot = Transform_data_to_plot(fig, len_data, ptx, 'x');
    // int *pty_plot = Transform_data_to_plot(fig, len_data, pty, 'y');
    
    flinedata->len_data = len_data;
    flinedata->x = ptx; 
    flinedata->y = pty;
    flinedata->fmax_X = fMaxval_array(flinedata->x, flinedata->len_data);
    flinedata->fmax_Y = fMaxval_array(flinedata->y, flinedata->len_data);
    flinedata->label = label;
    flinedata->linestyle = linestyle;
    // print_arr1D(len_data, linedata->y, 'n');
}


/* --------------------------------------------------------------------------- */
void Print_debug_fld(fLineData *flinedata, char w_xy)
{
    printf("*** Debug flinedata --------------------------\n");
    printf("* Label    = %s \n", flinedata->label);
    printf("* Len data = %ld \n", flinedata->len_data);
    printf("* Max X    = %f \n", flinedata->fmax_X);
    printf("* Max Y    = %f \n", flinedata->fmax_Y);
    if (w_xy == 'y') {
        printf("* Vecteur X : \n");
        print_Farr1D(flinedata->len_data, flinedata->x, 'n');
        printf("* Vecteur Y : \n");
        print_Farr1D(flinedata->len_data, flinedata->y, 'n');
    }    
    Print_debug_ls(flinedata->linestyle);
    printf("*** End linedata ----------------------------\n\n");
}



#endif /* FPLOT_H */