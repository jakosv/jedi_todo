#ifndef TASK_H_SENTRY
#define TASK_H_SENTRY

#include "project.h"

typedef unsigned task_id;

enum { max_task_name_len = 101 };
enum task_folder { tf_none, tf_today, tf_week };

struct task {
    task_id id;
    char name[max_task_name_len];
    char has_project;
    project_id pid;
    unsigned date;
    char repeat_days[7];
    enum task_folder folder; 
    char green;
};

#endif
