#include "todolist_view.h"
#include "task_list.h"
#include "project_list.h"
#include "task.h"
#include "project.h"

#include <string.h>

enum {
    title_size = 25,
    list_title_decor = '='
};

static const char *today_task_indicator = "Today";
static const char *week_task_indicator = "Week";

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
    printf("%s", task->name);
}

static void print_today_list_task(const struct task *task)
{
    print_task(task);
    putchar('\n');
}

static void print_week_list_task(const struct task *task)
{
    print_task(task);
    if (task->folder == tf_today)
        printf(" | %s", today_task_indicator);
    putchar('\n');
}

static void print_all_list_task(const struct task *task)
{
    print_task(task);
    switch (task->folder) {
    case tf_today:
        printf(" | %s", today_task_indicator);
        break;
    case tf_week:
        printf(" | %s", week_task_indicator);
        break;
    default:
        break;
    }
    putchar('\n');
}

static void print_project(const struct project *project)
{
    printf("%s\n", project->name);
}

static void print_list_title(const char *name)
{
    int i, decor_size;
    decor_size = (title_size - (strlen(name) + 2)) / 2;
    for (i = 0; i < decor_size; i++)
        putchar(list_title_decor);
    printf(" %s ", name);
    decor_size = title_size - (decor_size + strlen(name) + 2);
    for (i = 0; i < decor_size; i++)
        putchar(list_title_decor);
    putchar('\n');
}

static void print_list_bottom()
{
    int i;
    for (i = 0; i < title_size; i++)
        putchar(list_title_decor);
    putchar('\n');
}

void show_today_tasks(const struct task_list *lst)
{
    struct tl_item *tmp;
    task_id pos = list_start_pos;
    print_list_title(today_list_title);
    for (tmp = lst->first; tmp; tmp = tmp->next) {
        print_position(pos);
        print_today_list_task(&tmp->data);
        pos++;
    }
    print_list_bottom();
}

void show_week_tasks(const struct task_list *lst)
{
    struct tl_item *tmp;
    task_id pos = list_start_pos;
    print_list_title(week_list_title);
    for (tmp = lst->first; tmp; tmp = tmp->next) {
        print_position(pos);
        print_week_list_task(&tmp->data);
        pos++;
    }
    print_list_bottom();
}

void show_all_tasks(const struct task_list *lst)
{
    struct tl_item *tmp;
    task_id pos = list_start_pos;
    print_list_title(all_list_title);
    for (tmp = lst->first; tmp; tmp = tmp->next) {
        print_position(pos);
        print_all_list_task(&tmp->data);
        pos++;
    }
    print_list_bottom();
}

void show_completed_tasks(const struct task_list *lst)
{
    struct tl_item *tmp;
    task_id pos = list_start_pos;
    print_list_title(completed_list_title);
    for (tmp = lst->first; tmp; tmp = tmp->next) {
        print_position(pos);
        print_all_list_task(&tmp->data);
        pos++;
    }
    print_list_bottom();
}

void show_project_tasks(const struct task_list *lst, const char *proj_name)
{
    struct tl_item *tmp;
    task_id pos = list_start_pos;
    print_list_title(proj_name);
    for (tmp = lst->first; tmp; tmp = tmp->next) {
        print_position(pos);
        print_all_list_task(&tmp->data);
        pos++;
    }
    print_list_bottom();
}

static void gen_project_completed_title(char *str, int size, 
                                                const char* project_name)
{
    const char *completed_title = "Completed: ";
    strlcpy(str, completed_title, size);
    strncat(str, project_name, max_project_name_len - 1);
}


void show_project_completed_tasks(const struct task_list *lst,
                                                    const char *proj_name)
{
    struct tl_item *tmp;
    task_id pos = list_start_pos;
    char title[50 + max_project_name_len];
    gen_project_completed_title(title, sizeof(title), proj_name);
    print_list_title(title);
    for (tmp = lst->first; tmp; tmp = tmp->next) {
        print_position(pos);
        print_all_list_task(&tmp->data);
        pos++;
    }
    print_list_bottom();
}

void show_projects(const struct project_list *lst)
{
    struct pl_item *tmp;
    task_id pos = list_start_pos;
    print_list_title(projects_list_title);
    for (tmp = lst->first; tmp; tmp = tmp->next) {
        print_position(pos);
        print_project(&tmp->data);
        pos++;
    }
    print_list_bottom();
}
