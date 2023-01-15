#ifndef TASK_LIST_H_SENTRY
#define TASK_LIST_H_SENTRY

#include "task.h"

struct tl_item {
    struct task data;
    struct tl_item *next, *prev;
};

struct task_list {
    struct tl_item *first, *last;
};

void tl_init(struct task_list *lst);
void tl_destroy(struct task_list *lst);
void tl_add(struct task *new_task, struct task_list *lst);
struct task *tl_get_by_id(task_id id, struct task_list *lst); 
int tl_remove(unsigned pos, struct task_list *lst);
int tl_remove_by_id(task_id id, struct task_list *lst);

#endif
