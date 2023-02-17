#include "todolist_command.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *cmd_name[c_count] = {
    "q",    /* quit             */
    "l",    /* all tasks        */
    "t",    /* today tasks      */
    "w",    /* week tasks       */
    "c",    /* completed tasks  */
    "p",    /* project          */
    "i",    /* info             */
    "h",    /* help             */
    "a",    /* add              */
    "rm",   /* remove           */
    "d",    /* done task        */
    "mv",   /* move             */
    "sn",   /* set name         */
    "sd",   /* set description  */
    "sg",   /* set green        */
    "sp",   /* set position     */
    "ri",   /* repeat interval  */
    "rd",   /* repeat day       */
    "rr",   /* remove repeat    */
    "bm",   /* make backup      */
    "bl"    /* load backup      */
};

void read_command_str(char *cmd_str, int len, char *eof_flag)
{
    int c, i;
    i = 0;
    while ((c = getchar()) != EOF) {
        if (c == '\n' || i >= len - 1)
            break;
        cmd_str[i] = c; 
        i++;
    }
    cmd_str[i] = '\0';
    *eof_flag = c == EOF;
}

void parse_command_str(const char *cmd_str, char **params, int *parse_cnt)
{
    int i, param_len;
    char *param;
    param = malloc(strlen(cmd_str) + 1);
    param_len = 0;
    *parse_cnt = 0;
    for (i = 0; cmd_str[i]; i++) {
        if (cmd_str[i] == ' ' && param_len > 0) {
            param[param_len] = '\0';
            strlcpy(params[*parse_cnt], param, param_len + 1);
            (*parse_cnt)++;
            param_len = 0;
        } else {
            param[param_len] = cmd_str[i];
            param_len++;
        }
    }
    if (param_len > 0) {
        param[param_len] = '\0';
        strlcpy(params[*parse_cnt], param, param_len + 1);
        (*parse_cnt)++;
    }
    free(param);
}

enum commands get_command_by_name(const char *name)
{
    int i;
    for (i = 0; i < c_count; i++)
        if (strcmp(name, cmd_name[i]) == 0)
            return i;
    return c_count;
}

void params_array_init(char **params, int size)
{
    int i;
    for (i = 0; i < size; i++)
        params[i] = malloc(max_cmd_len);
}

void params_array_free(char **params, int size)
{
    int i;
    for (i = 0; i < size; i++)
        free(params[i]);
}

void concat_params(int from, char **params, int *params_cnt)
{
    int i;
    for (i = from + 1; i < *params_cnt; i++) {
        strlcat(params[from], " ", max_cmd_len);
        strlcat(params[from], params[i], max_cmd_len);
    } 
    *params_cnt = from + 1;
}

long param_to_num(const char *param)
{
    long num; 
    char *end;
    num = strtol(param, &end, 10);
    return num;
}
