#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <pthread.h>
#include <unistd.h>

#define ENG 26
#define DELAY 1000000

char alpha[ENG] = "abcdefghijklmnopqrstuvwxyz";

pthread_t t[2];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

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
	while(1) {
		pthread_mutex_lock(&mutex);
		for(i = 0; i < ENG / 2; i++) {
			t = alpha[i];
			alpha[i] = alpha[ENG - i - 1];
			alpha[ENG - i - 1] = t;
		}
		pthread_mutex_unlock(&mutex);
		usleep(DELAY);
	}
	return NULL;
}

void* chcase(void* d) {
	(void)d;
	int i;
	while(1) {
		pthread_mutex_lock(&mutex);
		for(i = 0; i < ENG; i++) {
			alpha[i] += (alpha[i] - 'A') < ENG ? 32 : -32;
		}
		pthread_mutex_unlock(&mutex);
		usleep(DELAY);
	}
	return NULL;
}

int sem_init[] = {0, 0, 0};

int main(void) {
	pthread_create(&t[0], NULL, reverse, NULL);
	pthread_create(&t[1], NULL, chcase, NULL);
	while(1) {
		pthread_mutex_lock(&mutex);
		print_alpha();
		pthread_mutex_unlock(&mutex);
		usleep(DELAY);
	}
	return 0;
}
