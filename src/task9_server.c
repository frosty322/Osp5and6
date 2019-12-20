#define _XOPEN_SOURCE    
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <time.h>
#include <sys/loadavg.h>  /* getloadavg */
#include <string.h>       /* memset */

#include "task.h"

struct server_info srv_info;
time_t start_time;

void sighandler(int signo) {
	int i;
	switch(signo) {
		case SIGHUP:
			printf("%li\n", (long)srv_info.pid);
			break;
		case SIGINT:
			printf("%li\n", (long)srv_info.uid);
			break;
		case SIGTERM:
			printf("%li\n", (long)srv_info.gid);
			break;
		case SIGUSR1:
			printf("%li\n", (long)srv_info.diff);
			break;
		case SIGUSR2:
			for(i = 0; i < LOADAVG_NSTATS; i++) 
				printf("%.6lf ", srv_info.loadavg[i]);
			printf("\n");
			break;
	}
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
	time_t cur_time;
	struct sigaction act;

	start_time = time(NULL);
	srv_info = save_info();

	memset(&act, 0, sizeof(struct sigaction));
	act.sa_handler = sighandler;
	sigaction(SIGHUP, &act, NULL);	
	sigaction(SIGINT, &act, NULL);	
	sigaction(SIGTERM, &act, NULL);	
	sigaction(SIGUSR1, &act, NULL);	
	sigaction(SIGUSR2, &act, NULL);	


	while(1) {
		cur_time = time(NULL);
		assert(cur_time > 0);

		srv_info.diff = cur_time - start_time;
		assert(getloadavg(srv_info.loadavg, LOADAVG_NSTATS));
		sleep(1);
	}

	return 0;
}
