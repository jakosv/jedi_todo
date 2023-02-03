#include "todolist.h"
#include "todolist_view.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

enum {
    max_cmd_len = 200, 
    add_params_cnt = 2,
    remove_params_cnt = 2,
    rename_params_cnt = 3,
    done_params_cnt = 2,
    move_params_cnt = 3 
};

void todolist_init(struct todolist *list)
{
    storage_init(&list->storage); 
    tl_init(&list->tasks);
    pl_init(&list->projects);
    storage_get_all_tasks(&list->tasks, &list->storage);
    list->view = view_all_tasks;
}

void todolist_destroy(struct todolist *list)
{
    storage_free(&list->storage); 
    tl_clear(&list->tasks);
    pl_clear(&list->projects);
}

static void show_list(const struct todolist *list)
{
    switch (list->view) {
    case view_today_tasks:
        show_task_list(&list->tasks, "Today");
        break;
    case view_all_tasks:
        show_task_list(&list->tasks, "All");
        break;
    case view_week_tasks:
        show_task_list(&list->tasks, "Week");
        break;
    case view_completed_tasks:
        show_task_list(&list->tasks, "Completed");
        break;
    case view_projects:
        show_project_list(&list->projects);
        break;
    default:
        show_task_list(&list->tasks, "All");
    }
}

static void update_todolist_view(enum view_state view, 
                                                struct todolist *list)
{
    list->view = view;
    switch (view) {
    case view_today_tasks:
        tl_clear(&list->tasks);
        storage_get_today_tasks(&list->tasks, &list->storage);
        break;
    case view_all_tasks:
        tl_clear(&list->tasks);
        storage_get_all_tasks(&list->tasks, &list->storage);
        break;
    case view_week_tasks:
        tl_clear(&list->tasks);
        storage_get_week_tasks(&list->tasks, &list->storage);
        break;
    case view_completed_tasks:
        tl_clear(&list->tasks);
        storage_get_completed_tasks(&list->tasks, &list->storage);
        break;
    case view_projects:
        pl_clear(&list->projects);
        storage_get_all_projects(&list->projects, &list->storage);
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

static void add_project(const char *name, struct todolist *list)
{
    struct project new_project;
    project_create(name, &new_project);
    storage_add_project(&new_project, &list->storage);
    update_todolist_view(list->view, list);
}

static void remove_task(int pos, struct todolist *list)
{
    task_id id;
    id = tl_get_item(pos - 1, &list->tasks)->id;
    storage_delete_task(id, &list->storage); 
    update_todolist_view(list->view, list);
}

static void remove_project(int pos, struct todolist *list)
{
    project_id id;
    id = pl_get_item(pos - 1, &list->projects)->id;
    storage_delete_project(id, &list->storage); 
    update_todolist_view(list->view, list);
}

static void rename_task(int pos, const char *name, 
                                            struct todolist *list)
{
    struct tl_item *task_item;
    struct task new_task;
    task_item = tl_get_item(pos - 1, &list->tasks);
    new_task = task_item->data;
    strlcpy(new_task.name, name, max_task_name_len);
    storage_set_task(task_item->id, &new_task, &list->storage); 
    update_todolist_view(list->view, list);
}

static void done_task(int pos, struct todolist *list)
{
    struct tl_item *task_item;
    struct task new_task;
    task_item = tl_get_item(pos - 1, &list->tasks);
    new_task = task_item->data;
    new_task.done = new_task.done ? 0 : 1;
    storage_set_task(task_item->id, &new_task, &list->storage); 
    update_todolist_view(list->view, list);
}

static int project_exists(project_id pid)
{
    return 0;
}

static void move_task_to_project(int pos, project_id pid, 
                                            struct todolist *list)
{
    struct tl_item *task_item;
    struct task new_task;
    if (!project_exists(pid))
        return;
    task_item = tl_get_item(pos - 1, &list->tasks);
    new_task = task_item->data;
    new_task.has_project = 1;
    new_task.pid = pid;
    storage_set_task(task_item->id, &new_task, &list->storage); 
    update_todolist_view(list->view, list);
}

static void move_task_to_folder(int pos, char to, struct todolist *list)
{
    struct tl_item *task_item;
    struct task new_task;
    task_item = tl_get_item(pos - 1, &list->tasks);
    new_task = task_item->data;
    switch (to) {
    case 't':
        new_task.folder = tf_today;
        break;
    case 'w':
        new_task.folder = tf_week;
        break;
    case 'n':
        new_task.folder = tf_none;
        break;
    default:
        return;
    }
    storage_set_task(task_item->id, &new_task, &list->storage); 
    update_todolist_view(list->view, list);
}

static void swap_tasks(int pos, int new_pos, struct todolist *list)
{
    struct tl_item *first_task_item, *second_task_item;
    struct task first_task, second_task;
    first_task_item = tl_get_item(pos - 1, &list->tasks);
    second_task_item = tl_get_item(new_pos - 1, &list->tasks);
    first_task = first_task_item->data;
    second_task = second_task_item->data;
    storage_set_task(first_task_item->id, &second_task, &list->storage); 
    storage_set_task(second_task_item->id, &first_task, &list->storage); 
    update_todolist_view(list->view, list);
}

static void swap_projects(int pos, int new_pos, struct todolist *list)
{
    struct pl_item *first_project_item, *second_project_item;
    struct project first_project, second_project;
    first_project_item = pl_get_item(pos - 1, &list->projects);
    second_project_item = pl_get_item(new_pos - 1, &list->projects);
    first_project = first_project_item->data;
    second_project = second_project_item->data;
    storage_set_project(first_project_item->id, &second_project, 
                                                        &list->storage); 
    storage_set_project(second_project_item->id, &first_project, 
                                                        &list->storage); 
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

static void params_array_init(char **params, int size)
{
    int i;
    for (i = 0; i < size; i++)
        params[i] = malloc(max_cmd_len);
}

static void params_array_free(char **params, int size)
{
    int i;
    for (i = 0; i < size; i++)
        free(params[i]);
}

static void command_add(const char *cmd, struct todolist *list) 
{
    char *params[add_params_cnt];
    int parse_cnt;
    params_array_init(params, add_params_cnt);
    parse_command(cmd, add_params_cnt, params, &parse_cnt); 
    if (parse_cnt >= add_params_cnt) {
        if (list->view == view_projects)
            add_project(params[1], list);
        else
            add_task(params[1], list);
    }
    params_array_free(params, add_params_cnt);
}

static void command_remove(const char *cmd, struct todolist *list) 
{
    char *params[remove_params_cnt];
    int parse_cnt;
    params_array_init(params, remove_params_cnt);
    parse_command(cmd, remove_params_cnt, params, &parse_cnt); 
    if (parse_cnt >= remove_params_cnt) {
        int pos; 
        char *end;
        pos = strtol(params[1], &end, 10);
        if (list->view == view_projects)
            remove_project(pos, list);
        else
            remove_task(pos, list);
    }
    params_array_free(params, remove_params_cnt);
}

static void command_rename(const char *cmd, struct todolist *list) 
{
    char *params[rename_params_cnt];
    int parse_cnt;
    params_array_init(params, rename_params_cnt);
    parse_command(cmd, rename_params_cnt, params, &parse_cnt); 
    if (parse_cnt >= rename_params_cnt) {
        int pos; 
        char *end;
        pos = strtol(params[1], &end, 10);
        rename_task(pos, params[2], list);
    }
    params_array_free(params, rename_params_cnt);
}

static void command_done(const char *cmd, struct todolist *list) 
{
    char *params[done_params_cnt];
    int parse_cnt;
    params_array_init(params, done_params_cnt);
    parse_command(cmd, done_params_cnt, params, &parse_cnt); 
    if (parse_cnt >= done_params_cnt) {
        int pos; 
        char *end;
        pos = strtol(params[1], &end, 10);
        done_task(pos, list);
    }
    params_array_free(params, done_params_cnt);
}

static void command_move(const char *cmd, struct todolist *list) 
{
    char *params[move_params_cnt];
    int parse_cnt;
    params_array_init(params, move_params_cnt);
    parse_command(cmd, move_params_cnt, params, &parse_cnt); 
    if (parse_cnt >= move_params_cnt) {
        int pos; 
        char *end;
        pos = strtol(params[1], &end, 10);
        if (isdigit(params[2][0])) {
            project_id proj_id;
            proj_id = strtol(params[2], &end, 10);
            move_task_to_project(pos, proj_id, list);
        } else {
            move_task_to_folder(pos, params[2][0], list);
        }
    }
    params_array_free(params, move_params_cnt);
}

static void command_swap(const char *cmd, struct todolist *list) 
{
    char *params[move_params_cnt];
    int parse_cnt;
    params_array_init(params, move_params_cnt);
    parse_command(cmd, move_params_cnt, params, &parse_cnt); 
    if (parse_cnt >= move_params_cnt) {
        int pos; 
        char *end;
        pos = strtol(params[1], &end, 10);
        if (isdigit(params[2][0])) {
            task_id new_pos;
            new_pos = strtol(params[2], &end, 10);
            if (list->view == view_projects)
                swap_projects(pos, new_pos, list);
            else
                swap_tasks(pos, new_pos, list);
        }
    }
    params_array_free(params, move_params_cnt);
}

void todolist_main_loop(struct todolist *list)
{
    char cmd[max_cmd_len];
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
            command_add(cmd, list);
            break;
        case 'd':
            command_done(cmd, list);
            break;
        case 'r':
            command_remove(cmd, list);
            break;
        case 'n':
            command_rename(cmd, list);
            break;
        case 'm':
            command_move(cmd, list);
            break;
        case 's':
            command_swap(cmd, list);
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
        case 'c':
            update_todolist_view(view_completed_tasks, list);
            break;
        case 'p':
            update_todolist_view(view_projects, list);
            break;
        default:
            break;
        }
    } while (cmd[0] != 'q');

}
