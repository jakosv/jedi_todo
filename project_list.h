#ifndef PROJECT_LIST_H_SENTRY
#define PROJECT_LIST_H_SENTRY

#include "project.h"
#include "record.h"

struct pl_item {
    project_id id;
    struct project data;
    struct pl_item *next, *prev;
};

struct project_list {
    struct pl_item *first, *last;
};

void pl_init(struct project_list *lst);
void pl_clear(struct project_list *lst);
void pl_add(project_id id, struct project *new_project, 
                                        struct project_list *lst);
struct pl_item *pl_get_item(unsigned pos, const struct project_list *lst); 
struct project *pl_get_project(unsigned pos, 
                                          const struct project_list *lst); 
int pl_remove(struct pl_item *item, struct project_list *lst);

#endif
