#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/loadavg.h>  /* getloadavg */
#include <string.h>       /* memset */
#include <signal.h>
#include <stdlib.h>

#include "task.h"

key_t key;
int mem_id;
struct server_info *info;

void sighandler(int signo) {
	(void)signo;
	shmctl(mem_id, IPC_RMID, NULL);	
	free(info);
	_exit(0);	
}

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
	time_t start_time = time(NULL);

	time_t cur_time;

	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
	act.sa_handler = sighandler;
	sigaction(SIGINT, &act, NULL);

	key = KEY;

	mem_id = shmget(key, sizeof(info), IPC_CREAT | 0644);
	if(mem_id < 0) {
		perror("shmget");
		return 1;
	}
	info = (struct server_info*)shmat(mem_id, NULL, 0);
	if(info == NULL) {
		perror("shmat");
		return 1;
	}

	*info = save_info();

	while(1) {
		cur_time = time(NULL);
		assert(cur_time > 0);

		info->diff = cur_time - start_time;
		assert(getloadavg(info->loadavg, LOADAVG_NSTATS));
		sleep(1);
	}

	return 0;
}
