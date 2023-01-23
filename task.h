#ifndef TASK_H_SENTRY
#define TASK_H_SENTRY

#include "project.h"

enum { max_task_name_len = 101 };
enum task_folder { tf_none, tf_today, tf_week };

struct task {
    char name[max_task_name_len];
    char has_project;
    project_id pid;
    unsigned date;
    char repeat_days[7];
    enum task_folder folder; 
    char green;
    char done;
};

void task_create(const char *name, enum task_folder folder,
                                                struct task *new_task);

#endif
