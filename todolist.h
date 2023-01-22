#ifndef LIST_H_SENTRY
#define LIST_H_SENTRY

#include "task_list.h"
#include "task.h"
#include "project.h"
#include "database.h"

struct todolist {
    struct database db;
    struct task_list tasks;
    struct project *projects;
};

void todolist_init(struct todolist *lst);
void todolist_free(struct todolist *lst);

/*===== tasks functions =====*/
void todolist_add_task(struct task *new_task, struct todolist *lst);
void todolist_set_task(task_id id, struct task *new_task, 
                                                struct todolist *lst);
void todolist_delete_task(task_id id, struct todolist *lst);
void todolist_get_all_tasks(struct task_list *tasks, 
                                                struct todolist *lst);
void todolist_get_today_tasks(struct task_list *tasks, 
                                                struct todolist *lst);
void todolist_get_week_tasks(struct task_list *tasks, 
                                                struct todolist *lst);
void todolist_get_green_tasks(struct task_list *tasks, 
                                                struct todolist *lst);
void todolist_get_done_tasks(struct task_list *tasks, 
                                                struct todolist *lst);

/*===== projects functions =====*/
void todolist_add_project(struct project *new_project, 
                                                struct todolist *lst);
void todolist_set_project(project_id id, struct project *new_project, 
                                                struct todolist *lst);
void todolist_delete_project(project_id id, struct todolist *lst);
struct project *todolist_get_projects(struct todolist *lst);

#endif
