/* ----------------------------------------------------------------------------
*  Programme permettant de parser un fichier json issu d'une requete SQL 
*  permettant de récupérer les open data Belib' disponibles sur le site 
*  OpenData Paris. Une fois parsé, le contenu du fichier json est injecté
*  dans une base de données sqlite, en vu d'un traitement futur sur python.  
* ---------------------------------------------------------------------------- 
*/

// ----------------------------------------------------------------------------
// License
// ----------------------------------------------------------------------------
/*
 * MIT License
 *
 * Copyright (c) 2023 Juba Hamma
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// ----------------------------------------------------------------------------
#include "libs/parsing_json.h"
#include "libs/funcs_sqlite3.h"

int main(int arg, char *argv[])
{

    // ------------------------------------------------------------------------
    // Lecture et recuperation du fichier json
    // ------------------------------------------------------------------------

    // Recuperation du nom du fichier json en argument
    char *json_filename = argv[1];

    // Test de presence d'un argument
    if (json_filename == NULL)
    {
        printf("Erreur : argument non spécifié. Le programme attend le nom d'un fichier en entrée. \n");
        exit(EXIT_FAILURE);
    }

    // Recuperation de la date de recolte a partir du nom du fichier
    char date_recolte[20] = ""; /**< le format de la date est YYYY-MM-DDTHH-mmTZD, i.e. 19 char + \0 */
    recup_date(json_filename, date_recolte);

    char *json_content = lecture_contenu_json(json_filename);

    // ------------------------------------------------------------------------
    // Parsing du contenu du fichier json
    // ------------------------------------------------------------------------

    int i;            /**< Index utilisé à partir de la racine du json */
    int r;            /**< Nombre de tokens dans le contenu json parsé */
    jsmn_parser p;    /**< Parser de la library JSMN*/
    jsmntok_t t[128]; /**< Nombre de tokens limité 128 */

    jsmn_init(&p);
    // int jsmn_parse(jsmn_parser *parser,               /**< le parser */
    //                const char *js,                    /**< chaine parsee */
    //                const size_t len,                  /**< len(chaine) */
    //                jsmntok_t *tokens,                 /**< les tokens */
    //                const unsigned int num_tokens)     /**< num tokens */

    // Parsing du contenu json
    r = jsmn_parse(&p, json_content, strlen(json_content), t,
                   sizeof(t) / sizeof(t[0]));

    // Test du parsing via JSMN
    if (r < 0)
    {
        printf("Erreur : Parsing du JSON impossible.: %d\n", r);
        exit(EXIT_FAILURE);
    }

    // Premier objet lu doit etre de type JSMN_OBJECT {}
    if (r < 1 || t[0].type != JSMN_OBJECT)
    {
        printf("Erreur parsing json : Object attendu.\n");
        exit(EXIT_FAILURE);
    }

    // Creation du tableau de records (type record)
    record *records = (record *)malloc(sizeof(record));
    int nb_record = 0; /**< Stockage du nombre de records*/
    // Boucle sur l'ensemble des cles de l'objet racine
    for (i = 1; i < r; i++)
    {
        // Recherche des "records"
        if (jsoneq(json_content, &t[i], "records") == 0)
        {
            jsmntok_t *record_array = &t[i + 1];
            // Le mot-clé "records" doit etre du type JSMN_ARRAY []
            if (record_array->type != JSMN_ARRAY)
            {
                continue;
            }
            nb_record = record_array->size;

            // Creation du tableau de records (type record)
            records = (record *)realloc(records, nb_record * sizeof(record));

            // Parsing des records
            for (int j = 0; j < nb_record; j++)
            {
                // Chaque record est de type JSMN_OBJECT {}
                jsmntok_t *record_obj = record_array + 5 + j * 9;
                int len_str=0;
                if (record_obj->type != JSMN_OBJECT)
                {
                    continue; // On veut que chaque record soit un object
                }    
                
                // Essayer de mettre tout ca dans une fonction !
                len_str = (record_obj + 2)->end - (record_obj + 2)->start;

                char *tok_content_statut_pdc = \
                        (char *)malloc((len_str+1) * sizeof(char));
                
                slice(json_content, tok_content_statut_pdc, \
                             (record_obj + 2)->start, (record_obj + 2)->end);
                tok_content_statut_pdc[len_str] = '\0';

                len_str = (record_obj + 4)->end - (record_obj + 4)->start;
                char *tok_content_nb_bornes = \
                        (char *)malloc((len_str+1) * sizeof(char));
                slice(json_content, tok_content_nb_bornes, \
                             (record_obj + 4)->start, (record_obj + 4)->end);
                tok_content_nb_bornes[len_str] = '\0';

                /*
                get_token_string(json_content,record_obj + 2, \
                                        &tok_content_statut_pdc);

                get_token_string(json_content, record_obj + 4, \
                                        &tok_content_nb_bornes);
                */

                // Copie du contenu des token dans la struct record
                strcpy(records[j].statut_pdc,tok_content_statut_pdc);
                records[j].nb_bornes = atoi(tok_content_nb_bornes); // atoi pour transformer str -> int;

                free(tok_content_nb_bornes);
                free(tok_content_statut_pdc);
            }
        }
    }

    free(json_content);

    // ------------------------------------------------------------------------
    // Gestion bdd SQLite
    // ------------------------------------------------------------------------
    sqlite3 *db_records;

    // Ouverture de la db
    //int rc = sqlite3_open("./db/records_belib.db", &db_records);
    int rc = sqlite3_open("./sqlitedb/records_belib.db", &db_records);

    // Test d'ouverture de la db
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Err: %s\n", sqlite3_errmsg(db_records));
        sqlite3_close(db_records);
        exit(EXIT_FAILURE);
    }

    // Insertion du tableau de records dans la db
    insert_records(date_recolte, nb_record, records, db_records);

    // Fermeture de la db
    sqlite3_close(db_records);

    // 29/01/23 : Temps d'execution pour l'ensemble du programme : 0m0,035s
    
    free(records);

    return 0;
}
