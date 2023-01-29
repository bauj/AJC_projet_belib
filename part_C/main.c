#include "libs/parsing_json.h"
#include <sqlite3.h>

// ============================================================================

int main(int arg, char* argv[])
{

    // ------------------------------------------------------------------------
    // Lecture et recuperation du fichier json
    // ------------------------------------------------------------------------

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

    char *json_content = lecture_contenu_json(json_filename);

    // ------------------------------------------------------------------------
    // Parsing du contenu du fichier json
    // ------------------------------------------------------------------------

    int i;  /**< Index utilisé à partir de la racine du json */
    int r;  /**< Nombre de tokens dans le contenu json parsé */
    jsmn_parser p;  /**< Parser de la library JSMN*/
    jsmntok_t t[128]; /**< Nombre de tokens limité 128 */

    jsmn_init(&p);
    //int jsmn_parse(jsmn_parser *parser,               /**< le parser */
    //               const char *js,                    /**< chaine parsee */
    //               const size_t len,                  /**< len(chaine) */
    //               jsmntok_t *tokens,                 /**< les tokens */
    //               const unsigned int num_tokens)     /**< num tokens */

    // Parsing du contenu json
    r = jsmn_parse(&p, json_content, strlen(json_content), t,
                    sizeof(t) / sizeof(t[0]));

    // Test du parsing via JSMN
    if (r < 0) {
        printf("Erreur : Parsing du JSON impossible.: %d\n", r);
        exit(EXIT_FAILURE);
    }

    // Premier objet lu doit etre de type JSMN_OBJECT {}
    if (r < 1 || t[0].type != JSMN_OBJECT) {
        printf("Erreur parsing json : Object attendu.\n");
        exit(EXIT_FAILURE);
    }

    int nb_record = 0; /**< Stockage du nombre de records*/
    // Boucle sur l'ensemble des cles de l'objet racine
    for (i = 1; i < r; i++)
    {
        // Recherche des "records"
        if (jsoneq(json_content, &t[i], "records") == 0) {
            jsmntok_t *record_array = &t[i + 1];
            // Le mot-clé "records" doit etre du type JSMN_ARRAY []
            if (record_array->type != JSMN_ARRAY) {
                continue;
            }
            nb_record = record_array->size;
            
            printf("+ Records : \n");
            printf("Nb record : %d \n", nb_record);

            // Creation du tableau de records (type record)
            record *records = (record *)malloc(nb_record*sizeof(record));

            // Parsing des records
            for (int j = 0; j < nb_record; j++) {
                // Chaque record est de type JSMN_OBJECT {}
                jsmntok_t *record_obj = record_array+5+j*9;
                if (record_obj->type != JSMN_OBJECT) {
                    continue; // On veut que chaque record soit un object
                }
                char *tok_content_statut_pdc = get_token_string(json_content, \
                                                         record_obj+2);    
                char *tok_content_nb_bornes = get_token_string(json_content, \
                                                         record_obj+4);
                
                records[j].statut_pdc = tok_content_statut_pdc;
                records[j].nb_bornes = atoi(tok_content_nb_bornes); //atoi pour transformer str -> int;
                
                print_record(records[j]);

            }
        }
    }

    free(json_content);

    // ------------------------------------------------------------------------
    // Gestion bdd SQLite
    // ------------------------------------------------------------------------


    return 0;
}