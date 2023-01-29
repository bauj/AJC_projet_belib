#include "libs/parsing_json.h"

// ============================================================================

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

    char *json_content = lecture_contenu_json(json_filename);
    
    // Debut parsing du contenu -----------------------------------------------
    int i;
    int r;
    jsmn_parser p;
    jsmntok_t t[128]; /* We expect no more than 128 tokens */

    jsmn_init(&p);
    //int jsmn_parse(jsmn_parser *parser,               /**< le parser */
    //               const char *js,                    /**< chaine parsee */
    //               const size_t len,                  /**< len(chaine) */
    //               jsmntok_t *tokens,                 /**< les tokens */
    //               const unsigned int num_tokens)     /**< num tokens */

    r = jsmn_parse(&p, json_content, strlen(json_content), t,
                    sizeof(t) / sizeof(t[0]));

    if (r < 0) {
        printf("Failed to parse JSON: %d\n", r);
        return 1;
    }

    /* Assume the top-level element is an object */
    if (r < 1 || t[0].type != JSMN_OBJECT) {
        printf("Erreur parsing json : Object attendu.\n");
        return 1;
    }

    int nb_record = 0;
    // Boucle sur l'ensemble des cles de l'objet racine
    for (i = 1; i < r; i++)
    {
        // printf("Token %d : start : %d, end : %d, size : %d\n", i, t[i].start,t[i].end,t[i].size);
        if (jsoneq(json_content, &t[i], "records") == 0) {
            jsmntok_t *record_array = &t[i + 1];
            if (record_array->type != JSMN_ARRAY) {
                continue; // On s'attend a ce que les records soit un array
            }
            nb_record = record_array->size;
            
            printf("+ Records : \n");
            printf("Nb record : %d \n", nb_record);

            // Creation du tableau de records
            record *records = (record *)malloc(nb_record*sizeof(record));

            for (int j = 0; j < nb_record; j++) {
                // On va chercher les record un a un 
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

    // ------------------------------------------------------------------------

    free(json_content);

    return 0;
}