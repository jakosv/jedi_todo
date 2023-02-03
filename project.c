#include "project.h"

#include <string.h>


void project_init(struct project *new_project)
{
    memset(new_project->name, 0, max_project_name_len);
}

void project_create(const char *name, struct project *new_project)
{
    project_init(new_project);
    strlcpy(new_project->name, name, max_project_name_len);
}
