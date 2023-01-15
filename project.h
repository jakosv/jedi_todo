#ifndef PROJECT_H_SENTRY
#define PROJECT_H_SENTRY

typedef unsigned project_id;

struct project {
    project_id id;
    char name[100];
};

#endif
