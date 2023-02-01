#ifndef STORAGE_H_SENTRY
#define STORAGE_H_SENTRY

#include "task_list.h"
#include "project_list.h"
#include "task.h"
#include "project.h"
#include "database.h"

struct storage {
    struct database db;
    struct task_list tasks;
    struct project_list projects;
};

void storage_init(struct storage *st);
void storage_free(struct storage *st);

/*===== tasks functions =====*/
void storage_add_task(struct task *new_task, struct storage *st);
void storage_set_task(task_id id, struct task *new_task, 
                                                struct storage *st);
void storage_delete_task(task_id id, struct storage *st);
void storage_get_all_tasks(struct task_list *tasks, 
                                                struct storage *st);
void storage_get_today_tasks(struct task_list *tasks, 
                                                struct storage *st);
void storage_get_week_tasks(struct task_list *tasks, 
                                                struct storage *st);
void storage_get_green_tasks(struct task_list *tasks, 
                                                struct storage *st);
void storage_get_completed_tasks(struct task_list *tasks, 
                                                struct storage *st);

/*===== projects functions =====*/
void storage_add_project(struct project *new_project, 
                                                struct storage *st);
void storage_set_project(project_id id, struct project *new_project, 
                                                struct storage *st);
void storage_delete_project(project_id id, struct storage *st);
void storage_get_all_projects(struct project_list *projects, 
                                                struct storage *st);

#endif
