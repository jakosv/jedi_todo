#include "task.h"
#include "project.h"
#include "todolist.h"

#include <stdio.h>
#include <string.h>

enum view_state { 
    vs_today_tasks, 
    vs_all_tasks, 
    vs_week_tasks,
    vs_projects,
    vs_project_tasks
};

struct application {
    struct todolist todo_list;
    enum view_state view;
    union {
        struct task_list tasks;
        /*
        struct project_list projects;
        */
    } cur_list;
};

void app_init(struct application *app)
{
    todolist_init(&app->todo_list); 
    tl_init(&app->cur_list.tasks);
    todolist_get_all_tasks(&app->cur_list.tasks, &app->todo_list);
    app->view = vs_all_tasks;
}

void app_destroy(struct application *app)
{
    todolist_free(&app->todo_list); 
    tl_clear(&app->cur_list.tasks);
}

void add_task(const char *name, enum task_folder folder, 
                                                struct todolist *list)
{
    struct task new_task;
    task_create(name, folder, &new_task);
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

void print_list(const struct application *app)
{
    puts("===========");
    if (app->view < vs_projects)
        print_tasks(&app->cur_list.tasks);
    puts("===========");
}

int main()
{
    const char *task_name = "First task";
    const char *test_name = "Second task";
    const char *another_name = "Another task";
    char cmd;
    struct application app;

    app_init(&app);

    do {
        print_list(&app);

        do {
            cmd = getchar();
        } while (cmd == ' ' || cmd == '\n');

        switch (cmd) {
        case 'a':
            break;
        case 't':
            app.view = vs_today_tasks;
            tl_clear(&app.cur_list.tasks);
            todolist_get_today_tasks(&app.cur_list.tasks, &app.todo_list);
            break;
        case 'l':
            app.view = vs_all_tasks;
            tl_clear(&app.cur_list.tasks);
            todolist_get_all_tasks(&app.cur_list.tasks, &app.todo_list);
            break;
        default:
            break;
        }
    } while (cmd != 'q');


    app_destroy(&app);
    /*
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
    */

    /*
    tl_clear(&tasks);
    todolist_free(&list);
    */

    return 0;
}
