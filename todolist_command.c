#include "todolist_command.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_command(char *cmd, int len)
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

void parse_command(const char *cmd, char **params, int *parse_cnt)
{
    int i, param_len;
    char *param;
    param = malloc(strlen(cmd) + 1);
    param_len = 0;
    *parse_cnt = 0;
    for (i = 0; cmd[i]; i++) {
        if (cmd[i] == ' ' && param_len > 0) {
            param[param_len] = '\0';
            strlcpy(params[*parse_cnt], param, param_len + 1);
            (*parse_cnt)++;
            param_len = 0;
        } else {
            param[param_len] = cmd[i];
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
