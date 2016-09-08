/* Exercicio 4 da lista de pthreads */
/* Compilação: gcc -Wall -pthread -o ex4 ex4_LIPC.c -lrt */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <assert.h>

#define MAX 100

int *n;
sem_t *sema;

void f1(){
    printf("filho\n");
    int i, temp, rc;
    for (i = 0; i < MAX; i++) {
        rc = sem_wait(sema);
        assert(rc == 0);
    	temp = *n;
    	temp++;
    	*n = temp;
        printf("Incrementa %i\n", i);
        rc = sem_post(sema);
        assert(rc == 0);
    }
}

void f2(){
    int i, temp, rc;
    for (i = 0; i < MAX; i++) {
        rc = sem_wait(sema);
        assert(rc == 0);
    	temp = *n;
    	temp--;
    	*n = temp;
        printf("Decrementa %i\n", i);
        rc = sem_post(sema);
        assert(rc == 0);
    }
}

int main(void){
    int rc, fd, status;
    pid_t x;

    fd = shm_open("/memmoria", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if(fd == -1)
        exit(1);
    rc = ftruncate(fd, sizeof(int));
    if( rc == -1 )
        exit(2);
    n = mmap( NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if( n == MAP_FAILED)
        exit(3);

    sema = sem_open("/sema", O_CREAT, 0664, 1);
    if(sema == SEM_FAILED)
        exit(4);

    x = fork();
    if( x == 0 ){
        f1();
        exit(0);
    }else{
        f2();
        waitpid(x, &status, 0);
    }

    printf("n=%d\n", *n);

    rc = munmap( n, sizeof(int));
    if(rc == -1)
        exit(7);
    rc =    shm_unlink("/memmoria");
    if( rc == -1)
        exit(8);
    return 0;
}
