#include "database.h"
#include <stdlib.h>
#include <string.h>

static void count_file_records(unsigned *records_count, 
                                    unsigned *deleted_count, FILE *file)
{
    enum { buf_size = 4096 / sizeof(struct record) };
    struct record buf[buf_size];
    int n, i;
    *records_count = 0;
    *deleted_count = 0;
    fseek(file, 0, SEEK_SET);
    while (!feof(file)) {
        n = fread(buf, sizeof(struct record), buf_size, file);
        if (ferror(file)) {
            perror("count_file_records()");
            exit(1);
        }
        *records_count += n;
        for (i = 0; i < n; i++)
            if (buf[i].is_deleted)
                (*deleted_count)++;
    }
}

void db_create(const char *path, struct database *db)
{
    db->file = fopen(path, "w+b"); 
    if (!db->file) {
        perror(path);
        exit(1);
    }
    strlcpy(db->path, path, max_path_len);
    db->records_count = 0;
    db->deleted_count = 0;
}

int db_open(const char *path, struct database *db)
{
    db->file = fopen(path, "r+b"); 
    if (!db->file) {
        return 0;
    }
    strlcpy(db->path, path, max_path_len);
    count_file_records(&db->records_count, &db->deleted_count, db->file);
    return 1;
}

static void write_records_array(struct record *records, unsigned num, 
                                                                FILE* file)
{
    long bytes_count;
    bytes_count = num * sizeof(struct record);
    fwrite(records, 1, bytes_count, file); 
    if (ferror(file)) {
        perror("write_records_array()");
        exit(1);
    }
}

int db_import_data(const char *path, struct database *db)
{
    struct database import_db;
    struct record *records;
    unsigned records_count;
    int res;
    res = db_open(path, &import_db); 
    if (!res)
        return res;
    records_count = import_db.records_count - import_db.deleted_count;
    records = malloc(records_count * sizeof(struct record));
    db_fetch_all_records(&records, records_count, &import_db);
    if (db->file)
        fclose(db->file);
    db->file = fopen(db->path, "w+b"); 
    write_records_array(records, records_count, db->file);
    count_file_records(&db->records_count, &db->deleted_count, db->file);
    free(records);
    db_close(&import_db);
    return 1;
}

int db_export_data(const char *path, struct database *db)
{
    struct record *records;
    unsigned records_count;
    FILE *f;
    f = fopen(path, "wb");
    if (!f) {
        return 0;    
    } 
    records_count = db->records_count - db->deleted_count;
    records = malloc(records_count * sizeof(struct record));
    db_fetch_all_records(&records, records_count, db);
    write_records_array(records, records_count, f);
    free(records);
    return 1;
}

void db_close(struct database *db)
{
    if (db->deleted_count > 0) {
        struct record *records;
        unsigned records_count;
        records_count = db->records_count - db->deleted_count;
        records = malloc(records_count * sizeof(struct record));
        db_fetch_all_records(&records, records_count, db);
        fclose(db->file);
        db->file = fopen(db->path, "wb"); 
        write_records_array(records, records_count, db->file);
        free(records);
    } 
    fclose(db->file);
    db->records_count = 0;
    db->deleted_count = 0;
}

static void write_record(record_pos pos, struct record *new_record, 
                                                                FILE *file)
{
    long bytes_offset;
    bytes_offset = pos * sizeof(struct record);
    fseek(file, bytes_offset, SEEK_SET);
    fwrite(new_record, 1, sizeof(struct record), file);
    if (ferror(file)) {
        perror("write_record()");
        exit(1);
    }
    fflush(file);
}

static void read_record(record_pos pos, struct record *rec, FILE *file)
{
    long bytes_offset;
    bytes_offset = pos * sizeof(struct record);
    fseek(file, bytes_offset, SEEK_SET);
    fread(rec, 1, sizeof(struct record), file);
    if (ferror(file)) {
        perror("read_record()");
        exit(1);
    }
}

unsigned db_count_records(struct database *db)
{
    return (db->records_count - db->deleted_count);
}

record_pos db_add_record(struct record *new_record, struct database *db)
{
    record_pos pos;
    if (!db) {
        fprintf(stderr, "add_record(): passed NULL database");
        exit(1);
    }
    pos = db->records_count;
    write_record(pos, new_record, db->file);
    db->records_count++;
    return pos;
}

void db_update_record(record_pos pos, struct record *updated_record, 
                                                    struct database *db)
{
    if (!db) {
        fprintf(stderr, "update_record(): passed NULL database");
        exit(1);
    }
    write_record(pos, updated_record, db->file);
}

void db_fetch_record(record_pos pos, struct record *rec, struct database *db)
{
    if (!db) {
        fprintf(stderr, "fetch_record(): passed NULL database");
        exit(1);
    }
    read_record(pos, rec, db->file); 
}

void db_delete_record(record_pos pos, struct database *db)
{
    struct record *rec;
    if (!db) {
        fprintf(stderr, "delete_record(): passed NULL database");
        exit(1);
    }
    rec = malloc(sizeof(struct record));
    read_record(pos, rec, db->file); 
    if (rec->is_deleted)
        return;
    rec->is_deleted = 1;
    write_record(pos, rec, db->file); 
    free(rec);
    db->deleted_count++;
}

int db_fetch_all_records(struct record **records, unsigned size, 
                                                const struct database *db)
{
    struct record *all_records;
    long bytes_count;
    record_pos i, num;
    if (!db) {
        fprintf(stderr, "delete_record(): passed NULL database");
        exit(1);
    }
    bytes_count = db->records_count * sizeof(struct record);
    all_records = malloc(bytes_count);
    fseek(db->file, 0, SEEK_SET);
    fread(all_records, 1, bytes_count, db->file);
    if (ferror(db->file)) {
        perror("fetch_all_records()");
        exit(1);
    }
    for (i = 0, num = 0; i < db->records_count; i++) {
        if (!all_records[i].is_deleted) {
            (*records)[num] = all_records[i];
            num++;
            if (num >= size)
                break;
        }
    }
    free(all_records);
    return num;
}
