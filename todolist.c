#include "todolist.h"
#include "todolist_view.h"
#include "todolist_command.h"
#include "task.h"
#include "project.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void todolist_init(struct todolist *list)
{
    storage_init(&list->storage); 
    tl_init(&list->tasks);
    pl_init(&list->projects);
    storage_get_all_tasks(&list->tasks, &list->storage);
    storage_get_all_projects(&list->projects, &list->storage);
    list->view = view_today_tasks;
}

void todolist_destroy(struct todolist *list)
{
    storage_free(&list->storage); 
    tl_clear(&list->tasks);
    pl_clear(&list->projects);
}

static void show_list(const struct todolist *list)
{
    char *project_name;
    switch (list->view) {
    case view_today_tasks:
        show_today_tasks(&list->tasks);
        break;
    case view_all_tasks:
        show_all_tasks(&list->tasks);
        break;
    case view_week_tasks:
        show_week_tasks(&list->tasks);
        break;
    case view_completed_tasks:
        show_completed_tasks(&list->tasks);
        break;
    case view_projects:
        show_projects(&list->projects);
        break;
    case view_project_tasks:
        project_name = 
            pl_get_item(list->cur_project, &list->projects)->data.name;
        show_project_tasks(&list->tasks, project_name);
        break;
    case view_project_completed_tasks:
        project_name = 
            pl_get_item(list->cur_project, &list->projects)->data.name;
        show_project_completed_tasks(&list->tasks, project_name);
        break;
    default:
        break;
    }
}

static void update_todolist_view(enum view_state view, 
                                                struct todolist *list)
{
    project_id pid;
    if (view == view_projects)
        pl_clear(&list->projects);
    else
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
    case view_completed_tasks:
        storage_get_completed_tasks(&list->tasks, &list->storage);
        break;
    case view_projects:
        storage_get_all_projects(&list->projects, &list->storage);
        break;
    case view_project_tasks:
        pid = pl_get_item(list->cur_project, &list->projects)->id;
        storage_get_project_tasks(pid, &list->tasks, &list->storage);
        break;
    case view_project_completed_tasks:
        pid = pl_get_item(list->cur_project, &list->projects)->id;
        storage_get_project_completed_tasks(pid, &list->tasks, 
                                                        &list->storage);
        break;
    default:
        break;
    }
}

static void add_task(const char *name, struct todolist *list)
{
    struct task new_task;
    enum task_folder folder = tf_none;
    project_id pid = 0;
    char has_project = 0;
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
        has_project = 1;
        pid = pl_get_item(list->cur_project, &list->projects)->id;
        folder = tf_none;
        break;
    default:
        return;
    }
    task_create(name, folder, &new_task);
    new_task.has_project = has_project;
    if (has_project)
        new_task.pid = pid;
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
    id = tl_get_item(pos, &list->tasks)->id;
    storage_delete_task(id, &list->storage); 
    update_todolist_view(list->view, list);
}

static void remove_tasks_from_project(int pos, struct todolist *list)
{
    project_id id;
    struct task_list project_tasks;
    struct tl_item *tmp;
    id = pl_get_item(pos, &list->projects)->id;
    tl_init(&project_tasks);
    storage_get_project_tasks(id, &project_tasks, &list->storage); 
    tmp = project_tasks.first;
    while (tmp) {
        tmp->data.has_project = 0;
        storage_set_task(tmp->id, &tmp->data, &list->storage);
        tmp = tmp->next;
    }
}

static void remove_project(int pos, struct todolist *list)
{
    project_id id;
    id = pl_get_item(pos, &list->projects)->id;
    storage_delete_project(id, &list->storage); 
    update_todolist_view(list->view, list);
}

static void rename_task(int pos, const char *name, 
                                            struct todolist *list)
{
    struct tl_item *task_item;
    struct task new_task;
    task_item = tl_get_item(pos, &list->tasks);
    new_task = task_item->data;
    strlcpy(new_task.name, name, max_task_name_len);
    storage_set_task(task_item->id, &new_task, &list->storage); 
    update_todolist_view(list->view, list);
}

static void rename_project(int pos, const char *name, 
                                            struct todolist *list)
{
    struct pl_item *proj_item;
    struct project proj;
    proj_item = pl_get_item(pos, &list->projects);
    proj = proj_item->data;
    strlcpy(proj.name, name, max_project_name_len);
    storage_set_project(proj_item->id, &proj, &list->storage); 
    update_todolist_view(list->view, list);
}

static void set_task_description(int pos, const char *description, 
                                            struct todolist *list)
{
    struct tl_item *task_item;
    struct task new_task;
    task_item = tl_get_item(pos, &list->tasks);
    new_task = task_item->data;
    strlcpy(new_task.description, description, max_task_descript_len);
    storage_set_task(task_item->id, &new_task, &list->storage); 
    update_todolist_view(list->view, list);
}

static void set_project_description(int pos, const char *description, 
                                            struct todolist *list)
{
    struct pl_item *proj_item;
    struct project proj;
    proj_item = pl_get_item(pos, &list->projects);
    proj = proj_item->data;
    strlcpy(proj.description, description, max_project_descript_len);
    storage_set_project(proj_item->id, &proj, &list->storage); 
    update_todolist_view(list->view, list);
}

static void done_task(int pos, struct todolist *list)
{
    struct tl_item *task_item;
    struct task task;
    task_item = tl_get_item(pos, &list->tasks);
    task = task_item->data;
    task.done = task.done ? 0 : 1;
    storage_set_task(task_item->id, &task, &list->storage); 
    if (task.done && is_task_repeating(&task)) {
        task.done = 0;
        task.creation_time = get_next_repeat(&task);
        storage_add_task(&task, &list->storage);
    }
    update_todolist_view(list->view, list);
}

static void move_task_to_project(int pos, project_id project_pos, 
                                            struct todolist *list)
{
    struct tl_item *task_item;
    struct pl_item *project_item;
    struct task new_task;
    project_item = pl_get_item(project_pos, &list->projects);
    if (!project_item)
        return;
    task_item = tl_get_item(pos, &list->tasks);
    new_task = task_item->data;
    new_task.has_project = 1;
    new_task.pid = project_item->id;
    storage_set_task(task_item->id, &new_task, &list->storage); 
    update_todolist_view(list->view, list);
}

static void move_task_to_folder(int pos, char to, struct todolist *list)
{
    struct tl_item *task_item;
    struct task new_task;
    task_item = tl_get_item(pos, &list->tasks);
    new_task = task_item->data;
    if (is_task_repeating(&new_task)) {
        new_task.creation_time = get_next_repeat(&new_task);
        storage_add_task(&new_task, &list->storage);
        task_unrepeat(&new_task);
    }
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

static void set_task_pos(int pos, int new_pos, struct todolist *list)
{
    struct tl_item *first_task_item, *second_task_item;
    struct task first_task, second_task;
    first_task_item = tl_get_item(pos, &list->tasks);
    second_task_item = tl_get_item(new_pos, &list->tasks);
    first_task = first_task_item->data;
    second_task = second_task_item->data;
    storage_set_task(first_task_item->id, &second_task, &list->storage); 
    storage_set_task(second_task_item->id, &first_task, &list->storage); 
    update_todolist_view(list->view, list);
}

static void set_task_green(int pos, struct todolist *list)
{
    struct tl_item *task_item;
    struct task new_task;
    task_item = tl_get_item(pos, &list->tasks);
    new_task = task_item->data;
    new_task.green = new_task.green ? 0 : 1;
    storage_set_task(task_item->id, &new_task, &list->storage); 
    update_todolist_view(list->view, list);
}

static void set_task_repeat_interval(int pos, int interval, int start_in, 
                                                    struct todolist *list)
{
    struct tl_item *task_item;
    struct task task;
    task_item = tl_get_item(pos, &list->tasks);
    task = task_item->data;
    task_add_repeat_interval(interval, start_in, &task);
    storage_set_task(task_item->id, &task, &list->storage); 
    update_todolist_view(list->view, list);
}

static void set_task_repeat_day(int pos, char day, struct todolist *list)
{
    struct tl_item *task_item;
    struct task task;
    task_item = tl_get_item(pos, &list->tasks);
    task = task_item->data;
    if (day == 0)
        task_unrepeat(&task);
    else
        task_update_repeat_days(day % 7, &task);
    storage_set_task(task_item->id, &task, &list->storage); 
    update_todolist_view(list->view, list);
}

static void cancel_task_repeat(int pos, struct todolist *list)
{
    struct tl_item *task_item;
    struct task task;
    task_item = tl_get_item(pos, &list->tasks);
    task = task_item->data;
    task_unrepeat(&task);
    storage_set_task(task_item->id, &task, &list->storage); 
    update_todolist_view(list->view, list);
}

static void task_info(int pos, struct todolist *list)
{
    struct tl_item *task_item;
    task_item = tl_get_item(pos, &list->tasks);
    show_task_info(&task_item->data);
}

static void project_info(int pos, struct todolist *list)
{
    struct pl_item *project_item;
    project_item = pl_get_item(pos, &list->projects);
    show_project_info(&project_item->data);
}

static void set_project_pos(int pos, int new_pos, struct todolist *list)
{
    struct pl_item *first_project_item, *second_project_item;
    struct project first_project, second_project;
    first_project_item = pl_get_item(pos, &list->projects);
    second_project_item = pl_get_item(new_pos, &list->projects);
    first_project = first_project_item->data;
    second_project = second_project_item->data;
    storage_set_project(first_project_item->id, &second_project, 
                                                        &list->storage); 
    storage_set_project(second_project_item->id, &first_project, 
                                                        &list->storage); 
    update_todolist_view(list->view, list);
}

static void command_add(char **params, int params_cnt, 
                                                struct todolist *list) 
{
    if (params_cnt >= pcnt_add) {
        concat_params(1, params, &params_cnt);
        if (list->view == view_projects)
            add_project(params[1], list);
        else
            add_task(params[1], list);
    }
}

static void command_remove(char **params, int params_cnt, 
                                                    struct todolist *list) 
{
    if (params_cnt >= pcnt_remove) {
        int pos; 
        pos = param_to_num(params[1]) - list_view_start_pos;
        if (list->view == view_projects) {
            remove_tasks_from_project(pos, list);
            remove_project(pos, list);
        } else {
            remove_task(pos, list);
        }
    }
}

static void command_rename(char **params, int params_cnt, 
                                                    struct todolist *list) 
{
    if (params_cnt >= pcnt_set_name) {
        int pos; 
        pos = param_to_num(params[1]) - list_view_start_pos;
        concat_params(2, params, &params_cnt);
        if (list->view == view_projects)
            rename_project(pos, params[2], list);
        else
            rename_task(pos, params[2], list);
    }
}

static void command_set_description(char **params, int params_cnt, 
                                                    struct todolist *list) 
{
    if (params_cnt >= pcnt_set_name) {
        int pos; 
        pos = param_to_num(params[1]) - list_view_start_pos;
        concat_params(2, params, &params_cnt);
        if (list->view == view_projects)
            set_project_description(pos, params[2], list);
        else
            set_task_description(pos, params[2], list);
    }
}

static void command_done(char **params, int params_cnt, 
                                                    struct todolist *list) 
{
    if (params_cnt >= pcnt_done) {
        int pos; 
        pos = param_to_num(params[1]) - list_view_start_pos;
        done_task(pos, list);
    }
}

static void command_move(char **params, int params_cnt, 
                                                    struct todolist *list) 
{
    if (params_cnt >= pcnt_move) {
        int pos; 
        pos = param_to_num(params[1]) - list_view_start_pos;
        if (isdigit(params[2][0])) {
            project_id proj_pos;
            proj_pos = param_to_num(params[2]) - list_view_start_pos;
            move_task_to_project(pos, proj_pos, list);
        } else {
            move_task_to_folder(pos, params[2][0], list);
        }
    }
}

static void command_set_pos(char **params, int params_cnt, 
                                                    struct todolist *list) 
{
    if (params_cnt >= pcnt_set_pos) {
        int pos; 
        pos = param_to_num(params[1]) - list_view_start_pos;
        if (isdigit(params[2][0])) {
            task_id new_pos;
            new_pos = param_to_num(params[2]) - list_view_start_pos;
            if (list->view == view_projects)
                set_project_pos(pos, new_pos, list);
            else
                set_task_pos(pos, new_pos, list);
        }
    }
}

static void command_set_green(char **params, int params_cnt, 
                                                    struct todolist *list) 
{
    if (params_cnt >= pcnt_set_green) {
        int pos; 
        pos = param_to_num(params[1]) - list_view_start_pos;
        if (list->view == view_projects)
            return;
        set_task_green(pos, list);
    }
}

static void command_set_repeat_interval(char **params, int params_cnt, 
                                                        struct todolist *list)
{
    if (params_cnt >= pcnt_set_repeat_interval) {
        int pos; 
        pos = param_to_num(params[1]) - list_view_start_pos;
        if (list->view == view_projects)
            return;
        if (isdigit(params[2][0])) {
            int interval, start_in;
            interval = param_to_num(params[2]);
            start_in = 0;
            if (params_cnt >= 4 && isdigit(params[3][0]))
               start_in = param_to_num(params[3]); 
            set_task_repeat_interval(pos, interval, start_in, list);
        }
    }
}

static void command_set_repeat_day(char **params, int params_cnt, 
                                                    struct todolist *list)
{
    if (params_cnt >= pcnt_set_repeat_day) {
        int pos; 
        pos = param_to_num(params[1]) - list_view_start_pos;
        if (list->view == view_projects)
            return;
        if (isdigit(params[2][0])) {
            int day;
            day = param_to_num(params[2]);
            if (day >= 0 && day <= 7)
                set_task_repeat_day(pos, day, list);
        }
    }
}

static void command_set_unrepeat(char **params, int params_cnt, 
                                                    struct todolist *list)
{
    if (params_cnt >= pcnt_set_unrepeat) {
        int pos; 
        pos = param_to_num(params[1]) - list_view_start_pos;
        if (list->view == view_projects)
            return;
        cancel_task_repeat(pos, list);
    }
}

static void command_show_today_tasks(struct todolist *list)
{
    if (list->view != view_today_tasks)
        update_todolist_view(view_today_tasks, list);
}

static void command_show_all_tasks(struct todolist *list)
{
    if (list->view != view_all_tasks)
        update_todolist_view(view_all_tasks, list);
}

static void command_show_week_tasks(struct todolist *list)
{
    if (list->view != view_week_tasks)
        update_todolist_view(view_week_tasks, list);
}

static void command_show_project(char **params, int params_cnt, 
                                                    struct todolist *list)
{
    if (params_cnt >= pcnt_show_project) {
        if (isdigit(params[1][0])) {
            project_id pos;
            pos = param_to_num(params[1]) - list_view_start_pos;
            if (list->view != view_project_tasks ||
                list->cur_project != pos) 
            {
                list->cur_project = pos;
                update_todolist_view(view_project_tasks, list);
            }
        }
    } else {
        if (list->view != view_projects)
            update_todolist_view(view_projects, list);
    }
} 

static void command_show_completed(char **params, int params_cnt, 
                                                    struct todolist *list)
{
    if (params_cnt >= pcnt_show_completed) {
        if (isdigit(params[1][0])) {
            project_id pos;
            pos = param_to_num(params[1]) - list_view_start_pos;
            if (list->view != view_project_completed_tasks ||
                list->cur_project != pos) 
            {
                list->cur_project = pos;
                update_todolist_view(view_project_completed_tasks, list);
            }
        }
    } else {
        if (list->view != view_completed_tasks)
            update_todolist_view(view_completed_tasks, list);
    }
} 

static void command_show_info(char **params, int params_cnt, 
                                                    struct todolist *list)
{
    if (params_cnt >= pcnt_show_info) {
        if (isdigit(params[1][0])) {
            int pos; 
            pos = param_to_num(params[1]) - list_view_start_pos;
            if (list->view == view_projects) {
                project_info(pos, list);
                update_todolist_view(view_task_info, list);
            } else {
                task_info(pos, list);
                update_todolist_view(view_project_info, list);
            }
        }
    }
} 

static void command_set(char cmd, char **params, int params_cnt, 
                                                    struct todolist *list)
{
    switch (cmd) {
    case c_set_name:
        command_rename(params, params_cnt, list);
    case c_set_description:
        command_set_description(params, params_cnt, list);
        break;
    case c_set_pos:
        command_set_pos(params, params_cnt, list);
        break;
    case c_set_green:
        command_set_green(params, params_cnt, list);
        break;
    case c_set_repeat_interval:
        command_set_repeat_interval(params, params_cnt, list);
        break;
    case c_set_repeat_day:
        command_set_repeat_day(params, params_cnt, list);
        break;
    case c_set_unrepeat:
        command_set_unrepeat(params, params_cnt, list);
        break;
    default:
        break;
    }
}

void todolist_main_loop(struct todolist *list)
{
    char cmd[max_cmd_len];
    char *params[max_params_cnt];
    int params_cnt;
    params_array_init(params, sizeof(params) / sizeof(char *));
    do {
        show_list(list);
        
        show_command_prompt();
        read_command(cmd, max_cmd_len);
        parse_command(cmd, params, &params_cnt); 

        switch (cmd[0]) {
        case c_add:
            command_add(params, params_cnt, list);
            break;
        case c_done:
            command_done(params, params_cnt, list);
            break;
        case c_remove:
            command_remove(params, params_cnt, list);
            break;
        case c_set:
            command_set(cmd[1], params, params_cnt, list);
            break;
        case c_move:
            command_move(params, params_cnt, list);
            break;
        case c_today_tasks:
            command_show_today_tasks(list);
            break;
        case c_all_tasks:
            command_show_all_tasks(list);
            break;
        case c_week_tasks:
            command_show_week_tasks(list);
            break;
        case c_completed_tasks:
            command_show_completed(params, params_cnt, list);
            break;
        case c_project:
            command_show_project(params, params_cnt, list);
            break;
        case c_info:
            command_show_info(params, params_cnt, list);
            break;
        case c_help:
            update_todolist_view(view_help, list);
            show_help(); 
            break;
        case c_quit:
            break;
        default:
            show_help(); 
            break;
        }
    } while (cmd[0] != c_quit);
    params_array_free(params, sizeof(params) / sizeof(char *));
}
