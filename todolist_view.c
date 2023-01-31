#include "todolist_view.h"
#include "task_list.h"
#include "task.h"

#include <string.h>

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
    printf("\n");
}

void show_task_list(const struct task_list *lst, const char *name)
{
    enum { decor_size = 3 };
    struct tl_item *tmp;
    const char list_title_decor = '=';
    int title_size, i;
    task_id pos;
    title_size = strlen(name) + 2 * (decor_size + 1);
    for (i = 0; i < decor_size; i++)
        putchar(list_title_decor);
    putchar(' ');
    printf("%s", name);
    putchar(' ');
    for (i = 0; i < decor_size; i++)
        putchar(list_title_decor);
    putchar('\n');
    for (tmp = lst->first, pos = 1; tmp; tmp = tmp->next, pos++) {
        printf("[%d] ", pos);
        print_task(&tmp->data);
    }
    for (i = 0; i < title_size; i++)
        putchar(list_title_decor);
    putchar('\n');
}
