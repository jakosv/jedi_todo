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

static char next_repeat_day(time_t date, char rep_days)
{
    char c_wday, day;
    c_wday = localtime(&date)->tm_wday;
    if (rep_days & (1 << c_wday))
        return c_wday;
    for (day = (c_wday + 1) % 7; day != c_wday; day = (day + 1) % 7)
        if (rep_days & (1 << day))
            return day;
    return -1;
}

time_t get_next_repeat(const struct task *task)
{
    if (task->rep_days) {
        char day, c_wday, days_diff;
        time_t date;
        date = task->creation_time + days_to_sec(1);
        c_wday = localtime(&date)->tm_wday;
        day = next_repeat_day(date, task->rep_days);
        days_diff = (day - c_wday + 7) % 7;
        return date + days_to_sec(days_diff); 
    } else if (task->rep_interval) {
        return task->creation_time + days_to_sec(task->rep_interval);
    }
    return 0;
}

char is_task_today(const struct task *task)
{
    time_t now;
    long today_date, task_date;
    if (task->folder == tf_today)
        return 1;
    if (is_task_repeating(task)) {
        now = time(NULL);
        today_date = sec_to_days(now);
        task_date = sec_to_days(task_repeat_date(task));
        return task_date <= today_date;
    }
    return 0;
}

char is_task_week(const struct task *task)
{
    if (task->folder == tf_week || is_task_today(task)) {
        return 1;
    } else if (is_task_repeating(task)) {
        time_t now;
        char today_wday;
        long today_date, sunday_date, task_date;
        now = time(NULL);
        today_wday = localtime(&now)->tm_wday; 
        today_date = sec_to_days(now);
        if (today_wday == 0)
            today_wday = 7;
        sunday_date = today_date + (7 - today_wday);
        task_date = sec_to_days(task_repeat_date(task));
        return task_date <= sunday_date;
    }
    return 0;
}

void task_unrepeat(struct task *task)
{
    task->rep_days = 0;
    task->rep_interval = 0;
    task->creation_time = time(NULL);
}

void task_update_days_repeat(char new_day, struct task *task)
{
    time_t date, now;
    char days_diff;
    now = time(NULL);
    date = task->creation_time;
    task->rep_days ^= 1 << new_day;
    if (task->rep_days & (1 << new_day)) {
        long prev_date, new_date, today_date;
        char today_wday;
        today_wday = localtime(&now)->tm_wday;
        days_diff = (new_day - today_wday + 7) % 7;
        prev_date = sec_to_days(date);
        today_date = sec_to_days(now);
        new_date = today_date + days_diff;
        if (!(task->rep_days ^ (1 << new_day)) || 
            (new_date < prev_date))
        {
            task->creation_time = days_to_sec(new_date);
        }
    } else {
        char nday, prev_day;
        prev_day = localtime(&date)->tm_wday;
        nday = next_repeat_day(date, task->rep_days);
        days_diff = (nday - prev_day + 7) % 7;
        date += days_to_sec(days_diff);
        task->creation_time = date;
    }
}
