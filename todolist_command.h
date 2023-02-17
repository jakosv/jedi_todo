#ifndef TODOLIST_COMMAND_H_SENTRY
#define TODOLIST_COMMAND_H_SENTRY

enum {
    max_cmd_len = 200, 
    max_params_cnt = max_cmd_len
};

enum commands {
    c_quit,
    c_all_tasks,
    c_today_tasks,
    c_week_tasks,
    c_completed_tasks,
    c_project,
    c_info,
    c_help,
    c_add,
    c_remove,
    c_done,
    c_move,
    c_set_name,
    c_set_description,
    c_set_green,
    c_set_pos,
    c_repeat_interval,
    c_repeat_day,
    c_repeat_remove,
    c_make_backup,
    c_load_backup,
    c_count
};

extern const char *cmd_name[c_count];

enum command_params_count {
    pcnt_add                 = 2,
    pcnt_remove              = 2,
    pcnt_done                = 2,
    pcnt_move                = 3,
    pcnt_set_name            = 3,
    pcnt_set_pos             = 3,
    pcnt_set_green           = 2,
    pcnt_repeat_interval     = 3,
    pcnt_repeat_day          = 3,
    pcnt_repeat_remove       = 2,
    pcnt_show_project        = 2,
    pcnt_show_completed      = 2,
    pcnt_show_info           = 2,
    pcnt_backup              = 2
};

void read_command_str(char *cmd_str, int len, char *eof_flag);
void parse_command_str(const char *cmd_str, char **params, int *parse_cnt);
enum commands get_command_by_name(const char *cmd_name);
void params_array_init(char **params, int size);
void params_array_free(char **params, int size);
void concat_params(int from, char **params, int *params_cnt);
long param_to_num(const char *param);

#endif
