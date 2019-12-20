#ifndef __SERVER_INFO
#define __SERVER_INFO

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define LOAD_ARGS 3
#define KEY 490542

struct info
{
        pid_t pid;
        uid_t uid;
        gid_t gid;

        time_t t;

        double loadavg[LOAD_ARGS];
};

typedef struct info info;

void print_title();
void print_info_barely(info* server_info);
void print_info(info* server_info);
#endif
