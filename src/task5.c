#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <pthread.h>
#include <unistd.h>

#include "task.h"

#define ENG 26
#define THREADS 2
char alpha[ENG] = "abcdefghijklmnopqrstuvwxyz";

pthread_t t[THREADS];
key_t key = KEY;
int semid;

void print_alpha(void) {
	int i;
	for(i = 0; i < ENG; i++) {
		printf("%c", alpha[i]);
	}
	printf("\n");
}

void* reverse(void* d) {
	(void)d;
	int i;
	char t;
	struct sembuf lock = {0, -1, SEM_UNDO};
	struct sembuf unlock = {2, 1, SEM_UNDO}; 
	while(semop(semid, &lock, 1) != -1) {
		for(i = 0; i < ENG / 2; i++) {
			t = alpha[i];
			alpha[i] = alpha[ENG - i - 1];
			alpha[ENG - i - 1] = t;
		}
		semop(semid, &unlock, 1);
	}
	return NULL;
}

void* chcase(void* d) {
	(void)d;
	int i;
	struct sembuf lock = {1, -1, SEM_UNDO};
	struct sembuf unlock = {2, 1, SEM_UNDO}; 
	while(semop(semid, &lock, 1) != -1) {
		for(i = 0; i < ENG; i++) {
			alpha[i] += (alpha[i] - 'A') < ENG ? 32 : -32;
		}
		semop(semid, &unlock, 1);
	}
	return NULL;
}

int sem_init[] = {0, 0, 0};

int main(void) {
	struct sembuf buf = {0, 1, SEM_UNDO};
	struct sembuf sem_print = {2, -1, SEM_UNDO};

	key = KEY;
	/* Create 3 semaphores and get their id*/
	semid = semget(key, 3, IPC_CREAT | 0600);
	/* Init all semaphores */
	semctl(semid, 0, GETALL, sem_init);

	pthread_create(&t[0], NULL, reverse, NULL);
	pthread_create(&t[1], NULL, chcase, NULL);
	while(1) {
		buf.sem_num = (buf.sem_num + 1) % THREADS;
		semop(semid, &buf, 1);
		semop(semid, &sem_print, 1);
		print_alpha();
		sleep(1);
	}
	return 0;
}
