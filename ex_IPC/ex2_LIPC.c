/* Compilar: gcc -Wall -pthread -o ex2 ex2_LIPC.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <assert.h>

int n, goY = 0, goZ = 0;
pthread_cond_t fimX = PTHREAD_COND_INITIALIZER, fimZ = PTHREAD_COND_INITIALIZER;
pthread_mutex_t the_mutex = PTHREAD_MUTEX_INITIALIZER;

void X(void *argp){
	pthread_mutex_lock(&the_mutex);

	n = n * 16;
	printf("X : %i\n", n);
	pthread_cond_signal(&fimX);
	goZ = 1;
	pthread_mutex_unlock(&the_mutex);
	pthread_exit(NULL);
}

void Y(void *argp){
	pthread_mutex_lock(&the_mutex);
	while( !goY )
		pthread_cond_wait(&fimZ, &the_mutex);
	n = n / 7;
	printf("Y : %i\n", n);
	pthread_mutex_unlock(&the_mutex);
	pthread_exit(NULL);
}

void Z(void *argp){
	pthread_mutex_lock(&the_mutex);
	while( !goZ )
		pthread_cond_wait(&fimX, &the_mutex);
	n = n + 40;
	printf("Z : %i\n", n);
	pthread_cond_signal(&fimZ);
	goY = 1;
	pthread_mutex_unlock(&the_mutex);
	pthread_exit(NULL);
}

int main(void){
    pthread_t t1, t2, t3;
    int rc;
    n = 1;
    rc = pthread_create(&t1, NULL, (void *) X, NULL);
    assert(rc == 0);
    rc = pthread_create(&t2, NULL, (void *) Y, NULL);
    assert(rc == 0);
    rc = pthread_create(&t3, NULL, (void *) Z, NULL);
    assert(rc == 0);
    rc = pthread_join(t1, NULL);
    assert(rc == 0);
    rc = pthread_join(t2, NULL);
    assert(rc == 0);
    rc = pthread_join(t3, NULL);
    assert(rc == 0);

    printf("n = %d\n", n);
    return 0;
}