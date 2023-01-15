#include "task.h"

#include <string.h>

void task_copy(struct task *dst, const struct task *src)
{
    int i;
    dst->id = src->id;
    strlcpy(dst->name, src->name, max_task_name_len);
    dst->has_project = src->has_project;
    dst->pid = src->pid;
    dst->date = src->date;
    for (i = 0; i < 7; i++)
        dst->repeat_days[i] = src->repeat_days[i];
    dst->folder = src->folder;
    dst->green = src->green;
}
