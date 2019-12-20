#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <fcntl.h> 		  /* fcntl */
#include <sys/loadavg.h>  /* getloadavg */
#include <string.h>       /* memset */

#include "task.h"

struct server_info srv_info;
time_t start_time;

struct server_info save_info(void) {
	struct server_info info;
	info.pid = getpid();
	info.uid = getuid();
	info.gid = getgid();
	info.diff = 0;
	assert(getloadavg(info.loadavg, LOADAVG_NSTATS));
	return info;	
}

int main(void) {
	time_t cur_time;
	struct sockaddr_un addr;
	int fd, cl, flags;

	start_time = time(NULL);
	srv_info = save_info();

	/* remove socket file if exists */
	unlink(name);

	/* open socket descriptor */
	if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		return -1;
	}
	
	/* setup socket */
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, name);

	/* bind socket */
	if(bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		perror("bind");
		return -2;
	}

	/* set non-blocking flag */
	flags = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	
	/* set listener */
	if(listen(fd, 5) == -1) {
		perror("listen");
		return -4;
	}

	while(1) {
		cur_time = time(NULL);
		assert(cur_time > 0);

		srv_info.diff = cur_time - start_time;
		assert(getloadavg(srv_info.loadavg, LOADAVG_NSTATS));
		if((cl = accept(fd, NULL, NULL)) != -1) {
			write(cl, &srv_info, sizeof(srv_info));
		}
		sleep(1);
	}

	return 0;
}
