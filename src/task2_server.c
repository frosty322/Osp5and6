#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/loadavg.h>  /* getloadavg */
#include <string.h>       /* memset */
#include <signal.h>
#include <stdlib.h>

#include "task.h"

int msg_id;
struct msg msg;

void sighandler(int signo) {
	(void)signo;
	msgctl(msg_id, IPC_RMID, NULL);	
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
	key_t key;

	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
	act.sa_handler = sighandler;
	sigaction(SIGINT, &act, NULL);

	/* generate ipc key */
	key = KEY;

	/* setup message queue */
	msg_id = msgget(key, IPC_CREAT | 0666);
	if(msg_id < 0) {
		perror("msgget");
		return 1;
	}

	msg.info = save_info();
	msg.type = 1;

	while(1) {
		cur_time = time(NULL);
		assert(cur_time > 0);

		msg.type++;
		msg.info.diff = cur_time - start_time;
		assert(getloadavg(msg.info.loadavg, LOADAVG_NSTATS));
		if(msgsnd(msg_id, &msg, sizeof(msg), 0) < 0) {
			perror("msgsnd");
		}


		sleep(1);
	}

	return 0;
}
