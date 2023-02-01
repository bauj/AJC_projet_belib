/* ----------------------------------------------------------------------------
*  Programme permettant d'ouvrir la bdd SQLite contenant les donnees belib et
*  de plot les infos interessantes.
*  
*  Author : Juba Hamma. 2023.
* ---------------------------------------------------------------------------- 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <gd.h>

#define PI 3.141592

float deg2rad(float angle);
void save_to_png(gdImagePtr im_fig, const char *dir_figures,\
                                            const char *filename_fig);
void make_background(gdImagePtr im_fig, const int padX, const int padY,\
                             const int couleur[3]);
void make_support_axes(gdImagePtr im_fig, const int orig[2], const int couleur[3]);
void ImageLineEpaisseur(gdImagePtr im_fig, \
            const int x1, const int y1, const int x2, const int y2,\
             const int couleur[3], const int w);
void PlotPoint(gdImagePtr im_fig, const int x1, const int y1, const int couleur[3], const int ms);
void PlotLine(gdImagePtr im_fig, const int nb_pts, \
                const int ptx[], const int pty[], const int couleur[3], const int w, const int ms);

/**
 * @brief Transforme les degrés en radians.
 * 
 * @param angle Angle en degrés
 * @return float Angle en radians
 */
float deg2rad(float angle) 
{
    return angle*PI/180;
}


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

void make_background(gdImagePtr im_fig, const int padX, const int padY,\
                             const int couleur[3])
{
    /* Fond bleu gris */
    gdImageColorAllocate(im_fig, couleur[0], couleur[1], couleur[2]);

    int black = gdImageColorAllocate(im_fig, 0, 0, 0);

    /* Fond noir a l'interieur : rectangle defini par 2 pts*/
    gdImageFilledRectangle(im_fig, padX, 0, im_fig->sx-1, im_fig->sy-1 - padY, black);
}

void make_support_axes(gdImagePtr im_fig, const int orig[2], const int couleur[3])
{
    int w = 3;
    ImageLineEpaisseur(im_fig, orig[0],   orig[1], orig[0],   0, couleur, w); // axe vertical
    ImageLineEpaisseur(im_fig, orig[0], orig[1],   im_fig->sx-1, orig[1],   couleur, w); // axe horizontal
}

void ImageLineEpaisseur(gdImagePtr im_fig, \
            const int x1, const int y1, const int x2, const int y2,\
             const int couleur[3], const int w)
{

    int couleur_trait = gdImageColorAllocate(im_fig,  couleur[0], couleur[1],  couleur[2]);
    gdImageSetThickness(im_fig, w);
    gdImageLine(im_fig, x1,   y1,   x2,   y2,   couleur_trait);

    gdImageSetThickness(im_fig, 1);
}

void PlotPoint(gdImagePtr im_fig, const int x1, const int y1, const int couleur[3], const int ms)
{
    int coul = gdImageColorAllocate(im_fig,  couleur[0], couleur[1],  couleur[2]);
    gdImageFilledEllipse(im_fig, x1, y1, ms, ms, coul);
}

void PlotLine(gdImagePtr im_fig, const int nb_pts, \
                const int ptx[], const int pty[], const int couleur[3], const int w, const int ms)
{
    for (int i=0; i < nb_pts-1; i++) 
    {
        ImageLineEpaisseur(im_fig, ptx[i], pty[i], ptx[i+1], pty[i+1], couleur, w); // diagonale no-se
            PlotPoint(im_fig, ptx[i], pty[i], couleur, ms);
        if (i == nb_pts-2) {
            PlotPoint(im_fig, ptx[i+1], pty[i+1], couleur, ms);
        }
    }
}
/* ============================================================================ */
int main(int argc, char* argv[]) 
{
    // data = get_data_from_db(db_path);

    // Parametres generaux
    const char *dir_figures= "./figures/";
    const int w_lines = 3;
    const int ms = 8;
    const int padX = 90, padY = 70; /**< pad zone de dessin*/
   
    /* Couleurs dans la figure*/
    const int couleur_fond[3] = {37, 44, 56};      // bleu-gris
    const int couleur_axes[3] = {255, 255, 255};   // white
    const int couleur_trait[3] = {51, 160,  44};   //vert_fonce
    const int couleur_trait2[3] = {253, 191, 111}; //orange_clair
    const int couleur_trait3[3] = {31, 120, 180}; //bleu_fonce

    gdImagePtr im_fig1; /**< Image figure evolution stations en favoris*/
    int w = 800, h = 600;  
    im_fig1 = gdImageCreate(w, h);
    printf("Figsize : %dpx x %dpx \n", im_fig1->sx, im_fig1->sy);

    /* Creation du fond et de la zone de dessin a fond noir*/
    make_background(im_fig1, padX, padY, couleur_fond);

    /* Zero des axes*/
    const int origin_axes[2] = {padX, im_fig1->sy - padY};


    /* Quelques donnees*/
    const int ptx[] = {94,120,450,500,550,600,650,700,750,795};

    const int pty[] = {300,350,300,320,320,360,365,345,305,305};
    const int pty2[] = {300-80,350-145,300-80,320-80,320-80,360-80,365-80,345-80,305-80,305-80};
    const int pty3[] = {300-140,350-140,300-140,320-140,320-140,360-140,365-140,345-140,305,305-140};

    /* Recupere nombre de points*/
    size_t lenArr = sizeof(ptx)/sizeof(ptx[0]);

    /* Plot de quelques tracés*/
    PlotLine(im_fig1, lenArr, ptx, pty, couleur_trait, w_lines, ms);
    PlotLine(im_fig1, lenArr, ptx, pty2, couleur_trait2, w_lines, ms);
    PlotLine(im_fig1, lenArr, ptx, pty3, couleur_trait3, w_lines, ms);

    /* Ecriture xlabel */

    /* Ecriture ylabel */

    /* Dessiner les ticks */

    /* Valeurs des ticks  */

    /* Creation des axes au bord de la zone de dessin*/
    make_support_axes(im_fig1, origin_axes, couleur_axes);

    /* Sauvegarde du fichier png */
    const char *filename_fig1= "fig1.png";
    save_to_png(im_fig1, dir_figures, filename_fig1);

    /* Destroy the image in memory. */
    gdImageDestroy(im_fig1);

    return 0;
}