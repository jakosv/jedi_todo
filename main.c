#include "task.h"
#include "project.h"
#include "todolist.h"

#include <stdio.h>
#include <string.h>

void add_task(const char *name, struct todolist *list)
{
    struct task new_task;
    int i;
    strcpy(new_task.name, name);
    new_task.has_project = 0;
    new_task.pid = 0;
    for (i = 0; i < 7; i++)
        new_task.repeat_days[i] = 0;
    new_task.folder = tf_none;
    new_task.green = 1;

    todolist_add_task(&new_task, list);
}

void print_tasks(const struct task_list *lst)
{
    struct tl_item *tmp;
    for (tmp = lst->first; tmp; tmp = tmp->next)
        printf("[%d] %s\n", tmp->data.id, tmp->data.name);
}

int main() {
    struct todolist list;
    struct task_list *tasks;
    const char *task_name = "First task";
    const char *test_name = "Second task";
    const char *another_name = "Another task";

    todolist_init(&list); 
    tasks = &list.tasks; 

    add_task(task_name, &list);
    puts("===========");
    print_tasks(&list.tasks);
    puts("===========");

    add_task(test_name, &list);
    puts("===========");
    print_tasks(&list.tasks);
    puts("===========");

    add_task(another_name, &list);
    puts("===========");
    print_tasks(&list.tasks);
    puts("===========");

    todolist_delete_task(1, &list);
    puts("===========");
    print_tasks(&list.tasks);
    puts("===========");
    
    add_task(another_name, &list);
    puts("===========");
    print_tasks(&list.tasks);
    puts("===========");

    todolist_destroy(&list);

    return 0;
}
