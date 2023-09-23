#include <stdlib.h>
#include <string.h>

#include "todolist.h"
#include "client.h"
#include "server.h"

static void usage(const char *progname)
{
    printf("usage: %s [-sc] [ip] [port]\n", progname);
}

int main(int argc, char **argv)
{
    if (argc > 3) {
        int port;

        port = atoi(argv[3]);

        if (strcmp(argv[1], "-c") == 0) {
            client_connect(argv[2], port); 
        } else
        if (strcmp(argv[1], "-s") == 0) {
            server_start(argv[2], port); 
        } else {
            usage(argv[0]);
        }
    } else {
        struct todolist list;

        todolist_init(&list);
        todolist_main_loop(&list);
        todolist_destroy(&list);
    }

    return 0;
}
