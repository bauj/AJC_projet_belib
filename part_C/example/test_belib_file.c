#include "jsmn.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Structure contenant un enregistrement (record) permettant de recuperer les donnees du fichier json
 * 
 */
typedef struct record {
    char *statut_pdc; /**< Statut des borne*/
    int nb_bornes; /**< Nombre de bornes*/
} record;


/**
 * @brief Fonction permettant de slicer une chaine de caractères
 * 
 * @param str_src Chaine de caractère SOURCE
 * @param str_dest Chaine de caractère DESTINATION
 * @param start Indice de départ pour le slicing
 * @param end Indice de fin pour le slicing
 */
void slice(const char *str_src, char *str_dest, int start, int end)
{
    strncpy(str_dest, str_src + start, end - start);
}

/**
 * @brief Recuperation de la date de collecte a partir du nom du fichier
 * Un nom de fichier finit par la date du jour de collecte :
 * XX_XX_XX_2023-01-28.json
 * 
 * @param json_filename Nom du fichier json entré en argument du programme
 * @param date_recolte Chaine de caractères contenant la date
 * @return void* 
 */
void recup_date(const char *json_filename, char* date_recolte) {
    // On recupere la fin du nom de fichier. Permet d'avoir un basename différent.
    char *last_piece = strrchr(json_filename, '_');
    slice(last_piece, date_recolte, 1, 11);
}

/**
 * @brief Lecture du contenu du fichier json passé en argument.
 * 
 * @warning Une allocation memoire de taille (nb_char*sizeof(char)) est faite sur json_content, avec nb_char le nombre de char dans le fichier.
 * @param json_filename Nom du fichier json a lire.
 * @return char* Renvoie la chaine de caracteres contenue dans le fichier.
 */
char *lecture_contenu_json(char *json_filename) {
    // Ouverture du fichier json
    FILE *json_file = fopen(json_filename, "r");
    
    // Test d'ouverture du fichier
    if (json_file == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier. Vérifiez si le fichier existe.\n");
        exit(EXIT_FAILURE);
    }

    // Comptage du nombre de caracteres dans le fichier
    int nb_char = 1; /**< Initialisation en comptant le \0*/
    while (fgetc(json_file) != EOF) {
        nb_char++;
    }
    rewind(json_file);

    // // Lecture du contenu du fichier json dans le tableau json-content
    char *json_content = (char *)malloc(nb_char * sizeof(char)); /**< Initialisation de la taille du tableau a partir du compteur nb_char*/

    // // Initialisation à 0 d'un objet de taille variable avec memset
    memset(json_content, 0, nb_char);
    
    // // Recuperation du contenu du fichier
    fgets(json_content,nb_char,json_file);

    // // Fermeture du fichier
    fclose(json_file);

    // Test contenu du fichier
    if (json_content[0] == '\0') {
        printf("Erreur : Le contenu du fichier json `%s` est vide.\n", json_filename);
        exit(EXIT_FAILURE);
    }

    return json_content;
}

int main(int arg, char* argv[])
{
    // Recuperation du nom du fichier en argument
    char *json_filename = argv[1];
    
    // Test de presence d'un argument
    if (json_filename == NULL) {
        printf("Erreur : argument non spécifié. Le programme attend le nom d'un fichier en entrée. \n");
        exit(EXIT_FAILURE);
    }

    // Recuperation de la date de recolte a partir du nom du fichier
    char date_recolte[11] = ""; /**< le format de la date est YYYY-MM-DD, i.e. 10 char + \0 */
    recup_date(json_filename, date_recolte);
    
    // printf("> Nom du fichier json en argument : %s \n", json_filename);
    // printf("> Date de recolte lue : %s \n", date_recolte);

    char *json_content = lecture_contenu_json(json_filename);
    
    printf("> Contenu du json :\n %s \n", json_content);

    // Debut parsing du contenu

    free(json_content);

    return 0;
}