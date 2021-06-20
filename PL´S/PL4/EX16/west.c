#include "asm.h"

int main() {
    sem_t *sem_west, *sem_ponte;
    if ((sem_west = sem_open("sem_west", O_EXCL, 0644, 0)) == SEM_FAILED ||
        (sem_ponte = sem_open("sem_ponte", O_EXCL, 0644, 1)) == SEM_FAILED) {
        perror("Erro a criar o semaforo.\n");
        exit(0);
    }

    sem_post(sem_west);//unblocked

    int value;
    sem_getvalue(sem_west, &value);
    printf("West: carro %d chegou á ponte\n", value);

    if (value == 1) {
        sem_wait(sem_ponte); //blocked
    }

    printf("West: carro %d a passar pela ponte\n", value);
    sleep(30);

    printf("West: carro %d a sair da ponte\n", value);
    sem_wait(sem_west); //blocked



    sem_getvalue(sem_west, &value);
    if (value == 0) {
        sem_post(sem_ponte);//unblocked
    }

    return 0;
}
