#include "project_list.h"
#include "project.h"

#include <stdlib.h>
#include <string.h>

void pl_init(struct project_list *lst)
{
    lst->first = NULL;
    lst->last = NULL;
}

static void pl_item_destroy(struct pl_item *item)
{
    if (!item)
        return;
    pl_item_destroy(item->next);
    free(item);
}

void pl_clear(struct project_list *lst)
{
    if (!lst->first)
        return;
    pl_item_destroy(lst->first);
    lst->first = NULL;
    lst->last = NULL;
}

void pl_add(project_id id, struct project *new_project, 
                                            struct project_list *lst)
{
    struct pl_item *tmp;
    tmp = malloc(sizeof(struct pl_item));
    memcpy(&tmp->data, new_project, sizeof(struct project));
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

struct pl_item *pl_get_item(unsigned pos, const struct project_list *lst)
{
    struct pl_item *tmp;
    unsigned i;
    i = 0;
    tmp = lst->first;
    while (tmp && i != pos) {
        tmp = tmp->next;
        i++;
    }
    return tmp;
}


struct project *pl_get_project(unsigned pos, 
                                        const struct project_list *lst)
{
    struct pl_item *item;
    item = pl_get_item(pos, lst);
    return item ? &item->data : NULL;
}

int pl_remove(struct pl_item *item, struct project_list *lst)
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
