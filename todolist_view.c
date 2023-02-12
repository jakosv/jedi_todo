#include "todolist_view.h"
#include "todolist_command.h"
#include "task_list.h"
#include "project_list.h"
#include "task.h"
#include "project.h"

#include <string.h>
#include <time.h>

#define COLOR_GREEN     "\x1B[32m"
#define COLOR_CYAN      "\x1B[36m"
#define COLOR_MAGENTA   "\x1B[35m"
#define COLOR_RESET     "\x1B[0m"

enum {
    title_size = 25,
    decoration = '=',
    date_str_size = 10
};

static const char *today_task_indicator = "Today";
static const char *week_task_indicator = "Week";
static const char *repeating_task_indicator = "Repeating";
static const char *date_format = "%d-%m-%y";

static const char *today_list_title = "Today";
static const char *week_list_title = "Week";
static const char *all_list_title = "All Tasks";
static const char *projects_list_title = "Projects";
static const char *completed_list_title = "Completed";

static void print_position(int pos)
{
    printf("[%d] ", pos);
}

static void print_task(const struct task *task)
{
    if (task->green)
        printf(COLOR_GREEN "%s" COLOR_RESET, task->name);
    else
        printf("%s", task->name);
    if (is_task_today(task) || !is_task_repeating(task))
        printf(" | " COLOR_CYAN "Days: %ld" COLOR_RESET, task_days(task));
}

static void print_today_indicator()
{
    printf(" | " COLOR_MAGENTA "%s" COLOR_RESET, today_task_indicator);
}

static void print_repeating_indicator()
{
    printf(" | " COLOR_MAGENTA "%s" COLOR_RESET, repeating_task_indicator);
}

static void print_week_indicator()
{
    printf(" | " COLOR_MAGENTA "%s" COLOR_RESET, week_task_indicator);
}

static void print_task_repeat_date(const struct task *task)
{
    char str[date_str_size];
    time_t repeat;
    repeat = task_repeat_date(task);
    strftime(str, date_str_size, date_format, localtime(&repeat));
    printf("Repeat: %s" COLOR_RESET, str);
}

static void print_today_list_task(const struct task *task)
{
    print_task(task);
    if (is_task_repeating(task))
        print_repeating_indicator();
    putchar('\n');
}

static void print_completed_list_task(const struct task *task)
{
    print_task(task);
    if (is_task_repeating(task))
        print_repeating_indicator();
    putchar('\n');
}

static void print_week_list_task(const struct task *task)
{
    print_task(task);
    if (is_task_today(task))
        print_today_indicator();
    else if (is_task_repeating(task)) {
        printf(" | " COLOR_MAGENTA);
        print_task_repeat_date(task);
        printf(COLOR_RESET);
    }
    putchar('\n');
}

static void print_all_list_task(const struct task *task)
{
    print_task(task);
    if (is_task_today(task)) {
        print_today_indicator();
    } else if (is_task_week(task)) {
        print_week_indicator();
        if (is_task_repeating(task))
            print_repeating_indicator();
    } else if (is_task_repeating(task)) {
        printf(" | " COLOR_MAGENTA);
        print_task_repeat_date(task);
        printf(COLOR_RESET);
    }
    putchar('\n');
}

static void print_project(const struct project *project)
{
    printf("%s\n", project->name);
}

static void print_view_title(const char *name)
{
    int i, decor_size;
    decor_size = (title_size - (strlen(name) + 2) + 1) / 2;
    for (i = 0; i < decor_size; i++)
        putchar(decoration);
    printf(" %s ", name);
    decor_size = title_size - (decor_size + strlen(name) + 2);
    for (i = 0; i < decor_size; i++)
        putchar(decoration);
    putchar('\n');
}

static void print_view_bottom()
{
    int i;
    for (i = 0; i < title_size; i++)
        putchar(decoration);
    putchar('\n');
}

static void concat_titles(char *title, int size, const char *first,
                                                    const char *second)
{
    strlcpy(title, first, size); 
    strlcat(title, second, size);
}

static void gen_today_tasks_title(char *title, int size)
{
    time_t today;
    char date_str[date_str_size];
    today = time(NULL);
    strftime(date_str, date_str_size, date_format, localtime(&today));
    strlcpy(title, today_list_title, size); 
    strlcat(title, ": ", size);
    strlcat(title, date_str, size);
}

void show_today_tasks(const struct task_list *lst)
{
    enum { max_today_title_size = 30 };
    struct tl_item *tmp;
    char title[max_today_title_size];
    task_id pos;
    pos = list_view_start_pos;
    gen_today_tasks_title(title, max_today_title_size);
    print_view_title(title);
    for (tmp = lst->first; tmp; tmp = tmp->next) {
        print_position(pos);
        print_today_list_task(&tmp->data);
        pos++;
    }
    print_view_bottom();
}

void show_week_tasks(const struct task_list *lst)
{
    struct tl_item *tmp;
    task_id pos;
    pos = list_view_start_pos;
    print_view_title(week_list_title);
    for (tmp = lst->first; tmp; tmp = tmp->next) {
        print_position(pos);
        print_week_list_task(&tmp->data);
        pos++;
    }
    print_view_bottom();
}

void show_all_tasks(const struct task_list *lst)
{
    struct tl_item *tmp;
    task_id pos;
    pos = list_view_start_pos;
    print_view_title(all_list_title);
    for (tmp = lst->first; tmp; tmp = tmp->next) {
        print_position(pos);
        print_all_list_task(&tmp->data);
        pos++;
    }
    print_view_bottom();
}

void show_completed_tasks(const struct task_list *lst)
{
    struct tl_item *tmp;
    task_id pos;
    pos = list_view_start_pos;
    print_view_title(completed_list_title);
    for (tmp = lst->first; tmp; tmp = tmp->next) {
        print_position(pos);
        print_completed_list_task(&tmp->data);
        pos++;
    }
    print_view_bottom();
}

void show_project_tasks(const struct task_list *lst, const char *proj_name)
{
    struct tl_item *tmp;
    task_id pos;
    pos = list_view_start_pos;
    print_view_title(proj_name);
    for (tmp = lst->first; tmp; tmp = tmp->next) {
        print_position(pos);
        print_all_list_task(&tmp->data);
        pos++;
    }
    print_view_bottom();
}

void show_project_completed_tasks(const struct task_list *lst,
                                                    const char *proj_name)
{
    struct tl_item *tmp;
    task_id pos;
    char title[50 + max_project_name_len];
    pos = list_view_start_pos;
    concat_titles(title, sizeof(title), "Completed: ", proj_name);
    print_view_title(title);
    for (tmp = lst->first; tmp; tmp = tmp->next) {
        print_position(pos);
        print_completed_list_task(&tmp->data);
        pos++;
    }
    print_view_bottom();
}

void show_projects(const struct project_list *lst)
{
    struct pl_item *tmp;
    task_id pos;
    pos = list_view_start_pos;
    print_view_title(projects_list_title);
    for (tmp = lst->first; tmp; tmp = tmp->next) {
        print_position(pos);
        print_project(&tmp->data);
        pos++;
    }
    print_view_bottom();
}

static void print_description(const char *description)
{
    printf("Description: %s\n", description);
}

static void print_task_repeat_info(const struct task *task)
{
    if (task->rep_days) {
        enum { week_days_cnt = 7 };
        const char *days[] = {
            "Mon",
            "Tue",
            "Wed",
            "Thu",
            "Fri",
            "Sat",
            "Sun" 
        };
        char day;
        printf("Repeat days: ");
        for (day = 1; day <= 7; day++) {
            if (task->rep_days & (1 << (day % 7)))
                printf("%s ", days[day - 1]);
        }
        putchar('\n');
    } else if (task->rep_interval) {
        printf("Repeat interval: %d\n", task->rep_interval);
    }
    print_task_repeat_date(task);
    putchar('\n');
}

void show_task_info(const struct task *task)
{
    enum { task_title_size = 20 + max_task_name_len };
    char title[task_title_size];
    concat_titles(title, task_title_size, "Task: ", task->name);
    print_view_title(title);
    print_description(task->description);
    print_task_repeat_info(task);
    print_view_bottom();
}

void show_project_info(const struct project *proj)
{
    enum { proj_title_size = 20 + max_project_name_len };
    char title[proj_title_size];
    concat_titles(title, proj_title_size, "Project: ", proj->name);
    print_view_title(title);
    print_description(proj->description);
    print_view_bottom();
}

void show_command_prompt()
{
    printf("> ");
}

void print_task_help()
{
    print_view_title("Task commands");
    printf("%c - all tasks\n", c_all_tasks);
    printf("%c - today tasks\n", c_today_tasks);
    printf("%c - week tasks\n", c_week_tasks);
    printf("%c - completed tasks\n", c_completed_tasks);
    printf("%c [name] - add task\n", c_add);
    printf("%c [pos] - remove task\n", c_remove);
    printf("%c [pos] - done task\n", c_done);
    printf("%c [pos] [t/w/n] - move task to list (t - today list /"
                               " w - week list / n - none)\n", c_move);
    printf("%c [pos] [project_pos] - move task to project\n", c_move);
    printf("%c%c [pos] [name] - set task name\n", c_set, c_set_name);
    printf("%c%c [pos] [description] - set task description\n", 
                                            c_set, c_set_description);
    printf("%c%c [pos] - set task green\n", c_set, c_set_green);
    printf("%c%c [pos] [new_pos] - set task list postion\n", 
                                                c_set, c_set_pos);
    printf("%c%c [pos] [week_day_number]- repeat task on (1 - Monday,"
           " ..., 7 - Sunday)\n", c_set, c_set_repeat_day);
    printf("%c%c [pos] [interval] [start_in] - repeat task with the"
           " interval starting in (start_in) days\n", 
           c_set, c_set_repeat_interval);
}

void print_project_help()
{
    print_view_title("Project commands");
    printf("%c - all projects\n", c_project);
    printf("%c [pos] - project tasks\n", c_project);
    printf("%c [pos] - completed project tasks\n", c_completed_tasks);
    printf("%c [name] - add project\n", c_add);
    printf("%c [pos] - remove project\n", c_remove);
    printf("%c%c [pos] [name] - set project name\n", c_set, c_set_name);
    printf("%c%c [pos] [description] - set project description\n", 
                                            c_set, c_set_description);
    printf("%c%c [pos] [new_pos] - set project postion\n", 
                                            c_set, c_set_pos);
}

void show_help()
{
    print_view_title("Help");
    printf("All command are performs on the last displayed list of"
           " tasks/projects\n");
    print_view_title("Base commands");
    printf("%c - help\n", c_help);
    printf("%c - quit\n", c_quit);
    print_task_help();
    print_project_help();
    print_view_bottom();
}
