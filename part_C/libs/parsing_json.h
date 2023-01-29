#ifndef PARSING_JSON_H
#define PARSING_JSON_H

#include "jsmn.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// ----------------------------------------------------------------------------
/**
 * @brief Structure contenant un enregistrement (record) permettant de recuperer les donnees du fichier json
 * 
 */
typedef struct record {
    int nb_bornes; /**< Nombre de bornes*/
    char *statut_pdc; /**< Statut des bornes*/
} record;

// ----------------------------------------------------------------------------
/**
 * @brief Fonction d'affichage du contenu d'un record
 * 
 * @param rec Record à afficher
 */
void print_record(record rec)
{
    printf("Statut           : %s \n", rec.statut_pdc);
    printf("Nombre de bornes : %d \n", rec.nb_bornes);
}

// ----------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------
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
    slice(last_piece, date_recolte, 1, 11); /**< le slice n'ajoute pas de \0 en fin de string*/
}

// ----------------------------------------------------------------------------
/**
 * @brief Lecture du contenu du fichier json passé en argument.
 * 
 * @warning Une allocation memoire de taille (nb_char*sizeof(char)) est faite sur json_content, avec nb_char le nombre de char dans le fichier.
 * @param json_filename Nom du fichier json a lire.
 * @return char* Renvoie la chaine de caracteres contenue dans le fichier.
 */
char *lecture_contenu_json(const char *json_filename) {
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

// ----------------------------------------------------------------------------
/**
 * @brief Verifie que le contenu de la chaine parsée et du token recupéré est le même.
 * Ce qui est vérifié : Type du token, longueur du token, et contenu du token
 * 
 * @param json Chaine de caractère parsée
 * @param tok Token en cours d'analyse
 * @param s Chaine de caractère à lire
 * @return int 0 si OK, -1 si KO
 */
static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

/**
 * @brief Affiche le contenu d'un token (string)
 * 
 * @param json Contenu du fichier json
 * @param tok Token a afficher
 */
void print_token(const char *json, jsmntok_t *tok) {
    printf("%.*s \n", tok->end - tok->start, json + tok->start);
}

/**
 * @brief Recupere le contenu (string) d'un token.
 * 
 * @warning La fonction slice est utilisée, on verifie donc que la chaine finit bien par '\0'.
 * @warning Le pointeur tok_str est alloué dynamiquement et doit donc etre désalloué.
 * @param json Contenu du fichier json
 * @param tok Token dont le contenu est récupéré
 * @return char* String contenue dans le token
 */
char *get_token_string(const char *json, jsmntok_t *tok) {
    int len_str = tok->end - tok->start;
    // Allocation de la chaine renvoyée
    char *tok_str = (char *)malloc((len_str+1) * sizeof(char));

    slice(json, tok_str, tok->start, tok->end);
    if (tok_str[len_str] != '\0') {
        tok_str[len_str] = '\0'; /**< le slice n'ajoute pas de \0 en fin de string*/
    }    

    return tok_str;
}

#endif /* PARSING_JSON_H */
