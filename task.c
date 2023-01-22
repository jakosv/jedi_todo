#include "task.h"

#include "string.h"

void task_create(const char *name, enum task_folder folder,
                                                struct task *new_task)
{
    int i;
    strlcpy(new_task->name, name, max_task_name_len); 
    new_task->has_project = 0;
    new_task->pid = 0;
    new_task->date = 0;
    for (i = 0; i < 7; i++)
        new_task->repeat_days[i] = 0;
    new_task->folder = folder;
    new_task->green = 0;
    new_task->done = 0;
}
