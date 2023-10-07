#include "task_list.h"
#include "task.h"

#include <stdlib.h>
#include <string.h>

void tl_init(struct task_list *lst)
{
    lst->first = NULL;
    lst->last = NULL;
}

static void tl_item_destroy(struct tl_item *item)
{
    if (!item)
        return;
    tl_item_destroy(item->next);
    free(item);
}

void tl_clear(struct task_list *lst)
{
    if (!lst->first)
        return;
    tl_item_destroy(lst->first);
    lst->first = NULL;
    lst->last = NULL;
}

void tl_add(task_id id, struct task *new_task, struct task_list *lst)
{
    struct tl_item *tmp;
    tmp = malloc(sizeof(struct tl_item));
    memcpy(&tmp->data, new_task, sizeof(struct task));
    tmp->next = NULL;
    tmp->prev = NULL;
    tmp->id = id;
    if (lst->last) {
        lst->last->next = tmp;
        tmp->prev = lst->last;
    } else {
        lst->first = tmp;
    }
    lst->last = tmp;
}

struct tl_item *tl_get_item(unsigned pos, const struct task_list *lst)
{
    struct tl_item *tmp;
    unsigned i;
    i = 0;
    tmp = lst->first;
    while (tmp && i != pos) {
        tmp = tmp->next;
        i++;
    }
    return tmp;
}


struct task *tl_get_task(unsigned pos, const struct task_list *lst)
{
    struct tl_item *item;
    item = tl_get_item(pos, lst);
    return item ? &item->data : NULL;
}

int tl_remove(struct tl_item *item, struct task_list *lst)
{
    if (!item)
        return 0;
    if (item->prev)
        item->prev->next = item->next;
    else
        lst->first = item->next;
    if (item->next)
        item->next->prev = item->prev;
    else
        lst->last = item->prev;
    return 1;
}
