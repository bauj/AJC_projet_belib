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


/**
 * @brief Transforme les degrés en radians.
 * 
 * @param angle Angle en degrés
 * @return float Angle en radians
 */
float deg2rad(float angle) {
    return angle*PI/180;
}

int main(int argc, char* argv[]) 
{
    
    return 0;
}