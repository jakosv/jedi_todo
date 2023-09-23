#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "client.h"

enum { buflen = 4096 };

void client_loop(int fd)
{
    char buf[buflen];
    int res, pid; 

    pid = fork();
    if (pid == 0) {
        while ((res = read(1, buf, buflen)) > 0)
            write(fd, buf, res);
        exit(0);
    }
    while ((res = read(fd, buf, buflen)) > 0)
        write(1, buf, res);
}

void client_connect(const char *server_ip, int server_port)
{
    int ok, s, res;
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(server_port);
    ok = inet_aton(server_ip, &(addr.sin_addr));
    if (!ok) {
        fprintf(stderr, "Invalid ip address: %s\n", server_ip);
        exit(1);
    } 
    
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1) {
        perror("socket");
        exit(1);
    }

    res = connect(s, (struct sockaddr*)&addr, sizeof(addr));
    if (res == -1) {
        perror("connect");
        exit(1);
    }

    client_loop(s);
    close(s);
}
