#include "task.h"
#include "project.h"
#include "todolist.h"

#include <stdio.h>
#include <string.h>

void add_task(const char *name, enum task_folder folder, 
                                                struct todolist *list)
{
    struct task new_task;
    int i;
    strcpy(new_task.name, name);
    new_task.has_project = 0;
    new_task.pid = 0;
    for (i = 0; i < 7; i++)
        new_task.repeat_days[i] = 0;
    new_task.folder = folder;
    new_task.green = 1;

    todolist_add_task(&new_task, list);
}

void print_task(const struct task *task)
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

void print_tasks(const struct task_list *lst)
{
    struct tl_item *tmp;
    for (tmp = lst->first; tmp; tmp = tmp->next) {
        printf("[%d] ", tmp->id);
        print_task(&tmp->data);
    }
}

int main() {
    struct todolist list;
    struct task_list tasks;
    const char *task_name = "First task";
    const char *test_name = "Second task";
    const char *another_name = "Another task";

    todolist_init(&list); 
    tl_init(&tasks);
    todolist_get_all_tasks(&tasks, &list);

    add_task(task_name, tf_none, &list);
    tl_clear(&tasks);
    todolist_get_all_tasks(&tasks, &list);
    puts("===========");
    print_tasks(&tasks);
    puts("===========");

    add_task(test_name, tf_today, &list);
    tl_clear(&tasks);
    todolist_get_all_tasks(&tasks, &list);
    puts("===========");
    print_tasks(&tasks);
    puts("===========");

    add_task(another_name, tf_today, &list);
    tl_clear(&tasks);
    todolist_get_all_tasks(&tasks, &list);
    puts("===========");
    print_tasks(&tasks);
    puts("===========");

    tl_clear(&tasks);
    todolist_get_all_tasks(&tasks, &list);

    puts("===========");
    print_tasks(&tasks);
    puts("===========");

    todolist_delete_task(1, &list);
    tl_clear(&tasks);
    todolist_get_all_tasks(&tasks, &list);
    puts("===========");
    print_tasks(&tasks);
    puts("===========");
    
    add_task(another_name, tf_none, &list);
    tl_clear(&tasks);
    todolist_get_all_tasks(&tasks, &list);
    puts("===========");
    print_tasks(&tasks);
    puts("===========");

    add_task(another_name, tf_none, &list);
    tl_clear(&tasks);
    todolist_get_today_tasks(&tasks, &list);
    puts("===== TODAY TASKS ======");
    print_tasks(&tasks);
    puts("===========");

    tl_clear(&tasks);
    todolist_free(&list);

    return 0;
}
