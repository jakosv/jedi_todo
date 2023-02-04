#ifndef TODOLIST_H_SENTRY
#define TODOLIST_H_SENTRY

#include "storage.h"
#include "todolist_view.h"
#include "task_list.h"

struct todolist {
    struct storage storage;
    enum view_state view;
    struct task_list tasks;
    struct project_list projects;
    project_id cur_project;
};

void todolist_init(struct todolist *list);
void todolist_destroy(struct todolist *list);
void todolist_main_loop(struct todolist *list);

#endif
