#ifndef TODOLIST_VIEW_H_SENTRY
#define TODOLIST_VIEW_H_SENTRY

#include "task_list.h"
#include "project_list.h"
#include "todolist_command.h"

#include <stdio.h>

enum { list_view_start_pos = 1 };

enum view_state { 
    view_today_tasks, 
    view_all_tasks, 
    view_week_tasks,
    view_completed_tasks,
    view_projects,
    view_project_tasks,
    view_project_completed_tasks
};


void show_today_tasks(const struct task_list *lst);
void show_week_tasks(const struct task_list *lst);
void show_all_tasks(const struct task_list *lst);
void show_completed_tasks(const struct task_list *lst);
void show_project_tasks(const struct task_list *lst, const char *proj_name);
void show_project_completed_tasks(const struct task_list *lst, 
                                                    const char *proj_name);
void show_projects(const struct project_list *lst);
void show_task_info(const struct task *task);
void show_project_info(const struct project *proj);
void show_help();
void show_command_prompt();
void show_message(const char *msg);
void show_error(const char *msg);
void show_command_info(enum commands cmd, const char *info);
void show_unknown_command_message(const char *cmd);
void show_list_only_command_error(enum commands cmd);
void show_pos_range_error(const char *pos);

#endif
