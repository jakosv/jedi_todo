#ifndef TASK_H_SENTRY
#define TASK_H_SENTRY

#include "project.h"

#include <time.h>

enum { max_task_name_len = 81 };
enum task_folder { tf_none, tf_today, tf_week };

struct task {
    char name[max_task_name_len];
    char has_project;
    project_id pid;
    time_t creation_time;
    char rep_days;
    short rep_interval;
    enum task_folder folder; 
    char green;
    char done;
};

void task_init(struct task *new_task);
void task_create(const char *name, enum task_folder folder,
                                                struct task *new_task);
long task_days(const struct task *task);
time_t next_repeat(const struct task *task);
char is_task_repeating(const struct task *task);
char is_task_today(const struct task *task);
char is_task_week(const struct task *task);

#endif
