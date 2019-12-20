#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "task.h"

int main(void) {
	struct server_info *info;
	
	key_t key;
	int mem_id;

	key = KEY;

	mem_id = shmget(key, sizeof(info), 0);
	
	info = (struct server_info*)shmat(mem_id, NULL, 0);

	printf("%li\n%li\n%li\n", (long)info->pid, (long)info->uid, (long)info->gid);
	printf("%li\n", (long)info->diff);
	printf("%.6f %.6f %.6f\n", info->loadavg[0], info->loadavg[1], info->loadavg[2]);
	
	return 0;
}
