#include "storage.h"
#include "database.h"
#include "task_list.h"
#include "record.h"
#include "task.h"
#include "project.h"

#include <stdio.h>
#include <stdlib.h>

static const char *database_path = "todolist.db";

static void fetch_database_records(struct storage *st)
{
    struct record *records;
    unsigned records_cnt, i;
    records_cnt = db_count_records(&st->db); 
    records = malloc(records_cnt * sizeof(struct record));
    db_fetch_all_records(&records, records_cnt, &st->db);
    tl_init(&st->tasks);
    for (i = 0; i < records_cnt; i++) {
        switch (records[i].type) {
        case rt_task:
            tl_add(i, &records[i].data.task, &st->tasks);
            break;
        case rt_project:
            break;
        }
    }
    free(records);
}

void storage_open(struct storage *st)
{
    int ok;
    if (!st) {
        fprintf(stderr, "lodolist_init(): passed NULL");
        exit(1);
    }
    ok = db_open(database_path, &st->db);
    if (!ok)
        db_create(database_path, &st->db);
    fetch_database_records(st);
}

void storage_close(struct storage *st)
{
    if (!st) {
        fprintf(stderr, "lodolist_destroy(): passed NULL");
        exit(1);
    }
    db_close(&st->db);
    tl_clear(&st->tasks);
}


/*===== tasks functions =====*/

void storage_add_task(struct task *new_task, struct storage *st)
{
    struct record rec;
    record_pos new_task_id;
    if (!st) {
        fprintf(stderr, "lodolist_add_task(): passed NULL");
        exit(1);
    }
    rec.type = rt_task;
    rec.data.task = *new_task;
    rec.is_deleted = 0;
    new_task_id = db_add_record(&rec, &st->db);
    tl_add(new_task_id, new_task, &st->tasks);
}

void storage_set_task(task_id id, struct task *new_task, 
                                                    struct storage *st)
{
    struct record rec;
    struct tl_item *task_item;
    if (!st) {
        fprintf(stderr, "lodolist_set_task(): passed NULL");
        exit(1);
    }
    task_item = tl_get_item(id, &st->tasks);
    task_item->data = *new_task;
    rec.data.task = *new_task;
    rec.is_deleted = 0;
    db_update_record(task_item->id, &rec, &st->db);
}

void storage_delete_task(task_id id, struct storage *st)
{
    struct tl_item *tmp; 
    if (!st) {
        fprintf(stderr, "lodolist_delete_task(): passed NULL");
        exit(1);
    }
    tmp = tl_get_item(id, &st->tasks); 
    db_delete_record(tmp->id, &st->db);
    tl_remove(tmp, &st->tasks); 
}

void storage_get_all_tasks(struct task_list *tasks, struct storage *st)
{
    struct tl_item *tmp;
    task_id pos;
    for (tmp = st->tasks.first, pos = 0; tmp; tmp = tmp->next, pos++)
        tl_add(pos, &tmp->data, tasks);
}

void storage_get_today_tasks(struct task_list *tasks, struct storage *st)
{
    struct tl_item *tmp;
    task_id pos;
    for (tmp = st->tasks.first, pos = 0; tmp; tmp = tmp->next, pos++)
        if (tmp->data.folder == tf_today)
            tl_add(pos, &tmp->data, tasks);
}

void storage_get_week_tasks(struct task_list *tasks, struct storage *st)
{
    struct tl_item *tmp;
    task_id pos;
    for (tmp = st->tasks.first, pos = 0; tmp; tmp = tmp->next, pos++)
        if (tmp->data.folder == tf_week || tmp->data.folder == tf_today)
            tl_add(pos, &tmp->data, tasks);
}

void storage_get_green_tasks(struct task_list *tasks, struct storage *st)
{

}

void storage_get_done_tasks(struct task_list *tasks, struct storage *st)
{

}


/*===== projects functions =====*/

void storage_add_project(struct project *new_project, 
                                                    struct storage *st)
{
    if (!st) {
        fprintf(stderr, "lodolist_add_project(): passed NULL");
        exit(1);
    }
}

void storage_set_project(project_id id, struct project *new_project, 
                                                    struct storage *st)
{
    if (!st) {
        fprintf(stderr, "lodolist_set_project(): passed NULL");
        exit(1);
    }
}

void storage_delete_project(project_id id, struct storage *st)
{
    if (!st) {
        fprintf(stderr, "lodolist_delete_project(): passed NULL");
        exit(1);
    }
}

struct project *storage_get_projects(struct storage *st)
{
    if (!st) {
        fprintf(stderr, "lodolist_get_projects(): passed NULL");
        exit(1);
    }
    return NULL;
}
