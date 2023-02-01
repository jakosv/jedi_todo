#ifndef PROJECT_H_SENTRY
#define PROJECT_H_SENTRY

typedef unsigned project_id;

enum { max_project_name_len = 101 };

struct project {
    project_id id;
    char name[100];
};

void project_init(struct project *new_project);
void project_create(const char *name, struct project *new_project);

#endif
