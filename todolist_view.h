#ifndef TODOLIST_VIEW_H_SENTRY
#define TODOLIST_VIEW_H_SENTRY

#include "task_list.h"

#include <stdio.h>

enum view_state { 
    view_today_tasks, 
    view_all_tasks, 
    view_week_tasks,
    view_completed_tasks,
    view_projects,
    view_project_tasks
};

void show_task_list(const struct task_list *lst, const char *name);

#endif
