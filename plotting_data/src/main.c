/* ----------------------------------------------------------------------------
*  Programme permettant d'ouvrir la bdd SQLite contenant les donnees belib et
*  de plot les infos interessantes.
*  
*  Author : Juba Hamma. 2023.
* ---------------------------------------------------------------------------- 
*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <gd.h>

#define PI 3.141592

/* --------------------------------------------------------------------------- */
// Structures
/* --------------------------------------------------------------------------- */

typedef struct LineStyle_s {
    char style;     /**< Style de plot : '-' : line, ':' dashed,
                                          '' : point seulement */
    int  w;         /**< Epaisseur du trait*/
    char marker;    /**< Type de marker : 's' : carré, 'o' : cercle,
                                           '' : pas de marker */
    int  ms;        /**< Markersize*/
    int  color[3];  /**< Couleur rgb*/
} LineStyle; // = {'-', 3, 'o', 8, {255,255,255}};
// typedef struct LineStyle_s LineStyle;

/* --------------------------------------------------------------------------- */
typedef struct LineData_s {
    size_t len_data;      /**< Taille du vecteur de data */
    int *x;               /**< Vecteur de data X */
    int *y;               /**< Vecteur de data Y */
    int max_X;            /**< Maximum des valeurs X */
    int max_Y;            /**< Maximum des valeurs Y */
    LineStyle *linestyle;  /**< LineStyle */
} LineData; // = {0, 0, 0, 0, 0, {'-', 3, 'o', 8, {255,255,255}}};
// typedef struct LineData_s LineData;

/* --------------------------------------------------------------------------- */
typedef struct Figure {
    gdImage *img;        /**< Pointeur sur objet gdImage */
    size_t nb_linedata;  /**< Nombre de linedata dans la figure */
    LineData **linedata;  /**< Vecteur de LineData */
    int max_X;           /**< max de l'ensemble des max_X de linedata[] */
    int max_Y;           /**< max de l'ensemble des max_Y de linedata[] */
    int pad[2];          /**< Padding pour definir canvas (cvs) et orig */
    int orig[2];         /**< Coordonnees de l'origine des plots */    
    int margin[2];       /**< Marge droite et haut (evite de toucher les bords)*/    
    int color_bg[3];     /**< Couleur du fond de la figure */
    int color_cvs_bg[3]; /**< Couleur du fond du canvas */
    int color_axes[3];   /**< Couleur des axes*/
} Figure;


/* --------------------------------------------------------------------------- */
// Declaration fonctions
/* --------------------------------------------------------------------------- */

/**
 * @brief Transforme les degrés en radians.
 * 
 * @param angle Angle en degrés
 * @return float Angle en radians
 */
float deg2rad(float angle);

int maxval_array(const int x_array[], size_t n);

int getCouleur(gdImagePtr im_fig, const int couleur[3]);

void save_to_png(gdImagePtr im_fig, const char *dir_figures,\
                const char *filename_fig);

void Make_background(Figure *fig,\
                const int couleur_bg[3], const int couleur_canvas_bg[3]);

void Make_support_axes(Figure *fig, const int couleur[3]);

void ImageLineEpaisseur(gdImagePtr im_fig, \
                const int x1, const int y1, const int x2, const int y2,\
                    LineStyle *linestyle);

void PlotPoint(gdImagePtr im_fig, const int x1, const int y1,\
                LineStyle *linestyle);
                    
void PlotLine(Figure *fig, LineData *linedata);

void Transform_dataX_to_plot(Figure *fig, size_t len_pts,\
                const int pts[], int* pts_dessin);

void Transform_dataY_to_plot(Figure *fig, size_t len_pts, \
                const int pts[], int* pts_dessin);

int *Transform_data_to_plot(Figure *fig, size_t len_pts,\
                const int pts[], char direction);

void Init_linestyle(LineStyle *linestyle, char style,\
        const int color[3], int width, char marker, int ms);

void Init_linedata(LineData *linedata, int len_data, int ptx[], int pty[],\
        LineStyle *linestyle);

void Change_fig_axes_color(Figure *fig, int color[3]);

void Change_fig_cvs_bg(Figure *fig, int color[3]);

void Change_fig_bg(Figure *fig, int color[3]);

void Init_figure(Figure *fig, int figsize[2], int pad[2], int margin[2]);

void Print_debug_ls(LineStyle *linestyle);

void Print_debug_ld(LineData *linedata);

/*
// void Make_xlabel(gdImagePtr im_fig, int origin_axes[2], int pad[2], \
//                             char* label, int labelSize, \
//                             char *fontpath, int couleur[3], \
//                                 int posX, int posY);

// void Make_ylabel(gdImagePtr im_fig, int origin_axes[2], int pad[2], \
//                             char* label, int labelSize, \
//                             char *fontpath, int couleur[3], \
//                                 int posX, int posY);

/* --------------------------------------------------------------------------- */
// Fonctions
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
int maxval_array(const int x_array[], size_t n)
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
float deg2rad(float angle) 
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
    gdImagePng(fig->img, pngout_fig);

    /* Close the files. */
    fclose(pngout_fig);

    free(fig->linedata);

}


/* --------------------------------------------------------------------------- */
void Make_background(Figure *fig,\
                        const int color_bg[3], const int color_canvas_bg[3])
{
    /* Remplissage du fond */
    gdImageFilledRectangle(fig->img,\
                        0, 0,\
                        fig->img->sx-1, fig->img->sy-1,
                        getCouleur(fig->img, color_bg));

    /* Remplissage du canvas */
    gdImageFilledRectangle(fig->img, fig->pad[0], 0,\
                        fig->img->sx-1, fig->img->sy-1 - fig->pad[1],\
                        getCouleur(fig->img, color_canvas_bg));
}

/* --------------------------------------------------------------------------- */
void Make_support_axes(Figure *fig, const int couleur[3])
{
    // int w_axes = 3;
    LineStyle linestyle_axe;
    Init_linestyle(&linestyle_axe, '-', couleur, 3, ' ', 0);

    // axe vertical
    ImageLineEpaisseur(fig->img, fig->orig[0], fig->orig[1],\
                            fig->orig[0],   0,\
                            &linestyle_axe); 

    // axe horizontal
    ImageLineEpaisseur(fig->img,  fig->orig[0],  fig->orig[1],\
                            fig->img->sx-1,  fig->orig[1],
                            &linestyle_axe); 
}
/* --------------------------------------------------------------------------- */
void ImageLineEpaisseur(gdImagePtr im_fig, \
                        const int x1, const int y1, const int x2, const int y2,\
                            LineStyle *linestyle)
{
                    
    gdImageSetThickness(im_fig, linestyle->w);
    gdImageLine(im_fig, x1,   y1,   x2,   y2,   getCouleur(im_fig, linestyle->color));
    gdImageSetThickness(im_fig, 1);
}

/* --------------------------------------------------------------------------- */
void PlotPoint(gdImagePtr im_fig, const int x1, const int y1,\
                               LineStyle *linestyle)
{
    gdImageFilledEllipse(im_fig, x1, y1,\
                    linestyle->ms, linestyle->ms, getCouleur(im_fig, linestyle->color));
}

/* --------------------------------------------------------------------------- */
void PlotLine(Figure *fig, LineData *linedata)
{
    int *x_plot = Transform_data_to_plot(fig, linedata->len_data, linedata->x, 'x');
    int *y_plot = Transform_data_to_plot(fig, linedata->len_data, linedata->y, 'y');   

    // Prise en compte du decalage de l'origine
    // printf("Orig = %d, %d \n", orig[0], orig[1]);
    for (int i=0; i < linedata->len_data-1; i++) 
    {
        ImageLineEpaisseur(fig->img,\
                x_plot[i]   + fig->orig[0],   y_plot[i] + fig->orig[1],\
                x_plot[i+1] + fig->orig[0], y_plot[i+1] + fig->orig[1],\
                linedata->linestyle);

        PlotPoint(fig->img,\
            x_plot[i] + fig->orig[0], y_plot[i] + fig->orig[1], linedata->linestyle);

        if (i == linedata->len_data-2) {
            PlotPoint(fig->img,\
                            x_plot[i+1] + fig->orig[0],\
                            y_plot[i+1] + fig->orig[1],\
                            linedata->linestyle);
        }
    }

    free(x_plot);
    free(y_plot);
}

/* --------------------------------------------------------------------------- */
void Transform_dataX_to_plot(Figure *fig, size_t len_pts,\
                                    const int pts[], int* pts_dessin)
{
    // Taile de la zone de dessin
    const int w_dessin = fig->img->sx - fig->orig[0] - fig->margin[0] - 1;

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

    const int h_dessin = fig->orig[1] - fig->margin[1];   
    for (int i = 0; i < len_pts; i++)
    {
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
int getCouleur(gdImagePtr im_fig, const int couleur[3])
{
    return gdImageColorAllocate(im_fig, couleur[0], couleur[1], couleur[2]);
}

/* --------------------------------------------------------------------------- */
void Init_figure(Figure *fig, int figsize[2], int pad[2], int margin[2])
{

    /* Initialisation de tous les parametres*/

    fig->img = gdImageCreate(figsize[0],figsize[1]);
    fig->pad[0] = pad[0];
    fig->pad[1] = pad[1];
    fig->orig[0] = fig->pad[0];
    fig->orig[1] = fig->img->sy - fig->pad[1] -1;
    fig->margin[0] = margin[0];
    fig->margin[1] = margin[1];

    int col_fond[3]   = {37, 44, 56};     // bleu-gris
    int col_cvs[3] = {0, 0, 0}; 
    int col_axes[3] = {255, 255, 255}; 

    for (int i = 0; i < 3; i++)
    {
        fig->color_bg[i] = col_fond[i];
        fig->color_cvs_bg[i] = col_cvs[i];        
        fig->color_axes[i] = col_axes[i];
    }
    fig->nb_linedata = 0;
    fig->linedata = NULL;
    
    Make_background(fig, fig->color_bg, fig->color_cvs_bg);
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
void Init_linedata(LineData *linedata, int len_data, int ptx[], int pty[],
                    LineStyle *linestyle)
{
    // int *ptx_plot = Transform_data_to_plot(fig, len_data, ptx, 'x');
    // int *pty_plot = Transform_data_to_plot(fig, len_data, pty, 'y');
    linedata->len_data = len_data;
    linedata->x = ptx;
    linedata->y = pty;
    linedata->max_X = maxval_array(linedata->x, linedata->len_data);
    linedata->max_Y = maxval_array(linedata->y, linedata->len_data);
    linedata->linestyle = linestyle;
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
void Print_debug_ld(LineData *linedata)
{
    printf("*** Debug linedata\n");
    printf("* Len data = %d \n", linedata->len_data);
    printf("* Max X    = %d \n", linedata->max_X);
    printf("* Max Y    = %d \n", linedata->max_Y);
    Print_debug_ls(linedata->linestyle);
    printf("*** End linedata\n\n");
}

int max_int(int x, int y) 
{
    return (x < y) ? y : x ;
}

/* --------------------------------------------------------------------------- */
void Update_fig(Figure *fig, LineData *linedata)
{
    fig->nb_linedata++;
    fig->linedata = (LineData *)realloc(fig->linedata, fig->nb_linedata*sizeof(LineData));
    fig->linedata[fig->nb_linedata] = linedata;
    fig->max_X = max_int(fig->max_X, linedata->max_X);
    fig->max_Y = max_int(fig->max_Y, linedata->max_Y);
}
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
    int pad[2] = {90,70}; /**< pad zone de dessin*/
    int margin[2] = {10,10}; /**< margin zone de dessin*/
    // char* fontLabels = "/usr/share/fonts/dejavu-sans-mono-fonts/DejaVuSansMono.ttf";
    char* fontLabels = "/usr/share/fonts/truetype/lato/Lato-Light.ttf";
    const int labelSize = 18;
    char* errStringFT;
    int coul_trait1[3]  = {51, 160,  44};   //vert_fonce
    int coul_trait2[3] = {253, 191, 111};  //orange_clair
    int coul_trait3[3] = {31, 120, 180};   //bleu_fonce
    int coul_trait4[3] = {255, 127,   0};   //orange_fonce
    int coul_trait5[3] = {202, 178, 214};   //violet_clair
    int coul_trait6[3] = {227,  26,  28};   //rouge_fonce


    Figure fig1;
    Init_figure(&fig1, figsize, pad, margin);

    int ptx[]  = {0, 100, 200, 300, 400};
    int pty[]  = {3,   8,   2,   1,   6};
    int pty2[] = {1,   4,   7,   0,   6};
    int pty3[] = {5,   1,   4,   9,   6};
    int pty4[] = {1,   1,   1,   1,   1};
    int pty5[] = {10,  10,  10,  10,  10};

    size_t len_data = sizeof(ptx)/sizeof(ptx[0]);

    LineStyle linestyle1;
    Init_linestyle(&linestyle1, '-', coul_trait1, w_lines,'o', ms);
    LineData line1;
    Init_linedata(&line1, len_data, ptx, pty, &linestyle1);
    Update_fig(&fig1, &line1);

    LineStyle linestyle2;
    Init_linestyle(&linestyle2, '-', coul_trait6, w_lines,'o', ms);
    LineData line2;
    Init_linedata(&line2, len_data, ptx, pty2, &linestyle2);
    Update_fig(&fig1, &line2);

    LineStyle linestyle3;
    Init_linestyle(&linestyle3, '-', coul_trait3, w_lines,'o', ms);
    LineData line3;
    Init_linedata(&line3, len_data, ptx, pty3, &linestyle3);    
    Update_fig(&fig1, &line3);

    LineStyle linestyle4;
    Init_linestyle(&linestyle4, '-', coul_trait4, w_lines,'o', ms);
    LineData line4;
    Init_linedata(&line4, len_data, ptx, pty4, &linestyle4);    
    Update_fig(&fig1, &line4);

    LineStyle linestyle5;
    Init_linestyle(&linestyle5, '-', coul_trait5, w_lines,'o', ms);
    LineData line5;
    Init_linedata(&line5, len_data, ptx, pty5, &linestyle5);    
    Update_fig(&fig1, &line5);

    // Plot_all_lines(&fig1);
    PlotLine(&fig1, &line1);
    PlotLine(&fig1, &line2);
    PlotLine(&fig1, &line3);
    PlotLine(&fig1, &line4);
    PlotLine(&fig1, &line5);

    /* Sauvegarde du fichier png */
    const char *filename_fig1= "fig1.png";
    Save_to_png(&fig1, dir_figures, filename_fig1);

    /* Destroy the image in memory. */
    gdImageDestroy(fig1.img);

    return 0;
}