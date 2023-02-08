/* ----------------------------------------------------------------------------
*  Bibliotheque de plot basée sur libgd developpee dans le but de visualiser
* le dataset recensant les bornes Belib sur Paris.
*  
*  Author : Juba Hamma. 2023.
* ---------------------------------------------------------------------------- 
*/

#ifndef PLOTTER_H
#define PLOTTER_H

#include "consts.h"
#include <stdlib.h>
#include "getter.h"
#include <gd.h>
#include <math.h>

#define PI 3.141592

/* --------------------------------------------------------------------------- */
typedef enum {label_f, annotation_f, title_f, ticklabel_f, subtitle_f, leg_f} fontsFig;

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
// Structures -> Check l'ordre des arguments pour la contiguite memoire
/* --------------------------------------------------------------------------- */

/**
 * @brief Structure definissant un certain style de plot
 * 
 */
typedef struct LineStyle_s {
    char style;     /**< Style de plot : '-' : line, ':' dashed,
                                          '' : point seulement */
    int  w;         /**< Epaisseur du trait*/
    char marker;    /**< Type de marker : 's' : carré, 'o' : cercle,
                                           '' : pas de marker */
    int  ms;        /**< Markersize*/
    int  color[3];  /**< Couleur rgb*/
} LineStyle;

/* --------------------------------------------------------------------------- */
/**
 * @brief Structure associant un vecteur de donnees (int) labelisé à un LineStyle
 * 
 */
typedef struct LineData_s {
    size_t len_data;      /**< Taille du vecteur de data */
    int *x;               /**< Vecteur de data X */
    int *y;               /**< Vecteur de data Y */
    int max_X;            /**< Maximum des valeurs X */
    int max_Y;            /**< Maximum des valeurs Y */
    char* label;
    LineStyle *linestyle;  /**< LineStyle */
} LineData; 

/* --------------------------------------------------------------------------- */
/**
 * @brief Structure associant un vecteur de donnees (float) labelisé à un LineStyle
 * 
 */
typedef struct fLineData_s {
    size_t len_data;      /**< Taille du vecteur de data */
    float *x;               /**< Vecteur de data X */
    float *y;               /**< Vecteur de data Y */
    float max_X;            /**< Maximum des valeurs X */
    float max_Y;            /**< Maximum des valeurs Y */
    char* label;
    LineStyle *linestyle;  /**< LineStyle */
} fLineData; 


/* --------------------------------------------------------------------------- */
/**
 * @brief Structure stockant les infos d'une police
 * 
 */
typedef struct Font_s {
    char* path;         /**< Chemin vers le fichier police*/
    int size;           /**< Taille de la police*/
    int color[3];       /**< Couleur de la police (rgb) */
} Font;


/* --------------------------------------------------------------------------- */
/**
 * @brief Structure stockant un ensemble de LineData et de données relatives à la
 * création d'une figure
 * 
 */
typedef struct Figure_s {
    gdImage *img;        /**< Pointeur sur objet gdImage */
    size_t nb_linedata;  /**< Nombre de linedata dans la figure */
    size_t nb_bardata;  /**< Nombre de linedata dans la figure */
    LineData **linedata;  /**< Vecteur de LineData */
    BarData **bardata;   /**< Vecteur de BarData */
    Font fonts[6];       /**< Fonts used in the fig*/
    int max_X;           /**< max de l'ensemble des max_X de linedata[] */
    int max_Y;           /**< max de l'ensemble des max_Y de linedata[] */
    float fmax_Y;        /**< max de l'ensemble des max_Y (float) de linedata[] */
    int padX[2];         /**< Padding gauche/droite pour definir canvas (cvs) et orig */
    int padY[2];         /**< Padding haut/bas pour definir canvas (cvs) et orig */
    int orig[2];         /**< Coordonnees de l'origine des plots */    
    int margin[2];       /**< Marge droite et haut (evite de toucher les bords)*/    
    int color_bg[3];     /**< Couleur du fond de la figure */
    int color_cvs_bg[3]; /**< Couleur du fond du canvas */
    int color_axes[3];   /**< Couleur des axes*/
} Figure;

// casser figure en figure_linedata, figure_bardata, figure_flinedata !!

/* --------------------------------------------------------------------------- */
// Declaration fonctions
/* --------------------------------------------------------------------------- */

/**
 * @brief 
 * 
 * @param bardata 
 * @param nb_ctg 
 * @param nb_tot 
 * @param nb_in_ctg 
 * @param colors 
 * @param label 
 */
void Init_bardata(BarData *bardata, int nb_ctg, char *labels_ctg[nb_ctg],\
                    int nb_tot, int nb_in_ctg[nb_ctg], \
                        int colors[nb_ctg][3], char* label);


/**
 * @brief Transforme les degrés en radians.
 * 
 * @param angle Angle en degrés
 * @return float Angle en radians
 */
float Deg2rad(float angle);

/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param x_array 
 * @param n 
 * @return int 
 */
int Maxval_array(const int x_array[], size_t n);

/* --------------------------------------------------------------------------- */
/**
 * @brief Get the Couleur object
 * 
 * @param im_fig 
 * @param couleur 
 * @return int 
 */
int GetCouleur(gdImagePtr im_fig, const int couleur[3]);

/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param fig 
 * @param dir_figures 
 * @param filename_fig 
 */
void Save_to_png(Figure *fig, const char *dir_figures, const char *filename_fig);

/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param fig 
 * @param couleur_bg 
 * @param couleur_canvas_bg 
 */
void Make_background(Figure *fig, const int couleur_bg[3],\
        const int couleur_canvas_bg[3]);

/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param fig 
 * @param couleur 
 */
void Make_support_axes(Figure *fig, const int couleur[3]);

/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param im_fig 
 * @param x1 
 * @param y1 
 * @param x2 
 * @param y2 
 * @param linestyle 
 */
void ImageLineEpaisseur(gdImagePtr im_fig, const int x1, const int y1, const int x2, \
            const int y2, LineStyle *linestyle);

/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param fig 
 * @param x1 
 * @param y1 
 * @param linestyle 
 */
void PlotPoint(Figure *fig, const int x1, const int y1,LineStyle *linestyle);

/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param fig 
 * @param linedata 
 */
void PlotLine(Figure *fig, LineData *linedata);

/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param fig 
 * @param len_pts 
 * @param pts 
 * @param pts_dessin 
 */
void Transform_dataX_to_plot(Figure *fig, size_t len_pts, const int pts[], \
            int* pts_dessin);

/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param fig 
 * @param len_pts 
 * @param pts 
 * @param pts_dessin 
 */
void Transform_dataY_to_plot(Figure *fig, size_t len_pts, const int pts[], \
            int* pts_dessin);

/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param fig 
 * @param len_pts 
 * @param pts 
 * @param direction 
 * @return int* 
 */
int *Transform_data_to_plot(Figure *fig, size_t len_pts, const int pts[], \
            char direction);

/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param linestyle 
 * @param style 
 * @param color 
 * @param width 
 * @param marker 
 * @param ms 
 */
void Init_linestyle(LineStyle *linestyle, char style,const int color[3], int width, \
            char marker, int ms);

/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param linedata 
 * @param len_data 
 * @param ptx 
 * @param pty 
 * @param label 
 * @param linestyle 
 */
void Init_linedata(LineData *linedata, int len_data, int ptx[], int pty[], \
            char* label, LineStyle *linestyle);

/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param fig 
 * @param color 
 */
void Change_fig_axes_color(Figure *fig, int color[3]);

/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param fig 
 * @param color 
 */
void Change_fig_cvs_bg(Figure *fig, int color[3]);

/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param fig 
 * @param color 
 */
void Change_fig_bg(Figure *fig, int color[3]);

/* --------------------------------------------------------------------------- */

/**
 * @brief 
 * 
 * @param fig 
 * @param figsize 
 * @param padX 
 * @param padY 
 * @param margin 
 */
void Init_figure(Figure *fig, int figsize[2],\
            int padX[2], int padY[2], int margin[2], char wAxes);

/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param linestyle 
 */
void Print_debug_ls(LineStyle *linestyle);

/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param linedata 
 * @param w_xy 
 */
void Print_debug_ld(LineData *linedata, char w_xy);

/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param x_array 
 * @param n 
 * @return int 
 */
int Maxval_array(const int x_array[], size_t n);

/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param x 
 * @param y 
 * @return int 
 */
int Max_int(int x, int y);

/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param x 
 * @param y 
 * @return int 
 */
int Min_int(int x, int y);

/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param fig 
 * @param xlabel 
 * @param font 
 * @param size 
 * @param color 
 * @param decalage_X 
 * @param decalage_Y 
 */
void Make_xlabel(Figure *fig, char* xlabel,\
             int decalage_X, int decalage_Y);

/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param fig 
 * @param ylabel 
 * @param font 
 * @param size 
 * @param color 
 * @param decalage_X 
 * @param decalage_Y 
 */
void Make_ylabel(Figure *fig, char* ylabel,\
             int decalage_X, int decalage_Y);

/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param fig 
 * @param decal_X 
 * @param decal_Y 
 * @param ecart 
 */
void Make_legend(Figure *fig, int decal_X, int decal_Y, int ecart);

/* --------------------------------------------------------------------------- */
/**
 * @brief 
 * 
 * @param fig 
 * @param wTicks 
 */
void Make_yticks_ygrid(Figure *fig, char wTicks);


/* --------------------------------------------------------------------------- */
// Définition des fonctions
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
int Maxval_array(const int x_array[], size_t n)
{
    int t, i;
    t = x_array[0];
    for(i = 1; i < n; i++)
    {
        t = (x_array[i] > t) ? x_array[i] : t;
    }
    return t;
}

/* --------------------------------------------------------------------------- */
float Deg2rad(float angle)
{
    return angle*PI/180;
}

/* --------------------------------------------------------------------------- */
void Save_to_png(Figure *fig, const char *dir_figures, const char *filename_fig)
{
    FILE *pngout_fig;

    char path_outputFig[40] = {""};
    strcat(path_outputFig, dir_figures);
    strcat(path_outputFig, filename_fig);
    pngout_fig = fopen(path_outputFig, "wb");

    /* Output the image to the disk file in PNG format. */
    gdImagePngEx(fig->img, pngout_fig,5);

    /* Close the files. */
    fclose(pngout_fig);

    // Free des tableau de data dans la figure
    free(fig->linedata);
    free(fig->bardata);

}


/* --------------------------------------------------------------------------- */
void Make_background(Figure *fig,\
                        const int color_bg[3], const int color_canvas_bg[3])
{
    /* Remplissage du fond */
    gdImageFilledRectangle(fig->img,\
                        0, 0,\
                        fig->img->sx-1, fig->img->sy-1,
                        GetCouleur(fig->img, color_bg));

    /* Remplissage du canvas */
    gdImageFilledRectangle(fig->img,\
                        fig->padX[0], fig->padY[0],\
                        (fig->img->sx-1)-fig->padX[1], (fig->img->sy-1) - fig->padY[1],\
                        GetCouleur(fig->img, color_canvas_bg));
}

/* --------------------------------------------------------------------------- */
void ImageLineEpaisseur(gdImagePtr im_fig,\
    const int x1, const int y1, const int x2, const int y2, LineStyle *linestyle)
{
                    
    gdImageSetThickness(im_fig, linestyle->w);
    gdImageSetAntiAliased(im_fig, GetCouleur(im_fig, linestyle->color));
    if (linestyle->style == '-') {
        gdImageLine(im_fig, x1, y1,   x2,   y2,\
                   gdAntiAliased);
    } else if (linestyle->style == ':') {
        gdImageDashedLine(im_fig, x1, y1,   x2,   y2,\
                   GetCouleur(im_fig, linestyle->color));
    } else {

    }
    
    // gdImageSetThickness(im_fig, 1);
}


/* --------------------------------------------------------------------------- */
void Make_support_axes(Figure *fig, const int couleur[3])
{
    int w_axes = 3;
    LineStyle linestyle_axe;
    Init_linestyle(&linestyle_axe, '-', couleur, w_axes, ' ', 0);

    // axe vertical
    ImageLineEpaisseur(fig->img,\
                            fig->orig[0]-(w_axes-1), fig->orig[1]+(w_axes-1),\
                            fig->orig[0]-(w_axes-1), fig->padY[0],\
                            &linestyle_axe); 

    // axe horizontal
    ImageLineEpaisseur(fig->img,\
                            fig->orig[0]-(w_axes-1), fig->orig[1]+(w_axes-1),\
                            (fig->img->sx-1)-fig->padX[1],  fig->orig[1]+(w_axes-1),
                            &linestyle_axe); 
}

/* --------------------------------------------------------------------------- */
void PlotPoint(Figure *fig, const int x1, const int y1,\
                        LineStyle *linestyle)
{
    if (x1 == fig->orig[0] && y1 != fig->orig[1])
    {   
        // Arc lorsque pt sur axe horizontal
        gdImageFilledArc (fig->img, x1, y1, linestyle->ms, linestyle->ms,\
                          -90, 90,\
                          GetCouleur(fig->img, linestyle->color), gdArc);
    } else if (x1 != fig->orig[0] && y1 == fig->orig[1])
    {
        // Arc lorsque pt sur axe vertical
        gdImageFilledArc (fig->img, x1, y1, linestyle->ms, linestyle->ms,\
                          -180, 0,\
                          GetCouleur(fig->img, linestyle->color), gdArc);
    } else if (x1 == fig->orig[0] && y1 == fig->orig[1])
    {
        // Arc lorsque pt sur l'origine
        gdImageFilledArc (fig->img, x1, y1, linestyle->ms, linestyle->ms,\
                          -90, 0,\
                          GetCouleur(fig->img, linestyle->color), gdArc);
    } else {
        gdImageFilledEllipse(fig->img, x1, y1,\
            linestyle->ms, linestyle->ms, GetCouleur(fig->img, linestyle->color));
    }

}

/* --------------------------------------------------------------------------- */
void PlotLine(Figure *fig, LineData *linedata)
{
    int *x_plot = Transform_data_to_plot(fig, linedata->len_data, linedata->x, 'x');
    int *y_plot = Transform_data_to_plot(fig, linedata->len_data, linedata->y, 'y');   

    for (int i=0; i < linedata->len_data-1; i++) 
    {
        ImageLineEpaisseur(fig->img,\
                x_plot[i]   + fig->orig[0],   y_plot[i] + fig->orig[1],\
                x_plot[i+1] + fig->orig[0], y_plot[i+1] + fig->orig[1],\
                linedata->linestyle);

        
        /* PlotPoint(fig,\
            x_plot[i] + fig->orig[0], y_plot[i] + fig->orig[1], linedata->linestyle);

        if (i == linedata->len_data-2) {
            PlotPoint(fig,\
                            x_plot[i+1] + fig->orig[0],\
                            y_plot[i+1] + fig->orig[1],\
                            linedata->linestyle);
        } */

    }

    free(x_plot);
    free(y_plot);
}

int *Transform_data_to_plot_bar(Figure *fig, size_t nb_ctg, \
                                    const int pts[]);


/* --------------------------------------------------------------------------- */
void Change_font(Figure *fig, int textType, char* path_f)
{
    fig->fonts[textType].path = path_f;
}

/* --------------------------------------------------------------------------- */
void Change_fontsize(Figure *fig, int textType, int size)
{
    fig->fonts[textType].size = size;
}

/* --------------------------------------------------------------------------- */
void Change_fontcolor(Figure *fig, int textType, int color[3])
{
    for (int i = 0; i < 3; i++)
        fig->fonts[textType].color[i] = color[i];
}


/* --------------------------------------------------------------------------- */
void PlotBarplot(Figure *fig, BarData *bardata, char wlabels)
{
    int *y_bars = \
        Transform_data_to_plot_bar(fig, bardata->nb_ctg, bardata->nb_in_ctg);   

    // largeur zone de dessin
    int w_dessin = (fig->img->sx-1) - fig->orig[0] - fig->margin[0] - fig->padX[1];

    // intervalle en px entre 2 centres de barplot selon X
    int itv_posX = w_dessin/(fig->nb_bardata+1); 
    
    // position des ticks/centre des barplot selon X
    int posX_center = fig->orig[0] + bardata->idx * itv_posX;

    // Plot les rectangles
    int y1_rect = 0;
    int y2_rect = fig->orig[1];

    for (int ctg = 0; ctg < bardata->nb_ctg; ctg++)
    {
        y1_rect = fig->orig[1] + y_bars[ctg];

        // Dessins rect 
        if (y1_rect != y2_rect) {
            // Ajout des labels
            if (wlabels == 'y') {
                int posX_label = posX_center + itv_posX/3 - 6;
                int posY_label = y2_rect - (y2_rect-y1_rect)/2 \
                                + fig->fonts[label_f].size / 2;

                char nb_in_ctg[fig->max_Y]; // pas de surprises
                sprintf(nb_in_ctg, "%d", bardata->nb_in_ctg[ctg]);

                gdImageStringFT(fig->img, NULL,\
                            GetCouleur(fig->img,\
                            bardata->colors[ctg]),\
                            fig->fonts[title_f].path,\
                            fig->fonts[label_f].size,\
                            0.,\
                            posX_label,\
                            posY_label,\
                            nb_in_ctg);  
            }

            // Plot rect
            gdImageFilledRectangle(fig->img, \
                posX_center - itv_posX/4, y1_rect,\
                posX_center + itv_posX/4, y2_rect,\
                GetCouleur(fig->img, bardata->colors[ctg]));
        }

        // Update y2=y1
        y2_rect = y1_rect;

    }

    free(y_bars);
}

/* --------------------------------------------------------------------------- */
int *Transform_data_to_plot_bar(Figure *fig, size_t nb_ctg, \
                                    const int pts[])
{
    const int h_dessin = fig->orig[1] - fig->padY[0] - fig->margin[1];   
    
    // MALLOC
    int *y_dessin_ctg = malloc(nb_ctg * sizeof(int));

    for (int i = 0; i < nb_ctg; i++) {
        if (i == 0)
            y_dessin_ctg[i] = -(pts[i] * h_dessin) / fig->max_Y;
        else
            y_dessin_ctg[i] = y_dessin_ctg[i-1] - (pts[i] * h_dessin) / fig->max_Y;
    }

    return y_dessin_ctg;
}      

/* --------------------------------------------------------------------------- */
void Transform_dataX_to_plot(Figure *fig, size_t len_pts,\
                                    const int pts[], int* pts_dessin)
{
    // Taile de la zone de dessin
    // (Nx-1) - orig X (0) - margin X (0) - padX droite (1)
    const int w_dessin = (fig->img->sx-1) - fig->orig[0] - fig->margin[0] - fig->padX[1];

    for (int i = 0; i < len_pts; i++)
    {
        pts_dessin[i] = (pts[i] * w_dessin) / fig->max_X;
        // printf("Point : %d, %d \n", i, pts_dessin[i]);
    }

}
                           

/* --------------------------------------------------------------------------- */
void Transform_dataY_to_plot(Figure *fig, size_t len_pts, \
                                    const int pts[], int* pts_dessin)
{
    // Taile de la zone de dessin
    // Orig - padding haut (0) - margin Y (1)
    const int h_dessin = fig->orig[1] - fig->padY[0] - fig->margin[1];   
    for (int i = 0; i < len_pts; i++){
        pts_dessin[i] = -(pts[i] * h_dessin) / fig->max_Y;
        // printf("Point : %d, %d \n", i, pts_dessin[i]);
    }

}

/* --------------------------------------------------------------------------- */
int *Transform_data_to_plot(Figure *fig, size_t len_pts,\
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
        Transform_dataX_to_plot(fig, len_pts, pts, pts_dessin);
    } else if (direction == 'y')
    {
        Transform_dataY_to_plot(fig, len_pts, pts, pts_dessin);
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
int GetCouleur(gdImagePtr im_fig, const int couleur[3])
{
    return gdImageColorAllocate(im_fig, couleur[0], couleur[1], couleur[2]);
}

/* --------------------------------------------------------------------------- */
void Init_figure(Figure *fig, int figsize[2],\
                int padX[2], int padY[2], int margin[2], char wAxes)
{

    /* Initialisation de tous les parametres*/

    // ATTENTION ! Important d'utiliser True Color au lieu de gdImageColor ... 
    // Ecrasement des plots un peu bizarre sinon ...
    fig->img = gdImageCreateTrueColor(figsize[0],figsize[1]);

    fig->padX[0] = padX[0]; /**< left */
    fig->padX[1] = padX[1]; /**< right */
    fig->padY[0] = padY[0]; /**< haut */
    fig->padY[1] = padY[1]; /**< bas */   
    fig->orig[0] = fig->padX[0];
    fig->orig[1] = (fig->img->sy-1) - fig->padY[1];
    fig->margin[0] = margin[0];
    fig->margin[1] = margin[1];

    int col_fond[3] = {37, 44, 56};     // bleu-gris
    int col_cvs[3] = {0, 0, 0}; 
    int col_axes[3] = {255, 255, 255}; 

    for (int i = 0; i < 3; i++) {
        fig->color_bg[i] = col_fond[i];
        fig->color_cvs_bg[i] = col_cvs[i];        
        fig->color_axes[i] = col_axes[i];
    }

    fig->nb_linedata = 0;
    fig->nb_bardata  = 0;
    fig->linedata = NULL;
    fig->bardata = NULL;

    // Fonts
    fig->fonts[label_f].path = "fonts/Lato-Regular.ttf";
    fig->fonts[title_f].path = "fonts/Lato-Medium.ttf";
    fig->fonts[annotation_f].path = "fonts/Lato-LightItalic.ttf";
    fig->fonts[ticklabel_f].path = "fonts/Lato-Regular.ttf";
    fig->fonts[subtitle_f].path = "fonts/Lato-Medium.ttf";
    fig->fonts[leg_f].path = "fonts/Lato-Regular.ttf";

    fig->fonts[label_f].size = 15;
    fig->fonts[title_f].size = 18;
    fig->fonts[annotation_f].size = 10;
    fig->fonts[ticklabel_f].size = 13;
    fig->fonts[subtitle_f].size = 14;
    fig->fonts[leg_f].size = 11;

    for (int i = 0; i < 3; i++) {
        fig->fonts[label_f].color[i] = col_axes[i];
        fig->fonts[title_f].color[i] = col_axes[i];
        fig->fonts[annotation_f].color[i] = col_axes[i];
        fig->fonts[ticklabel_f].color[i] = col_axes[i];
        fig->fonts[subtitle_f].color[i] = col_axes[i];
        fig->fonts[leg_f].color[i] = col_axes[i];
    }
    
    // Lenovo PC
    // fig.fonts[0] = "/usr/share/fonts/lato/Lato-Light.ttf";
    // fig.fonts[1] = "/usr/share/fonts/lato/Lato-Medium.ttf";
    // fig.fonts[2] = "/usr/share/fonts/lato/Lato-LightItalic.ttf";

    Make_background(fig, fig->color_bg, fig->color_cvs_bg);
    if ( wAxes == 'y')
        Make_support_axes(fig, fig->color_axes);

    fig->max_X = 0;
    fig->max_Y = 0;
}

/* --------------------------------------------------------------------------- */
void Change_fig_bg(Figure *fig, int color[3]) 
{
    for (int i = 0; i < 3; i++)
    {
        fig->color_bg[i] = color[i];
    }
    Make_background(fig, fig->color_bg, fig->color_cvs_bg);                                           
}

/* --------------------------------------------------------------------------- */
void Change_fig_cvs_bg(Figure *fig, int color[3]) 
{
    for (int i = 0; i < 3; i++)
    {
        fig->color_cvs_bg[i] = color[i];        
    }
    Make_background(fig, fig->color_bg, fig->color_cvs_bg);
}

/* --------------------------------------------------------------------------- */
void Change_fig_axes_color(Figure *fig, int color[3]) 
{
    for (int i = 0; i < 3; i++)
    {
        fig->color_axes[i] = color[i];
    }
    Make_support_axes(fig, fig->color_axes);
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
void Init_linedata(LineData *linedata, int len_data, int ptx[], int pty[],
                    char* label, LineStyle *linestyle)
{
    // int *ptx_plot = Transform_data_to_plot(fig, len_data, ptx, 'x');
    // int *pty_plot = Transform_data_to_plot(fig, len_data, pty, 'y');
    
    linedata->len_data = len_data;
    linedata->x = ptx; 
    linedata->y = pty;
    linedata->max_X = Maxval_array(linedata->x, linedata->len_data);
    linedata->max_Y = Maxval_array(linedata->y, linedata->len_data);
    linedata->label = label;
    linedata->linestyle = linestyle;
    // print_arr1D(len_data, linedata->y, 'n');
}

/* --------------------------------------------------------------------------- */
void Init_linestyle(LineStyle *linestyle, char style,\
        const int color[3], int width, char marker, int ms)
{
    linestyle->style = style;
    for (int i = 0; i < 3; i++)
    {
        linestyle->color[i] = color[i];
    }    
    linestyle->w = width;
    linestyle->marker = marker;
    linestyle->ms = ms;
}

/* --------------------------------------------------------------------------- */
void Print_debug_ls(LineStyle *linestyle)
{
    printf("*** Debug linestyle\n");
    printf("* Style        = %c \n", linestyle->style);
    printf("* Width        = %d \n", linestyle->w);
    printf("* Marker       = %c \n", linestyle->marker);
    printf("* MarkerSize   = %d \n", linestyle->ms);
    printf("* Color        = %d,%d,%d \n", linestyle->color[0],\
                                           linestyle->color[1],\
                                           linestyle->color[2]);
    printf("*** End linestyle\n");
}

/* --------------------------------------------------------------------------- */
void Print_debug_ld(LineData *linedata, char w_xy)
{
    printf("*** Debug linedata --------------------------\n");
    printf("* Label    = %s \n", linedata->label);
    printf("* Len data = %ld \n", linedata->len_data);
    printf("* Max X    = %d \n", linedata->max_X);
    printf("* Max Y    = %d \n", linedata->max_Y);
    if (w_xy == 'y') {
        printf("* Vecteur X : \n");
        print_arr1D(linedata->len_data, linedata->x, 'n');
        printf("* Vecteur Y : \n");
        print_arr1D(linedata->len_data, linedata->y, 'n');
    }    
    Print_debug_ls(linedata->linestyle);
    printf("*** End linedata ----------------------------\n\n");
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

/* --------------------------------------------------------------------------- */
int Max_int(int x, int y) 
{
    return (x < y) ? y : x ;
}

/* --------------------------------------------------------------------------- */
int Min_int(int x, int y) 
{
    return (x > y) ? y : x ;
}

/* --------------------------------------------------------------------------- */
void Add_line_to_fig(Figure *fig, LineData *linedata)
{
    fig->nb_linedata++;
    fig->linedata = (LineData **)realloc(fig->linedata, fig->nb_linedata*sizeof(LineData));

    if (fig->linedata == NULL) {
        printf("Erreur : Pas assez de memoire.\n");
        free(fig->linedata);
        exit(EXIT_FAILURE);
    }

    // Index = nb line - 1 : on remplit les linedata de la figure
    fig->linedata[fig->nb_linedata-1] = linedata;

    // Print_debug_ld(fig->linedata[fig->nb_linedata]);

    fig->max_X = Max_int(fig->max_X, linedata->max_X);
    fig->max_Y = Max_int(fig->max_Y, linedata->max_Y);

}


/* --------------------------------------------------------------------------- */
void Add_barplot_to_fig(Figure *fig, BarData *bardata)
{
    fig->nb_bardata++;
    fig->bardata = (BarData **)realloc(fig->bardata, fig->nb_bardata*sizeof(BarData));

    if (fig->bardata== NULL) {
        printf("Erreur : Pas assez de memoire.\n");
        free(fig->bardata);
        exit(EXIT_FAILURE);
    }

    // Index = nb line - 1 : on remplit les linedata de la figure
    fig->bardata[fig->nb_bardata-1] = bardata;

    // Print_debug_ld(fig->linedata[fig->nb_linedata]);

    fig->max_X = 0;
    fig->max_Y = Max_int(fig->max_Y, bardata->nb_tot);

    bardata->idx = fig->nb_bardata; // On update l'index du bardata, pour posX
}

/* --------------------------------------------------------------------------- */
void Make_annotation(Figure *fig, char* text,\
                    int decalage_X, int decalage_Y)
{
    // Pos avec decalage : Y decal autre sens
    // Prise en compte de la longueur du label
    int posX_xlabel = 5 + decalage_X;
    int posY_xlabel = (fig->img->sy-1) - 5 - decalage_Y; 

    // int brect[8] = {0};
    gdImageStringFT(fig->img, NULL,\
                    GetCouleur(fig->img,
                    fig->fonts[annotation_f].color),\
                    fig->fonts[annotation_f].path,\
                    fig->fonts[annotation_f].size,\
                    0., posX_xlabel, posY_xlabel, text);

    //Avoid memory leaks
    gdFontCacheShutdown();
    // printf("%s \n", errStringFT); 
}

/* --------------------------------------------------------------------------- */
void Make_subtitle(Figure *fig, char* subtitle,\
                int bbox_title[8], int decalage_X, int decalage_Y)
{

    int ecartY_title = 6;
    int posX_subtitle = bbox_title[0] + (bbox_title[2]-bbox_title[0])/2 \
                    - strlen(subtitle) * fig->fonts[subtitle_f].size / 3;
    int posY_subtitle = bbox_title[1] + fig->fonts[subtitle_f].size \
                    + ecartY_title - decalage_Y; 

    gdImageStringFT(fig->img, NULL,\
                            GetCouleur(fig->img,\
                                fig->fonts[subtitle_f].color),
                            fig->fonts[subtitle_f].path,\
                            fig->fonts[subtitle_f].size,\
                            0., posX_subtitle, posY_subtitle, subtitle);    
    //Avoid memory leaks
    gdFontCacheShutdown();
}

/* --------------------------------------------------------------------------- */
void Const_str_dudate1_audate2(Date *date1, Date *date2, char* str_out)
{
    Datetick datetick_debut;
    Datetick datetick_fin;
    Init_Datetick(&datetick_debut, date1,date1);
    Init_Datetick(&datetick_fin, date2, date2);

    strcat(str_out, "du ");
    strcat(str_out, datetick_debut.labeldate);
    strcat(str_out, " au ");
    strcat(str_out, datetick_fin.labeldate);
}

/* --------------------------------------------------------------------------- */
int *Make_title(Figure *fig, char* title,\
                    int decalage_X, int decalage_Y)
{
    // Pos avec decalage : Y decal autre sens
    // Prise en compte de la longueur du label
    int largeur_cvs = (fig->img->sx-1 - fig->padX[0] - fig->padX[1]);
    int centre_cvs =  fig->orig[0] + largeur_cvs/2;
    int posX_xlabel = centre_cvs - (strlen(title) \
                        * fig->fonts[title_f].size/3.7) + decalage_X;
    int posY_xlabel = fig->img->sy - (fig->padY[1] - 3*fig->padY[1]/4) - decalage_Y; 

    static int brect_title[8] = {0};
    gdImageStringFT(fig->img, brect_title,\
                            GetCouleur(fig->img,\
                                fig->fonts[title_f].color),\
                            fig->fonts[title_f].path,\
                            fig->fonts[title_f].size,\
                            0., posX_xlabel, posY_xlabel, title);    
    //Avoid memory leaks
    gdFontCacheShutdown();
    // printf("%s \n", errStringFT); 

    return brect_title;
}


/* --------------------------------------------------------------------------- */
void Make_xlabel(Figure *fig, char* xlabel,\
                    int decalage_X, int decalage_Y)
{
    // Pos avec decalage : Y decal autre sens
    // Prise en compte de la longueur du label
    int posX_xlabel = fig->orig[0] +\
                     (fig->img->sx - fig->padX[0] - fig->padX[1])/2 \
                  - (strlen(xlabel) * fig->fonts[label_f].size / 3) + decalage_X;
                  
    int posY_xlabel = fig->img->sy - (fig->padY[1] - fig->padY[1]/3) + decalage_Y; 

    // int brect[8] = {0};
    gdImageStringFT(fig->img, NULL,\
                            GetCouleur(fig->img,\
                                fig->fonts[label_f].color),\
                            fig->fonts[label_f].path,\
                            fig->fonts[label_f].size,\
                            0., posX_xlabel, posY_xlabel, xlabel);    
    //Avoid memory leaks
    gdFontCacheShutdown();
    // printf("%s \n", errStringFT); 
}


/* --------------------------------------------------------------------------- */
void Make_ylabel(Figure *fig, char* ylabel,\
                     int decalage_X, int decalage_Y)
{
    // Pos avec decalage : Y decal autre sens
    int posX_ylabel = fig->padX[0]/2 - fig->padX[0]/4      + decalage_X;

    // Prise en compte de la longueur du label
    int posY_ylabel = (fig->orig[1] + (fig->padY[0]))/2 + \
                            (strlen(ylabel) * fig->fonts[label_f].size/3)\
                             + decalage_Y;   

    int brect[8] = {0};
    char *errStringFT = gdImageStringFT(fig->img, brect,\
                            GetCouleur(fig->img,\
                            fig->fonts[label_f].color),
                            fig->fonts[label_f].path,\
                            fig->fonts[label_f].size,\
                            Deg2rad(90.), posX_ylabel, posY_ylabel, ylabel);    

    if (errStringFT != NULL)
        printf("> Warning: police introuvable ou autre probleme police.\n");

    //Avoid memory leaks
    gdFontCacheShutdown();
}

void Make_xticks_barplot(Figure *fig, float angle_labels);
void Make_legend_barplot(Figure *fig,\
                    int decal_X, int decal_Y, int ecart);


/* --------------------------------------------------------------------------- */
void Make_legend_barplot(Figure *fig,\
                    int decal_X, int decal_Y, int ecart)
{
    // Position des 4 legendes possibles
    int w_dessin = (fig->img->sx-1) - fig->orig[0] - fig->margin[0] - fig->padX[1];
    int pos_midX = fig->orig[0] + w_dessin/2 + 5;
    
    int pos_X[4] = {\
                fig->orig[0] + 5 + decal_X, fig->orig[0] + 5 + decal_X,\
                pos_midX + decal_X, pos_midX + decal_X};

    int ecart_base = 20;
    int ecart_Y = ecart_base + ecart;

    int pos_Y[4] = {  (fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    2*(fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                      (fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    2*(fig->fonts[leg_f].size + ecart_Y) - decal_Y};

    LineStyle linestyle1;
    Init_linestyle(&linestyle1, ' ', color_lines[0], 0, 'o', 4);

    int h_rect = 15;
    int l_rect = 30;
    int posX_label = 0;
    int posY_label = 0;

    for (int i = 0; i < fig->bardata[0]->nb_ctg ; i++) {
        // Plot petits rectangles legendes
        gdImageFilledRectangle(fig->img,\
                    pos_X[i], pos_Y[i]-h_rect,\
                    pos_X[i]+l_rect, pos_Y[i],\
                    GetCouleur(fig->img, fig->bardata[0]->colors[i]));

        // Labels
        posX_label = pos_X[i]+l_rect+10;
        posY_label = pos_Y[i] ;
        gdImageStringFT(fig->img, NULL,\
                            GetCouleur(fig->img,\
                            fig->fonts[leg_f].color),
                            fig->fonts[leg_f].path,\
                            fig->fonts[leg_f].size,\
                            0, posX_label, posY_label,\
                                fig->bardata[0]->ctg_names[i]);  
        
    }                 

}

/* --------------------------------------------------------------------------- */
void Make_legend(Figure *fig,\
                    int decal_X, int decal_Y, int ecart)
{
    // Position des 8 legendes possibles
    int w_img = fig->img->sx;
    int pos_midX = w_img/2 + 5;
    // printf("Done here.\n");

    int pos_X[8] = {fig->orig[0] + 5 + decal_X, fig->orig[0] + 5  + decal_X,\
                    fig->orig[0] + 5  + decal_X, fig->orig[0] + 5  + decal_X,\
                    pos_midX + decal_X, pos_midX + decal_X,\
                    pos_midX + decal_X, pos_midX + decal_X};

    int ecart_base = 8;
    int ecart_Y = ecart_base + ecart;
    int pos_Y[8] = {  (fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    2*(fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    3*(fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    4*(fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                      (fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    2*(fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    3*(fig->fonts[leg_f].size + ecart_Y) - decal_Y,\
                    4*(fig->fonts[leg_f].size + ecart_Y) - decal_Y};

    // Longueur petit trait
    int long_trait_leg = 30;
    // Decalage apres trait
    int decalage_trait_leg = long_trait_leg + 10;

    // Print des petits traits de legende pour chaque plot
    for (int i = 0; i < fig->nb_linedata; i++) { 
        ImageLineEpaisseur(fig->img,\
                    pos_X[i],\
                    pos_Y[i] - fig->fonts[leg_f].size/2,\
                    pos_X[i] + long_trait_leg,\
                    pos_Y[i] - fig->fonts[leg_f].size/2,\
                    fig->linedata[i]->linestyle);
    }

    char *errStringFT;
    int brect[8] = {0};

    for (int i = 0; i < fig->nb_linedata; i++)
    {
        errStringFT= gdImageStringFT(fig->img, brect,\
                            GetCouleur(fig->img,\
                                fig->fonts[leg_f].color),\
                            fig->fonts[leg_f].path,\
                            fig->fonts[leg_f].size,\
                            0.,\
                            decalage_trait_leg + pos_X[i],\
                            pos_Y[i],\
                            fig->linedata[i]->label);        
    }

    if (errStringFT != NULL) {
        printf("%s\n", errStringFT);
    }

    //Avoid memory leaks
    gdFontCacheShutdown();
}


/* --------------------------------------------------------------------------- */
void Make_xticks_barplot(Figure *fig, float angle_labels)
{
    // largeur zone de dessin
    int w_dessin = (fig->img->sx-1) - fig->orig[0] - fig->margin[0] - fig->padX[1];

    // intervalle en px entre 2 centres de barplot selon X
    int itv_posX = w_dessin/(fig->nb_bardata+1); 
    
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
    Init_linestyle(&style_tick, '-', fig->color_axes, w_tick, 'o', 8);

    for (int bp = 0; bp < fig->nb_bardata ; bp ++)
    {
        // pos centre de la barre
        posX_center = fig->orig[0] + fig->bardata[bp]->idx * itv_posX;

        // plot tick
        ImageLineEpaisseur(fig->img,\
                        posX_center,\
                        fig->orig[1]+2,\
                        posX_center,\
                        fig->orig[1] + long_tick +2,\
                        &style_tick);
        

        // tick label
        // Fake call : recuperation du brect sur le label horizontal
        // On ecrit en 0,0
        gdImageStringFT(fig->img, brect,\
                            gdTransparent,\
                            fig->fonts[ticklabel_f].path,\
                            fig->fonts[ticklabel_f].size, 0.,\
                            0, 0,\
                            fig->bardata[bp]->label);   

        // Longueur de l'adresse en px                                
        len_label_px = brect[2] - brect[0];

        posX_label = (int)(len_label_px * cos(Deg2rad(angle_labels)));
        posY_label = (int)(len_label_px * sin(Deg2rad(angle_labels)));

        gdImageStringFT(fig->img, NULL,\
                            GetCouleur(fig->img,\
                                    color_lines[bp]),\
                            fig->fonts[ticklabel_f].path,\
                            fig->fonts[ticklabel_f].size,\
                            Deg2rad(angle_labels),\
                            posX_center - posX_label,\
                            fig->orig[1] + (long_tick*3) + posY_label,\
                            fig->bardata[bp]->label);

    }

    //Avoid memory leaks
    gdFontCacheShutdown();

}

/* --------------------------------------------------------------------------- */
void Make_xticks_xgrid_time(Figure *fig, Date date_init)
{
    if (fig->max_X == 0)
    {
        printf("Erreur : pas de ticks pour un max a 0.");
        exit(EXIT_FAILURE);
    }
    
    int nb_ticks = 7;
    int nb_itv = nb_ticks-1;

    // Intervalles entre 2 ticks en sec
    int itv_sec = fig->max_X / nb_itv;

    // Largeur canvas
    int l_canvas = (fig->img->sx-1) - fig->orig[0] - fig->padX[1];
    // On pense a enlever la marge Y pour calculer l'ecart entre tick maj
    int l_max = l_canvas - fig->margin[0];

    // Intervalle entre 2 ticks en pix
    int itv_pixels = (itv_sec*l_max) / fig->max_X;
    // printf("Itv X en px = %d \n", itv_pixels);

    // Style tick
    int w_tick = 2;
    int w_linegrid = 0.5;
    int long_tick = 9;

    LineStyle style_tick;
    Init_linestyle(&style_tick, '-', fig->color_axes, w_tick, ' ', ' ');

    LineStyle style_linegrid;
    Init_linestyle(&style_linegrid, '-', gris_grid, w_linegrid, ' ', ' ');

    // Plot ticks
    // printf("Nb ticks : %d \n", nb_ticks);
    for (int i = 0; i < nb_ticks; i++)
    {
        // printf("Tick : %d \n", i);

        // tick
        ImageLineEpaisseur(fig->img,\
                        fig->orig[0] + i*itv_pixels,\
                        fig->orig[1]+2,\
                        fig->orig[0] + i*itv_pixels,\
                        fig->orig[1] + long_tick +2,\
                        &style_tick);

        //gridline
        ImageLineEpaisseur(fig->img,\
                        fig->orig[0] + i*itv_pixels,\
                        fig->orig[1],\
                        fig->orig[0] + i*itv_pixels,\
                        fig->padY[0],\
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


        int xlabel_date = fig->padX[0]/2 -10 + i*itv_pixels;
        int ylabel_date = fig->orig[1] + (long_tick+2) + fig->fonts[ticklabel_f].size;
        // tick label date
        gdImageStringFT(fig->img, NULL,\
                            GetCouleur(fig->img,\
                                fig->fonts[ticklabel_f].color),\
                            fig->fonts[ticklabel_f].path,\
                            fig->fonts[ticklabel_f].size,\
                            0.,\
                            xlabel_date, ylabel_date, tickdate);

        int xlabel_hour = fig->padX[0]/2 -10 + i*itv_pixels + fig->fonts[ticklabel_f].size/2;
        int ylabel_hour = fig->orig[1] + 2*((long_tick) + fig->fonts[ticklabel_f].size);
        // tick label heure
        gdImageStringFT(fig->img, NULL,\
                            GetCouleur(fig->img,\
                                fig->fonts[ticklabel_f].color),\
                            fig->fonts[ticklabel_f].path,\
                            fig->fonts[ticklabel_f].size,\
                            0.,xlabel_hour, ylabel_hour, tickhour);
                                
        // printf("%d\n", i);
    }

    //Avoid memory leaks
    gdFontCacheShutdown();

}

/* --------------------------------------------------------------------------- */
void Make_yticks_ygrid(Figure *fig, char wTicks)
{
    if (fig->max_Y == 0)
    {
        printf("Erreur : pas de ticks pour un max a 0.");
        exit(EXIT_FAILURE);
    }

    // int nb_ticks = Min_int(fig->max_Y, 10);
    // printf("Nb ticks  = %d \n", nb_ticks);

    // int itv = fig->max_Y / nb_ticks;
    // printf("Itv       = %d \n", itv);

    int itv=0, itv_minor=0;
    if (fig->max_Y <= 10) {
        itv = 1;
        itv_minor = 0;
    } else if (fig->max_Y <= 20)
    {
        itv = 2;
        itv_minor = 1;
    } else if (fig->max_Y <= 50)
    {
        itv = 5;
        itv_minor = 1;
    } else if (fig->max_Y <= 100)
    {
        itv = 10;
        itv_minor = 2;
    }
    
    // printf("Itv       = %d \n", itv);
    // printf("Itv minor = %d \n", itv_minor);
    int nb_ticks = fig->max_Y / itv;

    int nb_ticks_min = 0;    
    if (itv_minor != 0) {
        nb_ticks_min= (fig->max_Y / itv_minor) / nb_ticks;
        // printf("Nb ticks min = %d \n", nb_ticks_min);
    }    
    // printf("Nb ticks  = %d \n", nb_ticks);

    int h_canvas = fig->img->sy - fig->padY[0] - fig->padY[1];
    // On pense a enlever la marge Y pour calculer l'ecart entre tick maj
    // Pos de maxY repere figure = h_canvas - fig->margin[1]
    int h_max = h_canvas - fig->margin[1];
    // printf("h max     = %d \n", h_max);

    // int ecart_major = ( h_canvas - fig->margin[1] ) / (nb_interv)  ; 

    int itv_pixels = (itv*h_max) / fig->max_Y;
    // printf("Itv px     = %d \n", itv_pixels);

    // Style tick
    int w_tick = 2;
    int w_linegrid = 0.5;
    LineStyle style_tick;
    Init_linestyle(&style_tick, '-', fig->color_axes, w_tick, ' ', ' ');
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
        ImageLineEpaisseur(fig->img,\
                        fig->orig[0],\
                            fig->orig[1] - i*(itv_pixels+0.5),\
                        (fig->img->sx-1) - fig->padX[1],\
                            fig->orig[1] - i*(itv_pixels+0.5),\
                        &style_linegrid);        

        if (wTicks == 'y') {
            long_tick = 9;
            long_tick_min = 5;
            // tick : facteur 0.5 ajouté pour gerer les nombres pairs/impais de px
            ImageLineEpaisseur(fig->img,\
                            fig->orig[0]-long_tick-2,\
                                fig->orig[1] - i*(itv_pixels+0.5),\
                            fig->orig[0]-2,\
                                fig->orig[1] - i*(itv_pixels+0.5),\
                            &style_tick);

            if (itv_minor != 0) {

                for (int j = 1; j < nb_ticks_min; j++)
                {
                    int itv_ytickmin = fig->orig[1] - (i-1)*itv_pixels - j*(itv_pixels/(nb_ticks_min));
                    
                    ImageLineEpaisseur(fig->img,\
                                    fig->orig[0]-long_tick_min-2,\
                                        itv_ytickmin,\
                                    fig->orig[0]-2,\
                                        itv_ytickmin,\
                                    &style_tick);            
                }
            }
        }

        int posX_ticklab = fig->orig[0]-(long_tick+2) \
                            - strlen(tickVal) * fig->fonts[ticklabel_f].size;
        int posY_ticklab = fig->orig[1] - i*itv_pixels \
                            + fig->fonts[ticklabel_f].size / 2;

        gdImageStringFT(fig->img, NULL,\
                            GetCouleur(fig->img,\
                                    style_tick.color),\
                            fig->fonts[ticklabel_f].path,\
                            fig->fonts[ticklabel_f].size,\
                                0., posX_ticklab, posY_ticklab, tickVal);

    }

    //Avoid memory leaks
    gdFontCacheShutdown();
}

#endif /* PLOTTER_H */