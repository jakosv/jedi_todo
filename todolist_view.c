#include "todolist_view.h"
#include "task_list.h"
#include "task.h"

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

void show_task_list(const struct task_list *lst)
{
    struct tl_item *tmp;
    task_id pos;
    puts("========");
    for (tmp = lst->first, pos = 1; tmp; tmp = tmp->next, pos++) {
        printf("[%d] ", pos);
        print_task(&tmp->data);
    }
    puts("========");
}
