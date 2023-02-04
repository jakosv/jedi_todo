#include "todolist_view.h"
#include "task_list.h"
#include "project_list.h"
#include "task.h"
#include "project.h"

#include <string.h>

enum {
    title_size = 21,
    list_title_decor = '='
};

static void print_task(const struct task *task)
{
    printf("%s", task->name);
    switch (task->folder) {
    case tf_today:
        printf(" | Today");
        break;
    case tf_week:
        printf(" | Week");
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

void show_task_list(const struct task_list *lst, const char *name)
{
    struct tl_item *tmp;
    task_id pos;
    print_list_title(name);
    for (tmp = lst->first, pos = 1; tmp; tmp = tmp->next, pos++) {
        printf("[%d] ", pos);
        print_task(&tmp->data);
    }
    print_list_bottom();
}

void show_project_list(const struct project_list *lst)
{
    struct pl_item *tmp;
    const char *name = "Projects";
    project_id pos;
    print_list_title(name);
    for (tmp = lst->first, pos = 1; tmp; tmp = tmp->next, pos++) {
        printf("[%d] ", pos);
        print_project(&tmp->data);
    }
    print_list_bottom();
}
