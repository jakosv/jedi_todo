#include "todolist.h"


int main()
{
    struct todolist list;

    todolist_init(&list);

    todolist_main_loop(&list);

    todolist_destroy(&list);

    return 0;
}
