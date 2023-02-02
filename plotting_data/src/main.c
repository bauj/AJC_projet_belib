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

struct LineStyle_s {
    char style;

} LineStyle_default = {'-'};

typedef struct LineStyle_s LineStyle;

typedef struct Figure {
    gdImage *img; /**< Pointeur sur objet gdImage*/


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

void save_to_png(gdImagePtr im_fig, const char *dir_figures,\
                            const char *filename_fig);

void make_background(gdImagePtr im_fig, const int padX, const int padY,\
                            const int couleur[3]);

void make_support_axes(gdImagePtr im_fig, \
                            const int orig[2], const int couleur[3]);

void ImageLineEpaisseur(gdImagePtr im_fig, \
                            const int x1, const int y1, \
                                const int x2, const int y2,\
                                    const int couleur[3], const int w);

void PlotPoint(gdImagePtr im_fig, const int x1, const int y1,\
                    const int couleur[3], const int ms);
                    
void PlotLine(gdImagePtr im_fig,\
                     const int orig[2], const int margin[2],\
                        const int nb_pts, const int ptx[],\
                            const int pty[], const int couleur[3],\
                                const int w, const int ms);

void Transform_dataX_to_plot(gdImagePtr im_fig, const int orig[2],\
                                 const int margin[2], size_t len_pts,\
                                    const int pts[], int* pts_dessin);

void Transform_dataY_to_plot(gdImagePtr im_fig, const int orig[2],\
                                 const int margin[2], size_t len_pts, \
                                    const int pts[], int* pts_dessin);

int *Transform_data_to_plot(gdImagePtr im_fig, const int orig[2],\
                                 const int margin[2], size_t len_pts,\
                                  const int pts[], char direction);



void Make_xlabel(gdImagePtr im_fig, int origin_axes[2], int pad[2], \
                            char* label, int labelSize, \
                            char *fontpath, int couleur[3], \
                                int posX, int posY);

void Make_ylabel(gdImagePtr im_fig, int origin_axes[2], int pad[2], \
                            char* label, int labelSize, \
                            char *fontpath, int couleur[3], \
                                int posX, int posY);

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
void save_to_png(gdImagePtr im_fig, const char *dir_figures,\
                                            const char *filename_fig)
{
    FILE *pngout_fig;

    char path_outputFig[40] = {""};
    strcat(path_outputFig, dir_figures);
    strcat(path_outputFig, filename_fig);
    pngout_fig = fopen(path_outputFig, "wb");

    /* Output the image to the disk file in PNG format. */
    gdImagePng(im_fig, pngout_fig);

    /* Close the files. */
    fclose(pngout_fig);
}


/* --------------------------------------------------------------------------- */
void make_background(gdImagePtr im_fig, const int padX, const int padY,\
                             const int couleur[3])
{
    /* Fond bleu gris */
    gdImageColorAllocate(im_fig, couleur[0], couleur[1], couleur[2]);

    int black = gdImageColorAllocate(im_fig, 0, 0, 0);

    /* Fond noir a l'interieur : rectangle defini par 2 pts*/
    gdImageFilledRectangle(im_fig, padX, 0,\
                                   im_fig->sx-1, im_fig->sy-1 - padY, black);
}

/* --------------------------------------------------------------------------- */
void make_support_axes(gdImagePtr im_fig,\
                            const int orig[2], const int couleur[3])
{
    int w = 3;
    // axe vertical
    ImageLineEpaisseur(im_fig, orig[0],   orig[1], orig[0],   0, couleur, w); 

    // axe horizontal
    ImageLineEpaisseur(im_fig, orig[0], orig[1],
                               im_fig->sx-1, orig[1],   couleur, w); 
}
/* --------------------------------------------------------------------------- */
void ImageLineEpaisseur(gdImagePtr im_fig, \
                const int x1, const int y1, const int x2, const int y2,\
                    const int couleur[3], const int w)
{
    int couleur_trait = gdImageColorAllocate(im_fig,\
                              couleur[0], couleur[1],  couleur[2]);

    gdImageSetThickness(im_fig, w);
    gdImageLine(im_fig, x1,   y1,   x2,   y2,   couleur_trait);
    gdImageSetThickness(im_fig, 1);
}

/* --------------------------------------------------------------------------- */
void PlotPoint(gdImagePtr im_fig, const int x1, const int y1,\
                               const int couleur[3], const int ms)
{
    int coul = gdImageColorAllocate(im_fig,\
                    couleur[0], couleur[1],  couleur[2]);
    gdImageFilledEllipse(im_fig, x1, y1, ms, ms, coul);
}

/* --------------------------------------------------------------------------- */
void PlotLine(gdImagePtr im_fig, const int orig[2], const int margin[2],\
                                    const int nb_pts, const int ptx[],\
                                        const int pty[], const int couleur[3],\
                                            const int w, const int ms)
{

    int *ptx_plot = Transform_data_to_plot(im_fig,\
                        orig, margin, nb_pts, ptx, 'x');
    int *pty_plot = Transform_data_to_plot(im_fig,\
                        orig, margin, nb_pts, pty, 'y');   

    // Prise en compte du decalage de l'origine
    // printf("Orig = %d, %d \n", orig[0], orig[1]);
    for (int i=0; i < nb_pts-1; i++) 
    {
        ImageLineEpaisseur(im_fig, ptx_plot[i]  +orig[0],   pty_plot[i]+orig[1],\
                                   ptx_plot[i+1]+orig[0], pty_plot[i+1]+orig[1],\
                                   couleur, w);
        PlotPoint(im_fig, ptx_plot[i]+orig[0], pty_plot[i]+orig[1], couleur, ms);
        if (i == nb_pts-2) {
            PlotPoint(im_fig, ptx_plot[i+1]+orig[0], pty_plot[i+1]+orig[1],\
                         couleur, ms);
        }
    }

    free(ptx_plot);
    free(pty_plot);
}

/* --------------------------------------------------------------------------- */
void Transform_dataX_to_plot(gdImagePtr im_fig, const int orig[2],\
                                 const int margin[2], size_t len_pts,\
                                    const int pts[], int* pts_dessin)
{
    // Taile de la zone de dessin
    const int w_dessin = im_fig->sx - orig[0] - margin[0] - 1;
    const int max_pts = maxval_array(pts, len_pts);

    for (int i = 0; i < len_pts; i++)
    {
        pts_dessin[i] = (pts[i] * w_dessin) / max_pts;
        // printf("Point : %d, %d \n", i, pts_dessin[i]);
    }

}


/* --------------------------------------------------------------------------- */
void Transform_dataY_to_plot(gdImagePtr im_fig, const int orig[2],\
                                 const int margin[2], size_t len_pts, \
                                    const int pts[], int* pts_dessin)
{
    // Taile de la zone de dessin

    const int h_dessin = orig[1] - margin[1];
    const int max_pts = maxval_array(pts, len_pts);
   
    for (int i = 0; i < len_pts; i++)
    {
        pts_dessin[i] = -(pts[i] * h_dessin) / max_pts;
        // printf("Point : %d, %d \n", i, pts_dessin[i]);
    }

}

/* --------------------------------------------------------------------------- */
int *Transform_data_to_plot(gdImagePtr im_fig, const int orig[2],\
                                 const int margin[2], size_t len_pts,\
                                  const int pts[], char direction)
{
    int *pts_dessin = malloc(len_pts * sizeof(int));

    if (pts_dessin == NULL) {
        printf("Erreur : Pas assez de memoire.\n");
    }    

    // Taile de la zone de dessin
    if (direction == 'x') {
        Transform_dataX_to_plot(im_fig, orig,\
                                 margin,len_pts, pts, pts_dessin);
    } else if (direction == 'y')
    {
        Transform_dataY_to_plot(im_fig, orig,\
                                 margin,len_pts, pts, pts_dessin);
    }
    else 
    {
        printf("Erreur : Direction de tracé inconnue.\n");
        free(pts_dessin);
        exit(EXIT_FAILURE);
    }
    
    return pts_dessin;
}

void Make_xlabel(gdImagePtr im_fig, int origin_axes[2], int pad[2], \
                            char* label, int labelSize, \
                            char *fontpath, int couleur[3], \
                                int posX, int posY);

void Make_ylabel(gdImagePtr im_fig, int origin_axes[2], int pad[2], \
                            char* label, int labelSize, \
                            char *fontpath, int couleur[3], \
                                int posX, int posY);

/* --------------------------------------------------------------------------- */
int getCouleur(gdImagePtr im_fig, int couleur[3])
{
    return gdImageColorAllocate(im_fig, couleur[0], couleur[1], couleur[2]);
}

/* =========================================================================== */
int main(int argc, char* argv[]) 
{
    // data = get_data_from_db(db_path);

    // Parametres generaux
    const char *dir_figures= "./figures/";
    const int w_lines = 3;
    const int ms = 8;
    const int pad[2] = {90,70}; /**< pad zone de dessin*/
    const int margin[2] = {10,10}; /**< margin zone de dessin*/
    // char* fontLabels = "/usr/share/fonts/dejavu-sans-mono-fonts/DejaVuSansMono.ttf";
    char* fontLabels = "/usr/share/fonts/truetype/lato/Lato-Light.ttf";
    const int labelSize = 18;
    char* errStringFT;

    /* Couleurs dans la figure*/
    const int coul_fond[3]   = {37, 44, 56};     // bleu-gris
    const int coul_axes[3]   = {255, 255, 255};  // white
    const int coul_trait[3]  = {51, 160,  44};   //vert_fonce
    const int coul_trait2[3] = {253, 191, 111};  //orange_clair
    const int coul_trait3[3] = {31, 120, 180};   //bleu_fonce
    const int coul_trait4[3] = {227,  26,  28};  //rouge_fonce

    gdImagePtr im_fig1; /**< Image figure evolution stations en favoris*/
    int w = 800, h = 600;  
    im_fig1 = gdImageCreate(w, h);
    printf("Figsize : %dpx x %dpx \n", im_fig1->sx, im_fig1->sy);

    /* Creation du fond et de la zone de dessin a fond noir*/
    make_background(im_fig1, pad[0], pad[1], coul_fond);

    /* Zero des axes*/
    const int origin_axes[2] = {pad[0], im_fig1->sy-1-pad[1]};

    /* Quelques donnees*/
    // Limites zone de dessin :
    // Gauche : padX      (orig[0])
    // Droite : Nx-1 - margX
    // Bas    : Ny-padY-1 (orig[1])
    // Haut   : 0 + margY
    
    const int ptx[] = {0, 100, 200, 300, 400};
    const int pty[] = {3, 8, 2, 1, 6};
    const int pty2[] = {1, 4, 7, 3, 6};
    const int pty3[] = {5, 1, 4, 9, 6};

    /* Recupere nombre de points*/
    size_t len_data = sizeof(ptx)/sizeof(ptx[0]);

    /* Plot de quelques tracés*/
    PlotLine(im_fig1, origin_axes, margin,\
                len_data, ptx,  pty,  coul_trait, w_lines, ms);
    PlotLine(im_fig1, origin_axes, margin,\
                len_data, ptx, pty2, coul_trait2, w_lines, ms);
    PlotLine(im_fig1, origin_axes, margin,\
                len_data, ptx, pty3, coul_trait3, w_lines, ms);

    /* Ecriture xlabel */


    int brect[8] = {0};
    char *xlabel = "Date";
    int posX_xlabel = origin_axes[0]+ (w-pad[0])/2;
    int posY_xlabel = origin_axes[1]+(pad[1]/2) + pad[1]/3;
    printf("Pos xlabel : %d, %d \n", posX_xlabel, posY_xlabel);

    errStringFT = gdImageStringFT(im_fig1, brect,\
                        getCouleur(im_fig1, coul_axes), fontLabels,\
                            labelSize, 0., posX_xlabel, posY_xlabel, xlabel);
    
    printf("%s \b", errStringFT);

    // /* Ecriture ylabel */
    // char *ylabel = "Nb bornes";
    // int posX_ylabel = pad[0]/2;
    // int posY_ylabel = origin_axes[1]/2;    
    // errStringFT = gdImageStringFT(im_fig1, brect, coul_axes, fontLabels,\
    //                  26, 90., posX_ylabel, posY_ylabel, xlabel);

    // printf("%s \b", errStringFT);

    /* Dessiner les ticks */

    /* Valeurs des ticks  */

    /* Creation des axes au bord de la zone de dessin*/
    make_support_axes(im_fig1, origin_axes, coul_axes);

    /* Sauvegarde du fichier png */
    const char *filename_fig1= "fig1.png";
    save_to_png(im_fig1, dir_figures, filename_fig1);

    /* Destroy the image in memory. */
    gdImageDestroy(im_fig1);

    return 0;
}