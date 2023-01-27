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

static void get_command(char *cmd, int len)
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

static void parse_command(const char *cmd, char **params, int size,
                                                        int *params_cnt)
{
    int i, param_len, cmd_len;
    char *param;
    cmd_len = strlen(cmd);
    param = malloc(cmd_len + 1);
    param_len = 0;
    *params_cnt = 0;
    for (i = 0; cmd[i]; i++) {
        if (*params_cnt >= size)
            break;
        if (cmd[i] == ' ' && param_len > 0) {
            param[param_len] = '\0';
            strlcpy(params[*params_cnt], param, param_len + 1);
            (*params_cnt)++;
            param_len = 0;
        } else if (cmd[i] != ' ') {
            param[param_len] = cmd[i];
            param_len++;
        }
    }
    if (param_len > 0) {
        param[param_len] = '\0';
        strlcpy(params[*params_cnt], param, param_len + 1);
        (*params_cnt)++;
    }
    free(param);
}

void todolist_main_loop(struct todolist *list)
{
    enum { max_cmd_len = 200, max_params_cnt = 10 };
    char cmd[max_cmd_len];
    char *params[max_params_cnt];
    int params_cnt, i;
    for (i = 0; i < max_params_cnt; i++)
        params[i] = malloc(max_cmd_len);
    do {
        show_list(list);
        
        get_command(cmd, max_cmd_len);
        parse_command(cmd, params, max_params_cnt, &params_cnt); 
        printf("Params count: %d\n", params_cnt);
        for (i = 0; i < params_cnt; i++)
            printf("Param [%d]: '%s'\n", i, params[i]);

        switch (params[0][0]) {
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
    } while (params[0][0] != 'q');

    for (i = 0; i < max_params_cnt; i++)
        free(params[i]);
}
