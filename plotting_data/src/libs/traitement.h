/* ----------------------------------------------------------------------------
*  Bibliotheque composée de quelques fonctions facilitant le traitement des
*  données belib recupérées.
*  
*  Author : Juba Hamma. 2023.
* ---------------------------------------------------------------------------- 
*/

#ifndef TRAITEMENT_H
#define TRAITEMENT_H

#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


// ----------------------------------------------------------------------------
/**
 * @brief Structure contenant une string `datestr` de type date ainsi que sa valeur 
 * `ctime` en temps cpu. 
 * Format type d'une datestr : 2023-02-01T14:00Z
 */
typedef struct Date_s {
    char datestr[20];     /**< Contenu date récupéré de l'api + '\0' */
    struct tm tm;         /**< Struct tm contenant l'ensemble des donnees de la date */
    time_t ctime;         /**< Temps en sec depuis le 01/01/70 00:00 */
} Date;


// ----------------------------------------------------------------------------
/**
 * @brief Structure contenant une Date et permettant de creer le contenu des labels
 * des ticks associés, sur la figure d'évolution temporelle du nombre de bornes
 *  disponibles.
 * 
 */
typedef struct Datetick_s {
    int  ecart_init;   /**< Contient l'écart en secondes a la date initiale */
    char labeldate[30]; /**< string de type : DD/mm/YY + '\0' */
    char labelh[6];    /**< string de type : HH:MM + '\0' */ 
}   Datetick;


// ----------------------------------------------------------------------------
/**
 * @brief Initialise un objet de type Datetick
 * 
 * @param datetick Pointeur vers un objet de type Datetick
 * @param dateobj_i Pointeur vers un objet de type Date dont les données sont utilisées pour créer le Datetick
 * @param dateobj_init Pointeur vers un objet de type Date qui est l'origine des temps
 */
void Init_Datetick(Datetick *datetick, Date *dateobj_i, Date *dateobj_init);

// ----------------------------------------------------------------------------
/**
 * @brief Fonction de debug permettant d'afficher le contenu d'un objet de type Datetick
 * 
 * @param datetick Pointeur vers un objet de type DateTick
 */
void Print_debug_datetick(Datetick *datetick);

// ----------------------------------------------------------------------------
/**
 * @brief Fonction de debug permettant d'afficher le contenu d'un objet de type Date
 * 
 * @param dateobj Pointeur vers objet de type Date
 * @param debugmode Caractère 'y' ou 'n' permettant d'afficher l'ensemble des données ou juste la date au format humain
 */
void Print_debug_date(Date *dateobj, char debugmode);

// ----------------------------------------------------------------------------
/**
 * @brief Initialise l'objet tm (time machine) dans un objet de type Date
 * 
 * @param dateobj Pointeur vers un objet de type Date
 */
void Init_date_tm(Date *dateobj);

// ----------------------------------------------------------------------------
/**
 * @brief Initialise un objet de type Date
 * 
 * @param dateobj Pointeur vers un objet de type Date
 * @param date_input Chaine de caractère de la date dans le format date de récolte
 */
void Init_Date(Date *dateobj, char date_input[20]);


// ----------------------------------------------------------------------------
/**
 * @brief Fonction parsant la chaine de caractère date input au format de la date récoltée pour récupérer l'année, le mois, le jour, l'heure et les minutes
 * 
 * @param year Pointeur vers un entier où est sauvegardé l'année
 * @param mon Pointeur vers un entier  où est sauvegardé le mois
 * @param mday Pointeur vers un entier où est sauvegardé le jour
 * @param hour Pointeur vers un entier où est sauvegardé l'heure
 * @param min Pointeur vers un entier  où est sauvegardé les minutes
 * @param date_input 
 */
void Get_date_element(int *year, int *mon, int *mday, int *hour, int *min,char date_input[20]);


// ----------------------------------------------------------------------------
/**
 * @brief Fonction permettant de slicer une chaine de caractères
 * 
 * @param str_src Chaine de caractères source
 * @param str_dest Chaine de caractères destination
 * @param start Index de début du slicing
 * @param end Index de fin du slicing
 */
void Slice_str(const char *str_src, char *str_dest, int start,int end);

// ----------------------------------------------------------------------------
/**
 * @brief 2eme fonction permettant de slicer une chaine de caractères, ajoutant un caractère nul en fin de chaine pour spécifier la fin de chaine.
 * 
 * @param str Chaine de caractères source
 * @param src_dest Chaine de caractères destination
 * @param start Index de début du slicing
 * @param end Index de fin du slicing
 */
void slice_str(const char *str_src, char *src_dest, size_t start, size_t end);


/* --------------------------------------------------------------------------- */
// Definition des fonctions
/* --------------------------------------------------------------------------- */

// ----------------------------------------------------------------------------
void Slice_str(const char *str_src, char *str_dest, int start, int end)
{
    strncpy(str_dest, str_src + start, end - start + 1);
}

// ----------------------------------------------------------------------------
void Init_Datetick(Datetick *datetick, Date *dateobj_i, Date *dateobj_init)
{
    datetick->ecart_init = (int)difftime(dateobj_i->ctime, dateobj_init->ctime);

    sprintf(datetick->labeldate, "%02i/%02i/%02i", \
                             dateobj_i->tm.tm_mday,\
                             dateobj_i->tm.tm_mon+1,\
                            (dateobj_i->tm.tm_year+1900)%2000);
    sprintf(datetick->labelh, "%02d:%02d", \
                            dateobj_i->tm.tm_hour,\
                            dateobj_i->tm.tm_min);                            
}

// ----------------------------------------------------------------------------
void Get_date_element(int *year, int *mon, int *mday, int *hour, int *min,\
                            char date_input[20])
{
    // Rappel format = "%Y-%m-%dT%H-%MTZD";
    // Ex : 2023-01-23T14-00TZD;

    int size_str_elem;

    // Annee
    size_str_elem = 4;
    char str_year[size_str_elem+1];
    Slice_str(date_input, str_year, 0, 3);
    str_year[size_str_elem] = '\0';
    *year = (int) strtol(str_year, NULL, 10);

    // Mois
    size_str_elem = 2;
    char str_mon[size_str_elem+1];
    Slice_str(date_input, str_mon, 5, 6);
    str_mon[size_str_elem] = '\0';
    *mon = (int) strtol(str_mon, NULL, 10);

    // Jour
    size_str_elem = 2;
    char str_mday[size_str_elem+1];
    Slice_str(date_input, str_mday, 8, 9);
    str_mday[size_str_elem] = '\0';
    *mday = (int) strtol(str_mday, NULL, 10);

    // Heure
    size_str_elem = 2;
    char str_hour[size_str_elem+1];
    Slice_str(date_input, str_hour, 11, 12);
    str_hour[size_str_elem] = '\0';
    *hour = (int) strtol(str_hour, NULL, 10);

    // Min
    size_str_elem = 2;
    char str_min[size_str_elem+1];
    Slice_str(date_input, str_min, 14, 15);
    str_min[size_str_elem] = '\0';
    *min = (int) strtol(str_min, NULL, 10);

}

// ----------------------------------------------------------------------------
void Init_date_tm(Date *dateobj)
{
    int year = 1900, mon = 1, mday = 0, hour = 0, min = 0, sec = 0, isdst = -1;

    Get_date_element(&year, &mon, &mday, &hour, &min, dateobj->datestr);

    dateobj->tm.tm_year = year - 1900;
    dateobj->tm.tm_mon = mon - 1;
    dateobj->tm.tm_mday = mday;
    dateobj->tm.tm_hour = hour;
    dateobj->tm.tm_min = min;
    dateobj->tm.tm_sec = sec;
    dateobj->tm.tm_isdst = isdst;

}

// ----------------------------------------------------------------------------
void Init_Date(Date *dateobj, char date_input[20])
{
    // Recup string de la db
    strcpy(dateobj->datestr, date_input);

    // Calcul temps entier depuis 01/01/70
    Init_date_tm(dateobj);
    dateobj->ctime = mktime(&(dateobj->tm));
}


// ----------------------------------------------------------------------------
void Print_debug_date(Date *dateobj, char debugmode)
{
    if (debugmode == 'y') {
        printf("# -----------------------------------------\n");
        printf("> Date obj debug : \n");
        printf("  datestr : %s\n", dateobj->datestr);
        printf("  year    : %d\n", dateobj->tm.tm_year+1900);
        printf("  mon     : %d\n", dateobj->tm.tm_mon+1);
        printf("  dom     : %d\n", dateobj->tm.tm_mday);
        printf("  hour    : %d\n", dateobj->tm.tm_hour);
        printf("  min     : %d\n", dateobj->tm.tm_min);
        printf("  ctime      : %ld\n", dateobj->ctime);
        printf("# -----------------------------------------\n");
    } else if (debugmode == 'n') {
        printf("%02d/%02d/%02d %02d:%02d\n", dateobj->tm.tm_mday,\
                                 dateobj->tm.tm_mon+1,\
                                 (dateobj->tm.tm_year+1900)%2000,\
                                 dateobj->tm.tm_hour,\
                                 dateobj->tm.tm_min);
    } else {
        printf("Info : provide debugmode ('y' or 'n') for Print_date .");
    }

}

// ----------------------------------------------------------------------------
void Print_debug_datetick(Datetick *datetick)
{
    printf("# -----------------------------------------\n");
    printf("> Date tick debug : \n");
    printf("  labeldate  : %s \n", datetick->labeldate);
    printf("  labelh     : %s \n", datetick->labelh);
    printf("  ecart_time : %d \n", datetick->ecart_init);
    printf("# -----------------------------------------\n");
}

// ----------------------------------------------------------------------------
void slice_str(const char *str_src, char *str_dest, size_t start, size_t end)
{
    size_t j = 0;
    for ( size_t i = start; i <= end; ++i ) {
        buffer[j++] = str[i];
    }
    buffer[j] = 0;
}

#endif  /* TRAITEMENT_H */
