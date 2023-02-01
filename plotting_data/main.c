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

int main(int argc, char* argv[])
{
    // ------------------------------------------------------------------------
    // Gestion bdd SQLite
    // ------------------------------------------------------------------------
    sqlite3 *db_belib;

    // Ouverture de la db
    
    const char path_to_db[] = "../db_sqlite/belib_data.db";

    int rc = sqlite3_open(path_to_db, &db_belib);

    // Test d'ouverture de la db
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Err: %s\n", sqlite3_errmsg(db_belib));
        sqlite3_close(db_belib);
        exit(EXIT_FAILURE);
    }


    // Fermeture de la db
    sqlite3_close(db_belib);


    // ------------------------------------------------------------------------
    // Essais gd
    // ------------------------------------------------------------------------

    /* Declare the image */
    gdImagePtr im;
    /* Declare output files */
    FILE *pngout_evolution;

    /* Declare color indexes */
    int black;
    int white;
    char dir_figures[] = "./figures/";

    /* Allocate the image: 800 pixels across by 600 pixels tall */
    im = gdImageCreate(800, 600);

    /* Allocate the color black (red, green and blue all minimum).
        Since this is the first color in a new image, it will
        be the background color. */
    black = gdImageColorAllocate(im, 0, 0, 0);

    /* Allocate the color white (red, green and blue all maximum). */
    white = gdImageColorAllocate(im, 255, 255, 255);

    /* Draw a line from the upper left to the lower right,
        using white color index. */
    gdImageLine(im, 0, 0, 799, 599, white);

    /* Open a file for writing. "wb" means "write binary", important
        under MSDOS, harmless under Unix. */
    char filename_figEvolution[] = "test.png";
    
    printf("%ld\n", strlen(strcat(dir_figures, filename_figEvolution)));

    pngout_evolution = fopen(strcat(dir_figures, filename_figEvolution), "wb");

    /* Output the image to the disk file in PNG format. */
    gdImagePng(im, pngout_evolution);

    /* Close the files. */
    fclose(pngout_evolution);

    /* Destroy the image in memory. */
    gdImageDestroy(im);
    
    printf("Hello World !! Let's plot that !!\n");
    return 0;
}