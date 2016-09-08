/* Exercicio 2 da lista de IPC no Linux */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <assert.h>

int n = 1, okX = 0, okZ = 0;
pthread_cond_t fimX = PTHREAD_COND_INITIALIZER, fimZ = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void X(void *argp)
{
//     sleep(3);
     pthread_mutex_lock(&m);
     n = n*16;
     pthread_cond_signal(&fimX);
     okX = 1;
     pthread_mutex_unlock(&m);
     pthread_exit(NULL);
}

void Y(void *argp)
{
     pthread_mutex_lock(&m);
     while (!okZ) 
	  pthread_cond_wait(&fimZ, &m);
     n = n/7;
     pthread_mutex_unlock(&m);
     pthread_exit(NULL);
}

void Z(void *argp)
{
     pthread_mutex_lock(&m);
     while (!okX) 
	  pthread_cond_wait(&fimX, &m);
     n = n+40;
     pthread_cond_signal(&fimZ);
     okZ = 1;
     pthread_mutex_unlock(&m);
     pthread_exit(NULL);
}

int main(void)
{
     pthread_t t1, t2, t3;
     int rc;
     
     rc = pthread_create(&t1, NULL, (void *) X, NULL); assert(rc == 0);
     rc = pthread_create(&t2, NULL, (void *) Y, NULL); assert(rc == 0);
     rc = pthread_create(&t3, NULL, (void *) Z, NULL); assert(rc == 0);
     rc = pthread_join(t1, NULL); assert(rc == 0);
     rc = pthread_join(t2, NULL); assert(rc == 0);
     rc = pthread_join(t3, NULL); assert(rc == 0);
     printf("n=%d\n", n);
     return 0;
}
