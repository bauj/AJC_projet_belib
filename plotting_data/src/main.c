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
#include <gdfontt.h>
#include <gdfonts.h>
#include <gdfontmb.h>
#include <gdfontl.h>
#include <gdfontg.h>

#define PI 3.141592
#define DEG2RAD(x) ((x)*PI/180.)

int main(int argc, char* argv[])
{
    // ------------------------------------------------------------------------
    // Gestion bdd SQLite
    // ------------------------------------------------------------------------
    // sqlite3 *db_belib;

    // // Ouverture de la db
    
    // const char path_to_db[] = "../db_sqlite/belib_data.db";

    // int rc = sqlite3_open(path_to_db, &db_belib);

    // // Test d'ouverture de la db
    // if (rc != SQLITE_OK)
    // {
    //     fprintf(stderr, "Err: %s\n", sqlite3_errmsg(db_belib));
    //     sqlite3_close(db_belib);
    //     exit(EXIT_FAILURE);
    // }


    // // Fermeture de la db
    // sqlite3_close(db_belib);


    // ------------------------------------------------------------------------
    // Essais gd
    // ------------------------------------------------------------------------

    /* Declare the image */
    gdImagePtr im;
    /* Declare output files */
    FILE *pngout_evolution;
    FILE *pngout_fig2;

    /* Get font*/
	gdFontPtr fontptrt = gdFontGetTiny();
	gdFontPtr fontptrs = gdFontGetSmall();
	gdFontPtr fontptrmb = gdFontGetMediumBold();
	gdFontPtr fontptrl = gdFontGetLarge();
	gdFontPtr fontptrg = gdFontGetGiant();

    /* Declare color indexes */
    int black;
    int white;
    char dir_figures[] = "./figures/";
    
    int couleur_fond; // 37, 44, 56
    int bleu_clair;   // 166, 206, 227
    int bleu_fonce;   //  31, 120, 180
    int vert_clair;   // 178, 223, 138
    int vert_fonce;   //  51, 160,  44
    int rouge_clair;  // 251, 154, 153
    int rouge_fonce;  // 227,  26,  28
    int orange_clair; // 253, 191, 111
    int orange_fonce; // 255, 127,   0
    int violet_clair; // 202, 178, 214

    /* Allocate the image: 800 pixels across by 600 pixels tall */
    int Nx = 800;
    int Ny = 600;
    im = gdImageCreate(Nx, Ny);

    /* Allocate the color black (red, green and blue all minimum).
        Since this is the first color in a new image, it will
        be the background color. */
    couleur_fond = gdImageColorAllocate(im, 37, 44, 56);

    /* Allocate the color white (red, green and blue all maximum). */
    white        = gdImageColorAllocate(im, 255, 255, 255);
    black        = gdImageColorAllocate(im,   0,   0,   0);
    bleu_clair   = gdImageColorAllocate(im, 166, 206, 227);
    bleu_fonce   = gdImageColorAllocate(im,  31, 120, 180);
    vert_clair   = gdImageColorAllocate(im, 178, 223, 138);
    vert_fonce   = gdImageColorAllocate(im,  51, 160,  44);
    rouge_clair  = gdImageColorAllocate(im, 251, 154, 153);
    rouge_fonce  = gdImageColorAllocate(im, 227,  26,  28);
    orange_clair = gdImageColorAllocate(im, 253, 191, 111);
    orange_fonce = gdImageColorAllocate(im, 255, 127,   0);
    violet_clair = gdImageColorAllocate(im, 202, 178, 214);
    
    /* Tests de quelques traces de ligne*/
    gdImageLine(im,    0,    0, Nx-1, Ny-1, vert_fonce); // diagonale no-se
    gdImageLine(im,    0,    1, Nx-2, Ny-1, vert_fonce); // diagonale no-se
    gdImageLine(im,    0,    2, Nx-3, Ny-1, vert_fonce); // diagonale no-se
    gdImageLine(im,    0,    3, Nx-4, Ny-1, vert_fonce); // diagonale no-se
    gdImageLine(im,    0, Ny-1, Nx-1,    0, vert_clair); // diagonale so-ne
    gdImageLine(im, Nx/2,    0, Nx/2, Ny-1, bleu_fonce); // verticale
    gdImageDashedLine(im, Nx/2+3,    0, Nx/2+3, Ny-1, bleu_fonce); // verticale
    gdImageDashedLine(im, Nx/2-3,    0, Nx/2-3, Ny-1, bleu_fonce); // verticale
    gdImageLine(im,    0, Ny/2, Nx-1, Ny/2, bleu_clair); // horizontale

    /* Test ecriture */
    gdImageStringUp(im, fontptrg, 50, 50, "Salut !!", rouge_fonce); // ecrit verticalement
    gdImageString(im, fontptrl, 80, 80, "SALUT !!", rouge_fonce);
    gdImageString(im, fontptrmb, 110, 110, "SALUT !!", rouge_fonce);
    gdImageString(im, fontptrs, 140, 140, "SALUT !!", rouge_fonce);
    gdImageString(im, fontptrt, 170, 170, "SALUT !!", rouge_fonce);
    // gdImageString(im, fontptrt, 500, 400, "SALUT !!", rouge_fonce);
    int brect[8] = {0};
    char* f1 = "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf";
    char* f2 = "/usr/share/fonts/truetype/lato/Lato-Light.ttf";
    char* f3 = "/usr/share/fonts/truetype/lato/Lato-Thin.ttf";
    char* err;
    err = gdImageStringFT(im, brect, rouge_fonce, f1, 26, 0., 500, 300, "TEEEEST");
    err = gdImageStringFT(im, brect, rouge_fonce, f2, 26, 0., 500, 250, "TEEEEST");
    err = gdImageStringFT(im, brect, rouge_fonce, f3, 26, 0., 500, 200, "TEEEEST");
    err = gdImageStringFT(im, brect, rouge_fonce, f1, 26, 0., 500, 150, "Teeest");
    err = gdImageStringFT(im, brect, rouge_fonce, f2, 26, 0., 500, 100, "Teeest");
    err = gdImageStringFT(im, brect, rouge_fonce, f3, 26, 0., 500, 50, "Teeest");

    for (int i =0; i < 360;i=i+20) {
        err = gdImageStringFT(im, brect, rouge_fonce, f2, 26, DEG2RAD(i), 200, 450, "TEEEEST");
    }

    err = gdImageStringFT(im, brect, rouge_fonce, f2, 16, DEG2RAD(55), 450, 550, "Test test");
    err = gdImageStringFT(im, brect, rouge_fonce, f2, 16, DEG2RAD(55), 500, 550, "Test test");
    err = gdImageStringFT(im, brect, rouge_fonce, f2, 16, DEG2RAD(55), 550, 550, "Test test blabli bla");
    
    int upper_lim = brect[5]-5;
    gdImageDashedLine(im, Nx/2,    upper_lim, Nx-1, upper_lim, orange_fonce); // verticale

    printf("%s \n", err);

/**
 * Function: gdImageStringFT
 *
 * Render an UTF-8 string onto a gd image.
 *
 * Parameters:
 *	im       - The image to draw onto.
 *  brect    - The bounding rectangle as array of 8 integers where each pair
 *             represents the x- and y-coordinate of a point. The points
 *             specify the lower left, lower right, upper right and upper left
 *             corner.
 *	fg       - The font color.
 *	fontlist - The semicolon delimited list of font filenames to look for.
 *	ptsize   - The height of the font in typographical points (pt).
 *	angle    - The angle in radian to rotate the font counter-clockwise.
 *	x        - The x-coordinate of the basepoint (roughly the lower left corner) of the first letter.
 *	y        - The y-coordinate of the basepoint (roughly the lower left corner) of the first letter.
 *	string   - The string to render.
 *
 * Variant:
 *  - <gdImageStringFTEx>
 *
 * See also:
 *  - <gdImageString>
 */
    
    /* FIG GRILLE*/
    /* Declare the image */
    gdImagePtr im2;
    int Nx2 = 700;
    int Ny2 = 500;
    im2 = gdImageCreate(Nx2, Ny2);

    couleur_fond = gdImageColorAllocate(im2, 37, 44, 56);
    violet_clair = gdImageColorAllocate(im2, 202, 178, 214);
    black        = gdImageColorAllocate(im2,   0,   0,   0);
    white        = gdImageColorAllocate(im2,   255,   255,   255);

    /* Test figures*/
    // gdImageRectangle(im2, 2, 2, Nx2-3, Ny2-3, white);
    gdImageRectangle(im2, 3, 3, Nx2-4, Ny2-4, white);
    gdImageRectangle(im2, 4, 4, Nx2-5, Ny2-5, white);
    // gdImageFilledRectangle(im2, 100, 100, 300, 300, violet_clair);

    /*TEST COPIE*/
    // gdImageCopy(im, im2, 100, 100, 0, 0, Nx2-1, Ny2-1);

    /* Open a file for writing. "wb" means "write binary", important
        under MSDOS, harmless under Unix. */
    char filename_figEvolution[] = "test.png";

    char path_outputFigEvolution[40] = {""};
    strcat(path_outputFigEvolution, dir_figures);
    strcat(path_outputFigEvolution, filename_figEvolution);
    pngout_evolution = fopen(path_outputFigEvolution, "wb");

    /* Output the image to the disk file in PNG format. */
    gdImagePng(im, pngout_evolution);

    /* Close the files. */
    fclose(pngout_evolution);

    /* Destroy the image in memory. */
    gdImageDestroy(im);
    


    /* save fig2*/
    char filename_fig2[] = "test2.png";
    char path_outputFig2[40] = {""};
    strcat(path_outputFig2, dir_figures);
    strcat(path_outputFig2, filename_fig2);
    pngout_fig2 = fopen(path_outputFig2, "wb");

    /* Output the image to the disk file in PNG format. */
    gdImagePng(im2, pngout_fig2);

    fclose(pngout_fig2);

    gdImageDestroy(im2);
    

    printf("Hello World !! Let's plot that !!\n");
    return 0;
}