#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <string.h>

#include "server_common.h"
#include "sockets.h"

#define BACKLOG 5

int main()
{
        init_server();
        info server_info;

        unlink(server_socket_path);

        struct sockaddr_un server;
        int fd, flags;

        if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
                perror("socket");
                return -1;
        }
        
        memset(&server, 0, sizeof(server));
        server.sun_family = AF_UNIX;
        strcpy(server.sun_path, server_socket_path);

        if(bind(fd, (struct sockaddr*)&server, sizeof(server)) == -1) {
                perror("bind");
                return -1;
        }

        flags = fcntl(fd, F_GETFL, 0);
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        
        if(listen(fd, BACKLOG) == -1) {
                perror("listen");
                return -1;
        }

        int client_fd;
        while (1) {
                update_info(&server_info);
                if((client_fd = accept(fd, NULL, NULL)) != -1) {
                        write(client_fd, &server_info, sizeof(server_info));
                }
                sleep(1);
        }

        return 0;

