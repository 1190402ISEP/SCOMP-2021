#include "asm.h"

int main() {

    sem_t *sem_east, *sem_ponte;
    if ((sem_east = sem_open("sem_east", O_EXCL, 0644, 0)) == SEM_FAILED ||
        (sem_ponte = sem_open("sem_ponte", O_EXCL, 0644, 1)) == SEM_FAILED) {
        perror("Erro a criar o semaforo.\n");
        exit(0);
    }

    sem_post(sem_east);//unblocked

    int value;
    sem_getvalue(sem_east, &value);

    printf("East: carro %d chegou á\n", value);
    if (value == 1) {
        sem_wait(sem_ponte);//blocked
    }

    printf("East: carro %d a passar pela ponte\n", value);
    sleep(30);

    printf("East: carro %d a sair da ponte\n", value);
    sem_wait(sem_east);//blocked

    sem_getvalue(sem_east, &value);
    if (value == 0) {
        sem_post(sem_ponte);//unblocked
    }
    return 0;
}
