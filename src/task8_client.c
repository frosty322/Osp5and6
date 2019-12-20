#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#include "server_info.h"
#include "sockets.h"

int main()
{
        info server_info;

        struct sockaddr_un server;
        int fd;

        if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
                perror("socket");
                return -1;
        }

        memset(&server, 0, sizeof(server));
        server.sun_family = AF_UNIX;
        strcpy(server.sun_path, server_socket_path);

        if(connect(fd, (struct sockaddr*)&server, sizeof(server)) == -1) {
                perror("connect");
                return -1;
        }

        if(read(fd, &server_info, sizeof(server_info)) != sizeof(server_info)) {
                perror("read");
                return -1;
        }

        print_info(&server_info);
        close(fd);
        return 0;
}
