#ifndef RECORD_H_SENTRY
#define RECORD_H_SENTRY

#include "task.h"
#include "project.h"

typedef unsigned record_pos;

enum record_type { rt_task, rt_project };

struct record {
    record_pos pos;
    enum record_type type;
    union {
        struct task task;
        struct project project; 
    } data; 
    char is_deleted;
};

void record_copy(struct record *dst, const struct record *src);

#endif
