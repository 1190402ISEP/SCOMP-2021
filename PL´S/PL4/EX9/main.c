#include "asm.h"

void buy_chips() {
    printf("Chips bought!\n");
}

void buy_beer() {
    printf("Beer bought!\n");
}

void eat_and_drink_father() {
    printf("Meal eaten father!\n");
}

void eat_and_drink_son() {
    printf("Meal eaten son!\n");
}

int main() {
    pid_t pid;

    //declaração dos semáforos
    sem_t *chipsSemaphore, *beerSemaphore;

    //abertura dos semaforos
    if ((chipsSemaphore = sem_open(SEMAFORO_C, O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
        perror("Erro no sem_open!\n");
        exit(EXIT_FAILURE);
    }

    if ((beerSemaphore = sem_open(SEMAFORO_B, O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
        perror("Erro no sem_open!\n");
        exit(EXIT_FAILURE);
    }

    if ((pid = fork()) == -1) {
        perror("Erro no fork!\n");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    while (i < 10) {
        if (pid == 0) {//filho (P2)
            sleep(1);

            //compra cerveja
            buy_beer();
            //subimos o chipsSemaphore para o processo p1 compra batatas
            sem_post(chipsSemaphore);
            //descemos o beerSemaphore para que espere que as batatas sejam compradas
            sem_wait(beerSemaphore);
            //apos ter batatas e cerveja come
            eat_and_drink_son();

        } else {//pai (P1)
            sleep(1);

            //compra batatas
            buy_chips();
            //subimos o beerSemaphore para o processo p2 comprar batatas
            sem_post(beerSemaphore);
            //descemos o chipsSemaphore para que espere que as batatas sejam compradas
            sem_wait(chipsSemaphore);
            //apos ter batatas e cerveja come
            eat_and_drink_father();
        }
        i++;
    }

    /*O pai deve esperar que o filho termine,
     * e estes são executados concorrentemente, mas com controlo pelos semaforos
    */
    wait(NULL);

    //Fechar e apagar os semaforos criados
    if (sem_close(beerSemaphore) == -1) {
        perror("Erro no sem_close!\n");
        exit(EXIT_FAILURE);
    }

    if (sem_unlink(SEMAFORO_B) == -1) {
        perror("Erro no sem_unlink!\n");
        exit(EXIT_FAILURE);
    }

    if (sem_close(chipsSemaphore) == -1) {
        perror("Erro no sem_close!\n");
        exit(EXIT_FAILURE);
    }

    if (sem_unlink(SEMAFORO_C) == -1) {
        perror("Erro no sem_unlink!\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
