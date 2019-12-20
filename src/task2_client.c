#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "task.h"

int main(void) {
	struct msg msg;
	
	key_t key;
	int msg_id;

	/* generate ipc key */
	key = KEY;

	/* setup message queue */
	msg_id = msgget(key, IPC_CREAT | 0666);
	if(msg_id < 0) {
		perror("msgget");
		return 1;
	}
	
	/* Get msg */
	if(msgrcv(msg_id, &msg, sizeof(msg), 0, 0 < 0) < 0) {
		perror("msgrcv");
		return 1;
	}

	printf("%li\n%li\n%li\n", (long)msg.info.pid, (long)msg.info.uid, (long)msg.info.gid);
	printf("%li\n", (long)msg.info.diff);
	printf("%.6f %.6f %.6f\n", msg.info.loadavg[0], msg.info.loadavg[1], msg.info.loadavg[2]);
	
	return 0;
}
