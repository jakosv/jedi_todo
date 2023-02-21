#ifndef TASK_H_SENTRY
#define TASK_H_SENTRY

#include "project.h"

#include <time.h>

enum { 
    max_task_name_len = 128,
    max_task_descript_len = 256
};
enum task_folder { tf_none, tf_today, tf_week };

struct task {
    char name[max_task_name_len];
    char description[max_task_descript_len];
    char has_project;
    project_id pid;
    time_t creation_time;
    char rep_days;
    short rep_interval;
    time_t repeat_date;
    enum task_folder folder; 
    char green;
    char done;
};

void task_init(struct task *new_task);
void task_create(const char *name, enum task_folder folder,
                                                struct task *new_task);
long sec_to_days(time_t seconds);
time_t days_to_sec(long days);
int is_task_repeating(const struct task *task);
int is_task_completed(const struct task *task);
long task_days(const struct task *task);
time_t get_next_repeat(const struct task *task);
int is_task_today(const struct task *task);
int is_task_week(const struct task *task);
int is_task_in_project(project_id pid, const struct task *task);
int is_project_task_completed(project_id pid, const struct task *task);
void task_complete_repeating(struct task *task);
void task_unrepeat(struct task *task);
void task_add_repeat_interval(int interval, int start_in, 
                                                    struct task *task);
void task_update_repeat_days(char new_day, struct task *task);

#endif
