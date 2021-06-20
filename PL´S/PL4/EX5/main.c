#include "asm.h"

    int main() {
        sem_t *semaphore;
        pid_t pid;

        if ((semaphore = sem_open(SEMAFORO, O_CREAT | O_EXCL, 0644, 0))== SEM_FAILED) {
            perror("Erro no sem_open()");
            exit(1);
        }

        pid= fork();

        if(pid<0){
            perror("Erro no fork()\n\n");
            exit(EXIT_FAILURE);
        }

        if(pid==0){//Filho ->primeiro a executar!!!
            printf("I’m the child\n");
            sem_post(semaphore);

        }else{//Pai
            sem_wait(semaphore); //Espera pelo filho
            printf("I’m the father\n");
        }


        sem_close(semaphore);
        sem_unlink(SEMAFORO);

    return EXIT_SUCCESS;
}