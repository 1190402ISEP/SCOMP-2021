#include "asm.h"

int main() {

    sem_t *semaphore;
    pid_t pid;
    int k = 0;

    if ((semaphore = sem_open(SEMAFORO, O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
        perror("Erro no sem_open()");
        exit(1);
    }
    while (k < NUMERO_DE_MENSAGENS) {//executa 15x
        pid = fork();

        if (pid < 0) {
            perror("Erro no fork()\n\n");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {//Filho ->primeiro a executar!!!
            printf("\n-------------------------\nIteração %d\n", k + 1);
            printf("I’m the child\n");
            sem_post(semaphore); //Permite ao pai avançar
            exit(1);

        } else {//Pai
            sem_wait(semaphore); //Espera pelo filho
            printf("I’m the father\n");
        }
        k++;
    }
    for (int k = 0; k < NUMERO_DE_MENSAGENS; k++) {

    }

    sem_close(semaphore);

    sem_unlink(SEMAFORO);

    return EXIT_SUCCESS;
}