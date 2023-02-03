#include "todolist_view.h"
#include "task_list.h"
#include "project_list.h"
#include "task.h"
#include "project.h"

#include <string.h>

enum {
    decor_size = 3,
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

static void show_list_head(const char *name)
{
    int i;
    for (i = 0; i < decor_size; i++)
        putchar(list_title_decor);
    putchar(' ');
    printf("%s", name);
    putchar(' ');
    for (i = 0; i < decor_size; i++)
        putchar(list_title_decor);
    putchar('\n');
}

static void show_list_bottom(int size)
{
    int i;
    for (i = 0; i < size; i++)
        putchar(list_title_decor);
    putchar('\n');
}

void show_task_list(const struct task_list *lst, const char *name)
{
    struct tl_item *tmp;
    int title_size;
    task_id pos;
    title_size = strlen(name) + 2 * (decor_size + 1);
    show_list_head(name);
    for (tmp = lst->first, pos = 1; tmp; tmp = tmp->next, pos++) {
        printf("[%d] ", pos);
        print_task(&tmp->data);
    }
    show_list_bottom(title_size);
}

void show_project_list(const struct project_list *lst)
{
    struct pl_item *tmp;
    const char *name = "Projects";
    int title_size;
    project_id pos;
    title_size = strlen(name) + 2 * (decor_size + 1);
    show_list_head(name);
    for (tmp = lst->first, pos = 1; tmp; tmp = tmp->next, pos++) {
        printf("[%d] ", pos);
        print_project(&tmp->data);
    }
    show_list_bottom(title_size);
}
