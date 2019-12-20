#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <pthread.h>
#include <unistd.h>

#define ENG 26
#define DELAY 1000000

char alpha[ENG] = "abcdefghijklmnopqrstuvwxyz";

pthread_t t[3];
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

void print_alpha(void) {
	int i;
	for(i = 0; i < ENG; i++) {
		printf("%c", alpha[i]);
	}
	printf("\n");
}

void* print_upcase_count(void* d) {
	(void)d;
	int i, k;
	while(1) {
		pthread_rwlock_wrlock(&rwlock);
		k = 0;
		for(i = 0; i < ENG; i++) {
			if('A' <= alpha[i] && alpha[i] <= 'Z') {
				k++;
			}
		}
		printf("%i\n", k);
		pthread_rwlock_unlock(&rwlock);
		usleep(DELAY);
	}
}

void* reverse(void* d) {
	(void)d;
	int i;
	char t;
	while(1) {
		pthread_rwlock_rdlock(&rwlock);
		pthread_rwlock_wrlock(&rwlock);
		for(i = 0; i < ENG / 2; i++) {
			t = alpha[i];
			alpha[i] = alpha[ENG - i - 1];
			alpha[ENG - i - 1] = t;
		}
		pthread_rwlock_unlock(&rwlock);
		usleep(DELAY);
	}
	return NULL;
}

void* chcase(void* d) {
	(void)d;
	int i;
	while(1) {
		pthread_rwlock_rdlock(&rwlock);
		pthread_rwlock_wrlock(&rwlock);
		for(i = 0; i < ENG; i++) {
			alpha[i] += (alpha[i] - 'A') < ENG ? 32 : -32;
		}
		pthread_rwlock_unlock(&rwlock);
		usleep(DELAY);
	}
	return NULL;
}

int main(void) {
	pthread_create(&t[0], NULL, reverse, NULL);
	pthread_create(&t[1], NULL, chcase, NULL);
	pthread_create(&t[2], NULL, print_upcase_count, NULL);
	while(1) {
		pthread_rwlock_wrlock(&rwlock);
		print_alpha();
		pthread_rwlock_unlock(&rwlock);
		usleep(DELAY);
	}
	return 0;
}
