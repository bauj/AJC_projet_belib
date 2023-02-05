/* ----------------------------------------------------------------------------
*  Bibliotheque de constantes.
*  
*  Author : Juba Hamma. 2023.
* ---------------------------------------------------------------------------- 
*/

#ifndef consts_H
#define consts_H

int white[3]        = {255, 255, 255};   //white
int black[3]        = {  0,   0,   0};   //black
int gris_grid[3]    = { 71,  71,  71};   // gris grid
int vert_fonce[3]   = { 51, 160,  44};   //vert_fonce
int vert_clair[3]   = {178, 223, 138};   //vert_clair
int orange_fonce[3] = {255, 127,   0};   //orange_fonce
int orange_clair[3] = {253, 191, 111};   //orange_clair
int bleu_fonce[3]   = { 31, 120, 180};   //bleu_fonce
int bleu_clair[3]   = {166, 206, 227};   //bleu_clair
int rouge_fonce[3]  = {227,  26,  28};   //rouge_fonce
int rouge_clair[3]  = {251, 154, 153};   //rouge_clair
int violet_clair[3] = {202, 178, 214};   //violet_clair

int color_lines[9][3] = {\
                    {253, 191, 111},\
                    { 51, 160,  44},\
                    {178, 223, 138},\
                    {255, 127,   0},\
                    { 31, 120, 180},\
                    {166, 206, 227},\
                    {227,  26,  28},\
                    {251, 154, 153},\
                    {202, 178, 214},\
                    };

#endif