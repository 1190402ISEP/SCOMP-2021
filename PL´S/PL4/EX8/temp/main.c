#include "asm.h"

int main() {
    int valueSemS, valueSemC;

    valueSemS = 0;
    valueSemC = 1;

    sem_t *semaphoreS, *semaphoreC;

    //Inicialização dos Semáforos

    if ((semaphoreC = sem_open(SEMAFORO_C, O_CREAT, 0644, valueSemC)) == SEM_FAILED) {
        perror("Erro no sem_open()!!\n");
        exit(EXIT_FAILURE);
    }

    if ((semaphoreS = sem_open(SEMAFORO_S, O_CREAT, 0644, valueSemS)) == SEM_FAILED) {
        perror("Erro no sem_open()!!\n");
        exit(EXIT_FAILURE);
    }


/**
* Análise:
*
* Vamos ter que ter 2 semáforos um que controle o processo 1 que imprime S e um que controle o processo 2 que imprime C.
*
*    PROCESSO 1:
*        sem_wait(semaforo dos S)
*        imprime S
*        sem_post(semaforo dos C)
*
*    PROCESSO 2:
*        sem_wait(semaforo dos C)
*        imprime C
*        sem_post(semaforo dos S)
 */

    //Criar o filho

    pid_t pid = fork();
    if (pid < 0) {
        perror("Erro no fork()!!\n\n");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {//FILHO

        while (1) {
            sem_wait(semaphoreS);
            printf("S");
            sem_post(semaphoreC);
        }

    } else {//PAI

        while (1) {
            sem_wait(semaphoreC);
            printf("C");
            sem_post(semaphoreS);
        }
    }

    if (sem_unlink(SEMAFORO_C) == -1) {
        perror("Erro no sem_unlink()!\n");
        exit(EXIT_FAILURE);
    }

    if (sem_unlink(SEMAFORO_S) == -1) {
        perror("Erro no sem_unlink()!\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}