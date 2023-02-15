#ifndef DATABASE_H_SENTRY
#define DATABASE_H_SENTRY

#include "stdio.h"
#include "record.h"

enum { max_path_len = 101 };

struct database {
    FILE *file;
    char path[max_path_len];
    unsigned records_count;
    unsigned deleted_count;
};

void db_create(const char *path, struct database *db);
int db_open(const char *path, struct database *db);
void db_close(struct database *db);

void db_import_data(const char *path, struct database *db);
int db_export_data(const char *path, struct database *db);

unsigned db_count_records(struct database *db);
record_pos db_add_record(struct record *new_record, struct database *db); 
void db_update_record(record_pos pos, struct record *updated_record, 
                                                    struct database *db); 
void db_delete_record(record_pos pos, struct database *db); 
void db_fetch_record(record_pos pos, struct record *rec, 
                                                    struct database *db); 
int db_fetch_all_records(struct record **records, unsigned size, 
                                              const struct database *db); 

#endif
