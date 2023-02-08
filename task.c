#include "task.h"

#include <string.h>
#include <time.h>

void task_init(struct task *new_task)
{
    memset(new_task->name, 0, max_task_name_len); 
    new_task->has_project = 0;
    new_task->pid = 0;
    new_task->creation_time = time(NULL);
    new_task->rep_days = 0;
    new_task->rep_interval = 0;
    new_task->folder = tf_none;
    new_task->green = 0;
    new_task->done = 0;
}

void task_create(const char *name, enum task_folder folder,
                                                struct task *new_task)
{
    task_init(new_task);
    new_task->folder = folder;
    strlcpy(new_task->name, name, max_task_name_len); 
}

static long sec_to_days(time_t seconds)
{
    return seconds / (60 * 60 * 24);
}

static time_t days_to_sec(long days)
{
    return days * 24 * 60 * 60;
}

long task_days(const struct task *task)
{
    time_t today;
    today = time(NULL);
    return sec_to_days(today) - sec_to_days(task->creation_time);
}

time_t next_repeat(const struct task *task)
{
    if (task->rep_days) {
        char day, c_wday, days_diff;
        c_wday = localtime(&task->creation_time)->tm_wday;
        if (task->rep_days & (1 << c_wday))
            return task->creation_time;
        days_diff = 1;
        for (day = c_wday % 7 + 1; day != c_wday; day = day % 7 + 1) {
            if (task->rep_days & (1 << day))
                return task->creation_time + days_to_sec(days_diff);
            days_diff++;
        }
    } else if (task->rep_interval) {
        return task->creation_time + days_to_sec(task->rep_interval - 1);
    }
    return 0;
}

char is_task_repeating(const struct task *task)
{
    return task->rep_days || task->rep_interval;
}

char is_task_today(const struct task *task)
{
    enum { days_in_week = 7 };
    char today, creation_day;
    time_t now, time_diff;
    long days_diff;
    if (task->folder == tf_today)
        return 1;
    now = time(NULL);
    today = localtime(&now)->tm_wday;
    creation_day = localtime(&task->creation_time)->tm_wday;
    time_diff = difftime(now, task->creation_time);
    days_diff = sec_to_days(now) - sec_to_days(task->creation_time);
    if (task->rep_days) {
        char day;
        if (time_diff >= days_to_sec(days_in_week))
            return 1;
        if (task->rep_days & (1 << today))
            return 1;
        for (day = creation_day; day != today; day = day % 7 + 1)
           if (task->rep_days & (1 << day))
               return 1;
    } else if (task->rep_interval) {
        return days_diff >= task->rep_interval - 1;
    }
    return 0;
}

char is_task_week(const struct task *task)
{
    time_t now;
    if (task->folder == tf_week || is_task_today(task)) {
        return 1;
    } else if (task->rep_days) {
        char today, day;
        now = time(NULL);
        today = localtime(&now)->tm_wday;
        for (day = today; day <= 7; day++)
            if (task->rep_days & (1 << day))
                return 1;
    } else if (task->rep_interval) {
        long today, creation_day, last_week_day;
        char week_day;
        now = time(NULL);
        today = sec_to_days(now);
        week_day = localtime(&now)->tm_wday;
        last_week_day = today + (7 - week_day);
        creation_day = sec_to_days(task->creation_time);
        return creation_day + task->rep_interval - 1 <= last_week_day;
    }
    return 0;
}
