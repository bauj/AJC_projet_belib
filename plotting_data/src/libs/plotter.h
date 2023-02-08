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
#include "traitement.h"
#include "getter.h"
#include <stdlib.h>
#include <gd.h>

/* --------------------------------------------------------------------------- */
typedef enum {label_f, annotation_f, title_f, ticklabel_f, subtitle_f, leg_f} fontsFig;

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
    Font fonts[6];       /**< Fonts used in the fig*/
    int padX[2];         /**< Padding gauche/droite pour definir canvas (cvs) et orig */
    int padY[2];         /**< Padding haut/bas pour definir canvas (cvs) et orig */
    int orig[2];         /**< Coordonnees de l'origine des plots */    
    int margin[2];       /**< Marge droite et haut (evite de toucher les bords)*/    
    int color_bg[3];     /**< Couleur du fond de la figure */
    int color_cvs_bg[3]; /**< Couleur du fond du canvas */
    int color_axes[3];   /**< Couleur des axes*/
} Figure;




/* --------------------------------------------------------------------------- */
// Declaration fonctions
/* --------------------------------------------------------------------------- */



/* --------------------------------------------------------------------------- */
int GetCouleur(gdImagePtr im_fig, const int couleur[3]);

/* --------------------------------------------------------------------------- */
void ImageLineEpaisseur(gdImagePtr im_fig, const int x1, const int y1, const int x2, const int y2, LineStyle *linestyle);

/* --------------------------------------------------------------------------- */
void Change_fig_axes_color(Figure *fig, int color[3]);

/* --------------------------------------------------------------------------- */
void Change_fig_cvs_bg(Figure *fig, int color[3]);

/* --------------------------------------------------------------------------- */
void Change_fig_bg(Figure *fig, int color[3]);

/* --------------------------------------------------------------------------- */
void Change_font(Figure *fig, int textType, char* path_f);

/* --------------------------------------------------------------------------- */
void Change_fontsize(Figure *fig, int textType, int size);

/* --------------------------------------------------------------------------- */
void Change_fontcolor(Figure *fig, int textType, int color[3]);

/* --------------------------------------------------------------------------- */
void Make_background(Figure *fig, const int couleur_bg[3], const int couleur_canvas_bg[3]);

/* --------------------------------------------------------------------------- */
void Make_support_axes(Figure *fig, const int couleur[3]);

/* --------------------------------------------------------------------------- */
void Init_figure(Figure *fig, int figsize[2],\
            int padX[2], int padY[2], int margin[2], char wAxes);


/* --------------------------------------------------------------------------- */
void Init_linestyle(LineStyle *linestyle, char style,const int color[3], int width, \
            char marker, int ms);

/* --------------------------------------------------------------------------- */
void Print_debug_ls(LineStyle *linestyle);

/* --------------------------------------------------------------------------- */
void Make_annotation(Figure *fig, char* text,\
                    int decalage_X, int decalage_Y);

/* --------------------------------------------------------------------------- */
void Make_subtitle(Figure *fig, char* subtitle,\
                int bbox_title[8], int decalage_X, int decalage_Y);

/* --------------------------------------------------------------------------- */
void Const_str_dudate1_audate2(Date *date1, Date *date2, char* str_out);

/* --------------------------------------------------------------------------- */
int *Make_title(Figure *fig, char* title,\
                    int decalage_X, int decalage_Y);
/* --------------------------------------------------------------------------- */
void Make_xlabel(Figure *fig, char* xlabel,\
             int decalage_X, int decalage_Y);

/* --------------------------------------------------------------------------- */
void Make_ylabel(Figure *fig, char* ylabel,\
             int decalage_X, int decalage_Y);

/* --------------------------------------------------------------------------- */
void PlotPoint(Figure *fig, const int x1, const int y1, LineStyle *linestyle);

/* --------------------------------------------------------------------------- */
void Save_to_png(Figure *fig, const char *dir_figures, const char *filename_fig);

/* --------------------------------------------------------------------------- */
// Définition des fonctions
/* --------------------------------------------------------------------------- */


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
    
    if (linestyle->style == '-') {
        gdImageLine(im_fig, x1, y1,   x2,   y2,\
                   GetCouleur(im_fig, linestyle->color));
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
int GetCouleur(gdImagePtr im_fig, const int couleur[3])
{
    return gdImageColorAllocate(im_fig, couleur[0], couleur[1], couleur[2]);
}

/* --------------------------------------------------------------------------- */
void Init_figure(Figure *fig, int figsize[2],\
                int padX[2], int padY[2], int margin[2], char wAxes)
{

    /* Initialisation de tous les parametres d'affichage */

    // ATTENTION ! Important d'utiliser True Color au lieu de gdImageColor ... 
    // Ecrasement des plots un peu bizarre sinon ...

    fig->img = gdImageCreateTrueColor(figsize[0],figsize[1]);
    printf("INSIDE INIT FIGURE \n");

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





#endif /* PLOTTER_H */