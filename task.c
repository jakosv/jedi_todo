#include "task.h"

#include <string.h>
#include <time.h>

void task_init(struct task *new_task)
{
    memset(new_task->name, 0, max_task_name_len); 
    memset(new_task->description, 0, max_task_descript_len);
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

long sec_to_days(time_t seconds)
{
    return seconds / (60 * 60 * 24);
}

time_t days_to_sec(long days)
{
    return days * 24 * 60 * 60;
}

long task_days(const struct task *task)
{
    time_t today;
    long days;
    today = time(NULL);
    days = sec_to_days(today) - sec_to_days(task->creation_time);
    if (task->rep_interval)
        days -= task->rep_interval;
    return days;
}

char is_task_repeating(const struct task *task)
{
    return task->rep_days || task->rep_interval;
}

time_t task_repeat_date(const struct task *task)
{
    if (task->rep_days)
        return task->creation_time;
    else if (task->rep_interval)
        return task->creation_time + days_to_sec(task->rep_interval);
    return task->creation_time;
}

time_t get_next_repeat(const struct task *task)
{
    if (task->rep_days) {
        char day, c_wday, days_diff;
        time_t date;
        date = task->creation_time + days_to_sec(1);
        c_wday = localtime(&date)->tm_wday;
        if (task->rep_days & (1 << c_wday))
            return date;
        days_diff = 1;
        for (day = c_wday % 7 + 1; day != c_wday; day = day % 7 + 1) {
            if (task->rep_days & (1 << day))
                return date + days_to_sec(days_diff);
            days_diff++;
        }
    } else if (task->rep_interval) {
        return task->creation_time + days_to_sec(task->rep_interval);
    }
    return 0;
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
    if (task->rep_days)
        return task->creation_time <= now;
    else if (task->rep_interval)
        return (task->creation_time + days_to_sec(task->rep_interval)) <= now;
        /*
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
        return days_diff >= task->rep_interval;
    }
    */
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
        return creation_day + task->rep_interval <= last_week_day;
    }
    return 0;
}

void task_unrepeat(struct task *task)
{
    task->rep_days = 0;
    task->rep_interval = 0;
    task->creation_time = time(NULL);
}

static char is_wday_in_interval(char wday, char l_bound, char r_bound)
{
    return (wday >= l_bound && wday <= r_bound) ||
           (wday >= l_bound && r_bound < l_bound && wday >= r_bound) ||
           (wday <= l_bound && wday <= r_bound);
}

void task_update_days_repeat(char new_day, struct task *task)
{
    char prev_day, today, nday, days_diff;
    time_t date, now;
    now = time(NULL);
    today = localtime(&now)->tm_wday;
    date = task->creation_time;
    prev_day = localtime(&date)->tm_wday;
    nday = new_day;
    if (task->rep_days & (1 << new_day)) {
        /* 
        - find closest day and update creation_time
        for (day = (new_day + 1) % 7; day != new_day % 7; day = day % 7 + 1)
           if (task->rep_days & (1 << day))
               return 1;
       */
    } else {
        nday = new_day % 7;
        days_diff = (nday - today + 7) % 7;
        date = now + days_to_sec(days_diff);
        if (!task->rep_days || 
            (is_wday_in_interval(nday, today, prev_day) &&
                task->creation_time > date))
        {
            task->creation_time = date;
        }
    }
    task->rep_days ^= 1 << new_day;
}
