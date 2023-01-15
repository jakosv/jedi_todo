#include "task_list.h"
#include "task.h"

#include <stdlib.h>

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

void tl_destroy(struct task_list *lst)
{
    if (!lst->first)
        return;
    tl_item_destroy(lst->first);
    lst->first = NULL;
    lst->last = NULL;
}

void tl_add(struct task *new_task, struct task_list *lst)
{
    struct tl_item *tmp;
    tmp = malloc(sizeof(struct tl_item));
    task_copy(&tmp->data, new_task);
    tmp->next = NULL;
    tmp->prev = NULL;
    if (lst->last) {
        lst->last->next = tmp;
        tmp->prev = lst->last;
    } else {
        lst->first = tmp;
    }
    lst->last = tmp;
}

struct task *tl_get_by_id(task_id id, struct task_list *lst)
{
    struct tl_item *tmp;
    tmp = lst->first;
    while (tmp && tmp->data.id != id)
        tmp = tmp->next;
    return tmp ? &tmp->data : NULL;
}


static struct tl_item *search_item(unsigned pos, 
                                        const struct task_list *lst)
{
    struct tl_item *tmp;
    int i;
    tmp = lst->first;
    i = 1;
    while (tmp && i != pos) {
        tmp = tmp->next;
        i++;
    } 
    return tmp;
}

int tl_remove(unsigned pos, struct task_list *lst)
{
    struct tl_item *tmp = search_item(pos, lst);
    if (!tmp)
        return 0;
    if (tmp->prev)
        tmp->prev->next = tmp->next;
    else
        lst->first = tmp->next;
    if (tmp->next)
        tmp->next->prev = tmp->prev;
    else
        lst->last = tmp->prev;
    return 1;
}

int tl_remove_by_id(task_id id, struct task_list *lst)
{
    struct tl_item *tmp;
    unsigned pos;
    pos = 1;
    tmp = lst->first;
    while (tmp && tmp->data.id != id) {
        tmp = tmp->next;
        pos++;
    }
    if (!tmp)
        return 0;
    return tl_remove(pos, lst);
}

