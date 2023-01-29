#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

int main()
{
    sqlite3 *db_records;
    int rc = sqlite3_open("./db/test.db", &db_records);

    // Test d'ouverture de la db
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Err: %s\n", sqlite3_errmsg(db_records));
        sqlite3_close(db_records);
        exit(EXIT_FAILURE);
    }
    const char *query = "INSERT INTO Records \
            (DATERECOLTE, DISPONIBLE, OCCUPE, INCONNU, MAINTENANCE, RESERVE, NONIMPLEMENTE, SUPPRIME, ENCOURSMISESERVICE, MISESERVICEPLANIFIEE) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db_records, query, -1, &stmt, NULL))
    {
        printf("Erreur SQL :\n");
        printf("%s : %s\n", sqlite3_errstr(sqlite3_extended_errcode  (db_records)), sqlite3_errmsg(db_records));
        sqlite3_close(db_records);
        exit(EXIT_FAILURE);
    }

    sqlite3_bind_text(stmt, 1, "vide", -1, NULL);
    sqlite3_bind_int(stmt, 2, 0);
    sqlite3_bind_int(stmt, 3, 0);
    sqlite3_bind_int(stmt, 4, 0);
    sqlite3_bind_int(stmt, 5, 0);
    sqlite3_bind_int(stmt, 6, 0);
    sqlite3_bind_int(stmt, 7, 0);
    sqlite3_bind_int(stmt, 8, 0);
    sqlite3_bind_int(stmt, 9, 0);
    sqlite3_bind_int(stmt, 10, 0);

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);
    sqlite3_close(db_records);
}