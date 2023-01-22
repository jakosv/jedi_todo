#ifndef TASK_LIST_H_SENTRY
#define TASK_LIST_H_SENTRY

#include "task.h"
#include "record.h"

typedef record_pos task_id;

struct tl_item {
    task_id id;
    struct task data;
    struct tl_item *next, *prev;
};

struct task_list {
    struct tl_item *first, *last;
};

void tl_init(struct task_list *lst);
void tl_clear(struct task_list *lst);
void tl_add(task_id id, struct task *new_task, struct task_list *lst);
struct tl_item *tl_get_item(unsigned pos, const struct task_list *lst); 
struct task *tl_get_task(unsigned pos, const struct task_list *lst); 
int tl_remove(struct tl_item *item, struct task_list *lst);

#endif
