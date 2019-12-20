#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define ENG 26
#define THREADS 3
char alpha[ENG] = "abcdefghijklmnopqrstuvwxyz";

pthread_t t[THREADS];
sem_t sem[THREADS];

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
	while(sem_wait(&sem[0]) == 0) {
		for(i = 0; i < ENG / 2; i++) {
			t = alpha[i];
			alpha[i] = alpha[ENG - i - 1];
			alpha[ENG - i - 1] = t;
		}
		sem_post(&sem[2]);
	}
	return NULL;
}

void* chcase(void* d) {
	(void)d;
	int i;
	while(sem_wait(&sem[1]) == 0) {
		for(i = 0; i < ENG; i++) {
			alpha[i] += (alpha[i] - 'A') < ENG ? 32 : -32;
		}
		sem_post(&sem[2]);
	}
	return NULL;
}

int main(void) {
	int i;
	for(i = 0; i < THREADS; i++) {
		if(sem_init(&sem[i], 0, 0) == -1) {
			perror("sem_init");
			return -1;
		}
	}
	pthread_create(&t[0], NULL, reverse, NULL);
	pthread_create(&t[1], NULL, chcase, NULL);
	while(1) {
		i = (i + 1) % (THREADS - 1);
		sem_post(&sem[i]);
		sem_wait(&sem[2]);
		print_alpha();
		sleep(1);
	}
	return 0;
}
