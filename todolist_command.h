#ifndef TODOLIST_COMMAND_H_SENTRY
#define TODOLIST_COMMAND_H_SENTRY

enum {
    max_cmd_len = 200, 
    max_params_cnt = max_cmd_len
};

enum commands {
    c_quit                  = 'q',
    c_add                   = 'a',
    c_remove                = 'r',
    c_done                  = 'd',
    c_move                  = 'm',
    c_set                   = 's',
    c_set_name              = 'n',
    c_set_description       = 'd',
    c_set_green             = 'g',
    c_set_pos               = 'p',
    c_set_repeat_interval   = 'i',
    c_set_repeat_day        = 'r',
    c_set_unrepeat          = 'u',
    c_all_tasks             = 'l',
    c_today_tasks           = 't',
    c_week_tasks            = 'w',
    c_completed_tasks       = 'c',
    c_project               = 'p',
    c_info                  = 'i',
    c_help                  = 'h',
    c_backup                = 'b',
    c_make_backup           = 'm',
    c_load_backup           = 'l'
};

enum command_params_count {
    pcnt_add                 = 2,
    pcnt_remove              = 2,
    pcnt_done                = 2,
    pcnt_move                = 3,
    pcnt_set_name            = 3,
    pcnt_set_pos             = 3,
    pcnt_set_green           = 2,
    pcnt_set_repeat_interval = 3,
    pcnt_set_repeat_day      = 3,
    pcnt_set_unrepeat        = 2,
    pcnt_show_project        = 2,
    pcnt_show_completed      = 2,
    pcnt_show_info           = 2,
    pcnt_backup              = 2
};

void read_command(char *cmd, int len);
void parse_command(const char *cmd, char **params, int *parse_cnt);
void params_array_init(char **params, int size);
void params_array_free(char **params, int size);
void concat_params(int from, char **params, int *params_cnt);
long param_to_num(const char *param);

#endif

