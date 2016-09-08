/* Exercicio 1 da lista de pthreads */
/* Compilação: gcc -Wall -pthread -o ex1 ex1_LIPC.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#define MAX 100

int n;
pthread_mutex_t the_mutex;

void f1(void *argp)
{
    int i, temp;

    for (i = 0; i < MAX; i++) {
        if(pthread_mutex_lock(&the_mutex) != 0)
            printf("erro\n");
    	temp = n;
    	temp++;
    	n = temp;
        printf("Incrementa %i\n", i);
        if(pthread_mutex_unlock(&the_mutex) != 0)
            printf("Erro\n");
    }
}

void f2(void *argp)
{
    int i, temp;

    for (i = 0; i < MAX; i++) {
        if(pthread_mutex_lock(&the_mutex) != 0)
            printf("erro\n");
    	temp = n;
    	temp--;
    	n = temp;
        printf("Decrementa %i\n", i);
        if(pthread_mutex_unlock(&the_mutex) != 0)
            printf("Erro\n");
    }
}

int main(void)
{
    pthread_t t1, t2;
    int rc;
    //do{
    n = 0;
    pthread_mutex_init(&the_mutex, NULL);
    rc = pthread_create(&t1, NULL, (void *) f1, NULL);
    rc = pthread_create(&t2, NULL, (void *) f2, NULL);
    rc = pthread_join(t1, NULL);
    rc = pthread_join(t2, NULL);
    pthread_mutex_destroy(&the_mutex);
    printf("n=%d\n", n);
    //}while(1);
    return 0;
}

//pthread_mutex_unlock(&the_mutex);
//pthread_mutex_lock(&the_mutex);