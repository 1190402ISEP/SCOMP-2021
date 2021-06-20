#include "asm.h"

int spawn_childs(int n) {
    pid_t pid;
    int i;
    for (i = 0; i < n; i++) {
        pid = fork();
        if (pid < 0) {
            return -1;
        } else if (pid == 0) {
            return i + 1;
        }
    }
    return 0;
}


int main() {

    int i;
    char semaforosName[TOTAL_SEMAFOROS][80] = {"EX8_S", "EX8_C"};

    sem_t *semaphoreS, *semaphoreC;
/*
 * Necessário devido ao loop infinito até esclarecimento de dúvida !!!MARCADO COMO BUG
 */
    if (sem_unlink(semaforosName[0]) == -1) {
        perror("Erro no sem_unlink()!\n");
        exit(EXIT_FAILURE);
    }
    if (sem_unlink(semaforosName[1]) == -1) {
        perror("Erro no sem_unlink()!\n");
        exit(EXIT_FAILURE);
    }


//------------------------------------------------------------

    if ((semaphoreS = sem_open(semaforosName[0], O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
        perror("Erro no sem_open()!\n");
        exit(EXIT_FAILURE);
    }

    if ((semaphoreC = sem_open(semaforosName[1], O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
        perror("Erro no sem_open()!\n");
        exit(EXIT_FAILURE);
    }


    int numeroFilho = spawn_childs(TOTAL_PROCESSOS);

    if (numeroFilho == 1) {
        while (1) {
            sem_wait(semaphoreS);
            printf("S");
            sem_post(semaphoreC);
        }
        exit(EXIT_SUCCESS);
    }

    if (numeroFilho == 2) {
        while (1) {
            sem_wait(semaphoreC);
            printf("C");
            sem_post(semaphoreS);
        }
        exit(EXIT_SUCCESS);
    }

    for (i = 0; i < TOTAL_PROCESSOS; i++) {
        wait(NULL);
    }


    if (sem_close(semaphoreS) == -1) {
        perror("Erro no sem_close()!\n");
        exit(EXIT_FAILURE);
    }

    if (sem_close(semaphoreC) == -1) {
        perror("Erro no sem_close()!\n");
        exit(EXIT_FAILURE);
    }

    if (sem_unlink(semaforosName[0]) == -1) {
        perror("Erro no sem_unlink()!\n");
        exit(EXIT_FAILURE);
    }
    if (sem_unlink(semaforosName[1]) == -1) {
        perror("Erro no sem_unlink()!\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

