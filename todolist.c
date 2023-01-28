#include "todolist.h"
#include "todolist_view.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

static void update_todolist_view(enum view_state view, 
                                                struct todolist *list)
{
    tl_clear(&list->tasks);
    list->view = view;
    switch (view) {
    case view_today_tasks:
        storage_get_today_tasks(&list->tasks, &list->storage);
        break;
    case view_all_tasks:
        storage_get_all_tasks(&list->tasks, &list->storage);
        break;
    case view_week_tasks:
        storage_get_week_tasks(&list->tasks, &list->storage);
        break;
    default:
        storage_get_all_tasks(&list->tasks, &list->storage);
    }
}

static void add_task(const char *name, struct todolist *list)
{
    struct task new_task;
    enum task_folder folder;
    switch (list->view) {
    case view_today_tasks:
        folder = tf_today;
        break;
    case view_all_tasks:
        folder = tf_none;
        break;
    case view_week_tasks:
        folder = tf_week;
        break;
    case view_project_tasks:
        folder = tf_none;
        break;
    default:
        folder = tf_none;
        break;
    }
    task_create(name, folder, &new_task);
    storage_add_task(&new_task, &list->storage);
    update_todolist_view(list->view, list);
}

static void remove_task(int pos, struct todolist *list)
{
    task_id id;
    id = tl_get_item(pos - 1, &list->tasks)->id;
    storage_delete_task(id, &list->storage); 
    update_todolist_view(list->view, list);
}

static void read_command(char *cmd, int len)
{
    int c, i;
    i = 0;
    while ((c = getchar()) != EOF) {
        if (c == '\n' || i >= len - 1)
            break;
        cmd[i] = c; 
        i++;
    }
    cmd[i] = '\0';
}

static void parse_command(const char *cmd, int count, char **params, 
                                                        int *parse_cnt)
{
    int i, param_len, cmd_len;
    char *param;
    cmd_len = strlen(cmd);
    param = malloc(cmd_len + 1);
    param_len = 0;
    *parse_cnt = 0;
    for (i = 0; cmd[i]; i++) {
        if (*parse_cnt < (count - 1) && cmd[i] == ' ' && param_len > 0) {
            param[param_len] = '\0';
            strlcpy(params[*parse_cnt], param, param_len + 1);
            (*parse_cnt)++;
            param_len = 0;
        } else {
            param[param_len] = cmd[i];
            param_len++;
        }
    }
    if (*parse_cnt < count && param_len > 0) {
        param[param_len] = '\0';
        strlcpy(params[*parse_cnt], param, param_len + 1);
        (*parse_cnt)++;
    }
    free(param);
}

void todolist_main_loop(struct todolist *list)
{
    enum {
        max_cmd_len = 200, 
        max_params_cnt = 10,  
        add_params_cnt = 2,
        remove_params_cnt = 2,
        rename_params_cnt = 3,
        move_params_cnt = 3 
    };
    char cmd[max_cmd_len];
    char *params[max_params_cnt];
    int parse_cnt, i;
    for (i = 0; i < max_params_cnt; i++)
        params[i] = malloc(max_cmd_len);
    do {
        show_list(list);
        
        read_command(cmd, max_cmd_len);
        /*
        printf("Params count: %d\n", params_cnt);
        for (i = 0; i < params_cnt; i++)
            printf("Param [%d]: '%s'\n", i, params[i]);
            */

        switch (cmd[0]) {
        case 'a':
            parse_command(cmd, add_params_cnt, params, &parse_cnt); 
            if (parse_cnt >= add_params_cnt)
                add_task(params[1], list);
            break;
        case 'r':
            parse_command(cmd, remove_params_cnt, params, &parse_cnt); 
            if (parse_cnt >= remove_params_cnt) {
                int pos; 
                char *end;
                pos = strtol(params[1], &end, 10);
                remove_task(pos, list);
            }
            break;
        case 't':
            update_todolist_view(view_today_tasks, list);
            break;
        case 'l':
            update_todolist_view(view_all_tasks, list);
            break;
        case 'w':
            update_todolist_view(view_week_tasks, list);
            break;
        default:
            break;
        }
    } while (cmd[0] != 'q');

    for (i = 0; i < max_params_cnt; i++)
        free(params[i]);
}
