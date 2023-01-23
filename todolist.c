#include "todolist.h"
#include "todolist_view.h"

#include <stdio.h>

void todolist_init(struct todolist *list)
{
    storage_open(&list->storage); 
    tl_init(&list->tasks);
    /* pl_init(&list-projects); */
    storage_get_all_tasks(&list->tasks, &list->storage);
    list->view = view_all_tasks;
}

void todolist_destroy(struct todolist *list)
{
    storage_close(&list->storage); 
    tl_clear(&list->tasks);
    /*pl_clear(&app->cur_list)*/
}

static void show_list(const struct todolist *list)
{
    if (list->view < view_projects)
        show_task_list(&list->tasks);
}

void todolist_main_loop(struct todolist *list)
{
    char cmd;
    do {
        show_list(list);

        do {
            cmd = getchar();
        } while (cmd == ' ' || cmd == '\n');

        switch (cmd) {
        case 'a':
            break;
        case 't':
            list->view = view_today_tasks;
            tl_clear(&list->tasks);
            storage_get_today_tasks(&list->tasks, &list->storage);
            break;
        case 'l':
            list->view = view_all_tasks;
            tl_clear(&list->tasks);
            storage_get_all_tasks(&list->tasks, &list->storage);
            break;
        default:
            break;
        }
    } while (cmd != 'q');
}
