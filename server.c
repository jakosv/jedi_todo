#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "server.h"
#include "todolist.h"

enum { qlen = 16 };

static void client_handler(int fd)
{
    struct todolist *list;

    dup2(fd, 0);
    dup2(fd, 1);
    dup2(fd, 2);

    list = malloc(sizeof(struct todolist));

    todolist_init(list);
    todolist_main_loop(list);
    todolist_destroy(list);

    free(list);
}

static void server_loop(int ls)
{
    for (;;) {
        struct sockaddr_in addr;
        int fd, pid;
        socklen_t addrlen;

        addrlen = sizeof(addr);
        
        fd = accept(ls, (struct sockaddr*)&addr, &addrlen);
        if (fd == -1) {
            perror("accept");
            continue;
        }

        pid = fork();
        if (pid == 0) {
            close(ls);
            client_handler(fd);
            close(fd);
            exit(0);
        }
        close(fd);

        do {
            pid = waitpid(-1, NULL, WNOHANG);
        } while (pid > 0);
    }
}

void server_start(const char *ip, int port)
{
    int s, ok, res;
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    ok = inet_aton(ip, &(addr.sin_addr));
    if (!ok) {
        fprintf(stderr, "Invalid ip address: %s\n", ip);
        exit(1);
    } 

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1) {
        perror("socket");
        exit(1);
    }

    res = bind(s, (struct sockaddr*)&addr, sizeof(addr));
    if (res == -1) {
        perror("bind");
        exit(1);
    }

    res = listen(s, qlen);
    if (res == -1) {
        perror("listen");
        exit(1);
    }

    server_loop(s);

    close(s);
}
