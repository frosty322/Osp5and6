#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#include "task.h"

int main(void) {
	int fd;
	struct server_info *info;

	fd = open(name, O_RDWR);
	if(fd < 0) {
		perror(name);
		return -1;
	}
	info = (struct server_info*)mmap(NULL, sizeof(struct server_info), PROT_READ, MAP_SHARED, fd, 0);
	if(info == MAP_FAILED) {
		perror("mmap");
		return -2;
	}

	printf("%li\n%li\n%li\n", (long)info->pid, (long)info->uid, (long)info->gid);
	printf("%li\n", (long)info->diff);
	printf("%.6f %.6f %.6f\n", info->loadavg[0], info->loadavg[1], info->loadavg[2]);
	
	munmap(info, sizeof(struct server_info));

	return 0;
}
