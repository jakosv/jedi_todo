#ifndef PROJECT_H_SENTRY
#define PROJECT_H_SENTRY

typedef unsigned project_id;

enum { 
    max_project_name_len = 81,
    max_project_descript_len = 100
};

struct project {
    char name[max_project_name_len];
    char description[max_project_descript_len];
};

void project_init(struct project *new_project);
void project_create(const char *name, struct project *new_project);

#endif
