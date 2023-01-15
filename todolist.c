#include "todolist.h"
#include "database.h"
#include "task_list.h"
#include "record.h"
#include "task.h"
#include "project.h"

#include <stdio.h>
#include <stdlib.h>

static const char *database_path = "todolist.db";

static void fetch_database_records(struct todolist *lst)
{
    struct record *records;
    unsigned records_cnt, i;
    records_cnt = db_count_records(&lst->db); 
    records = malloc(records_cnt * sizeof(struct record));
    db_fetch_all_records(&records, records_cnt, &lst->db);
    tl_init(&lst->tasks);
    for (i = 0; i < records_cnt; i++) {
        switch (records[i].type) {
        case rt_task:
            records[i].data.task.id = records[i].pos;
            tl_add(&records[i].data.task, &lst->tasks);
            break;
        case rt_project:
            break;
        }
    }
    free(records);
}

void todolist_init(struct todolist *lst)
{
    int ok;
    if (!lst) {
        fprintf(stderr, "lodolist_init(): passed NULL");
        exit(1);
    }
    ok = db_open(database_path, &lst->db);
    if (!ok)
        db_create(database_path, &lst->db);
    fetch_database_records(lst);
}

void todolist_destroy(struct todolist *lst)
{
    if (!lst) {
        fprintf(stderr, "lodolist_destroy(): passed NULL");
        exit(1);
    }
    db_close(&lst->db);
    tl_destroy(&lst->tasks);
}


void todolist_add_task(struct task *new_task, struct todolist *lst)
{
    struct record rec;
    if (!lst) {
        fprintf(stderr, "lodolist_add_task(): passed NULL");
        exit(1);
    }
    rec.type = rt_task;
    task_copy(&rec.data.task, new_task);
    rec.is_deleted = 0;
    db_add_record(&rec, &lst->db);
    new_task->id = rec.pos;
    tl_add(new_task, &lst->tasks);
}

void todolist_set_task(task_id id, struct task *new_task, 
                                                    struct todolist *lst)
{
    struct record rec;
    struct task *old_task;
    if (!lst) {
        fprintf(stderr, "lodolist_set_task(): passed NULL");
        exit(1);
    }
    rec.pos = id;
    old_task = tl_get_by_id(id, &lst->tasks);
    task_copy(old_task, new_task);
    task_copy(&rec.data.task, new_task);
    rec.is_deleted = 0;
    db_update_record(&rec, &lst->db);
}

void todolist_delete_task(task_id id, struct todolist *lst)
{
    if (!lst) {
        fprintf(stderr, "lodolist_delete_task(): passed NULL");
        exit(1);
    }
    tl_remove_by_id(id, &lst->tasks); 
    db_delete_record(id, &lst->db);
}

struct task *todolist_get_tasks(struct todolist *lst)
{
    if (!lst) {
        fprintf(stderr, "lodolist_get_task(): passed NULL");
        exit(1);
    }

    return NULL;
}


void todolist_add_project(struct project *new_project, 
                                                    struct todolist *lst)
{
    if (!lst) {
        fprintf(stderr, "lodolist_add_project(): passed NULL");
        exit(1);
    }

}

void todolist_set_project(project_id id, struct project *new_project, 
                                                    struct todolist *lst)
{
    if (!lst) {
        fprintf(stderr, "lodolist_set_project(): passed NULL");
        exit(1);
    }

}

void todolist_delete_project(project_id id, struct todolist *lst)
{
    if (!lst) {
        fprintf(stderr, "lodolist_delete_project(): passed NULL");
        exit(1);
    }
}

struct project *todolist_get_projects(struct todolist *lst)
{
    if (!lst) {
        fprintf(stderr, "lodolist_get_projects(): passed NULL");
        exit(1);
    }
    return NULL;
}

