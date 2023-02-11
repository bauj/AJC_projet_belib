/* ----------------------------------------------------------------------------
*  Bibliotheque de constantes.
*  Utilisee notamment pour les couleurs et polices.
*  
*  Author : Juba Hamma. 2023.
* ---------------------------------------------------------------------------- 
*/

#ifndef consts_H
#define consts_H


#define AJC
// #define QEMU
// #define LENOVO

int white[3]        = {255, 255, 255};
int black[3]        = {  0,   0,   0};
int gris_grid[3]    = { 71,  71,  71};

int vert_fonce[3]   = { 51, 160,  44};
int orange_fonce[3] = {255, 127,   0};
int bleu_fonce[3]   = { 31, 120, 180};
int rouge_fonce[3]  = {227,  26,  28};
int vert_clair[3]   = {178, 223, 138};
int orange_clair[3] = {253, 191, 111};
int bleu_clair[3]   = {166, 206, 227};
int rouge_clair[3]  = {251, 154, 153};
int violet_clair[3] = {202, 178, 214};
int violet_fonce[3] = {152,  78, 163};

/**
 * @brief Liste des couleurs utilisees pour les lines plots
 * 
 */
int color_lines[10][3] = {\
                    { 51, 160,  44},
                    {255, 127,   0},
                    { 31, 120, 180},
                    {227,  26,  28},
                    {152,  78, 163},
                    {178, 223, 138},
                    {253, 191, 111},
                    {166, 206, 227},
                    {251, 154, 153},
                    {202, 178, 214}};

/**
 * @brief Liste des couleurs utilisees pour les bar plots
 * 
 */
int color_ctg[6][3] = {\
            {102,194,165},\
            {252,141, 98},\
            {231,138,195},\
            {166,216, 84},\
            {141,160,203},\
            {255,217, 47}};

/**
 * @brief Labels utilisés pour les légendes des bar plots
 * 
 */
char* labels_ctg[4] = { "Disponible",\
                       "Occupé",
                       "En maintenance",
                       "Inconnu"};

#if defined(AJC) || defined(QEMU)
char *fonts_fig[3] = {\
    "/usr/share/fonts/truetype/lato/Lato-Regular.ttf",\
    "/usr/share/fonts/truetype/lato/Lato-Medium.ttf",\
    "/usr/share/fonts/truetype/lato/Lato-LightItalic.ttf",\
    };
#else
char *fonts_fig[3] = {\
    "/usr/share/fonts/lato/Lato-Regular.ttf",\
    "/usr/share/fonts/lato/Lato-Medium.ttf",\
    "/usr/share/fonts/lato/Lato-LightItalic.ttf",\
    };
#endif


#endif