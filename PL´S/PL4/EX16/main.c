#include "asm.h"

int main() {
    sem_t *sem_west, *sem_ponte, *sem_east;

    /*Eliminar semáforos que caso tenha occorido erro não tenham sido eliminados anteriormente*/
    sem_unlink("sem_west");
    sem_unlink("sem_ponte");
    sem_unlink("sem_east");

    if ((sem_ponte = sem_open("sem_ponte", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
        perror("Erro no sem_open().\n");
        exit(0);
    }


    if ((sem_east = sem_open("sem_east", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
        perror("Erro no sem_open().\n");
        exit(EXIT_FAILURE);
    }

    if ((sem_west = sem_open("sem_west", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
        perror("Erro no sem_open().\n");
        exit(0);
    }

    return 0;
}
