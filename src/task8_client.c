#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include "task.h"

int main(void) {
	struct sockaddr_un addr;
	int fd;
	struct server_info info;

	if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		return -1;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, name);

	if(connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		perror("connect");
		return -2;
	}

	if(read(fd, &info, sizeof(info)) != sizeof(info)) {
		printf("Read error!\n");
		return -3;
	}
	
	printf("%li\n%li\n%li\n", (long)info.pid, (long)info.uid, (long)info.gid);
	printf("%li\n", (long)info.diff);
	printf("%.6f %.6f %.6f\n", info.loadavg[0], info.loadavg[1], info.loadavg[2]);
	close(fd);

	return 0;
}
