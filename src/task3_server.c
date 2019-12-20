#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <sys/loadavg.h>  /* getloadavg */
#include <string.h>       /* memset */

#include "task.h"


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
	struct server_info *srv_info;
	time_t start_time = time(NULL);

	time_t cur_time;
	int fd;

	/* remove socket file if exists */
	unlink(name);

	/* create ipc file */
	fd = open(name, O_RDWR | O_CREAT, 0644);
	if(fd < 0) {
		perror(name);
		return -1;
	}
	/* memory-mapping */
	srv_info = (struct server_info*)mmap(NULL, sizeof(struct server_info), PROT_READ| PROT_WRITE, MAP_SHARED, fd, 0);
	if(srv_info == MAP_FAILED) {
		perror("mmap");
		return -2;
	}

	/* Prevent SIGBUS */
	truncate(name, sizeof(struct server_info));

	*srv_info = save_info();

	while(1) {
		cur_time = time(NULL);
		assert(cur_time > 0);

		srv_info->diff = cur_time - start_time;
		assert(getloadavg(srv_info->loadavg, LOADAVG_NSTATS));
		sleep(1);
	}

	return 0;
}
