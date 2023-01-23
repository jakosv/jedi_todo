#include "todolist.h"

/*
void add_task(const char *name, enum task_folder folder, 
                                                struct todolist *list)
{
    struct task new_task;
    task_create(name, folder, &new_task);
    todolist_add_task(&new_task, list);
}
*/

int main()
{
    struct todolist list;

    todolist_init(&list);

    todolist_main_loop(&list);

    todolist_destroy(&list);

    return 0;
}
