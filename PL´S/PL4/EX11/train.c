#include "asm.h"

void errorDetected(char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

int main() {

    int val;
    int count;
    int fileDesc;

    estruturaTrain *train;
    sem_t *openSem;
    sem_t *controlSem;


    if ((openSem = sem_open(SEMAFORO_OPEN, O_CREAT | O_EXCL, 0600, 0)) == SEM_FAILED) {
        errorDetected("creating \"openSem\" Semaphore.");
    }

    if ((controlSem = sem_open(SEMAFORO_CONTROL, O_CREAT | O_EXCL, 0600, 1)) == SEM_FAILED) {
        errorDetected("creating \"controlSem\" Semaphore.");
    }

    if ((fileDesc = shm_open(MEMORIA_PARTILHADA, O_CREAT | O_EXCL | O_RDWR, 0600)) == -1) {
        errorDetected("creating Shared Memory Block.");
    }

    if (ftruncate(fileDesc, TAMANHO_MEMORIA_PARTILHADA) == -1) {
        errorDetected("allocating space to Shared Memory Block.");
    }

    if ((train = (estruturaTrain *) mmap(NULL, TAMANHO_MEMORIA_PARTILHADA, PROT_READ | PROT_WRITE, MAP_SHARED, fileDesc, 0)) == NULL) {
        errorDetected("mapping Shared Memory Block.");
    }

    if (sem_wait(controlSem) == -1) {
        errorDetected("decrementing \"controlSem\" Semaphore.");
    }

    train->passengers = 0;
    train->currentStop = 0;

    if (sem_post(controlSem) == -1) {
        errorDetected("incrementing \"controlSem\" Semaphore.");
    }

    for (; train->currentStop <= STOPS; train->currentStop++) {
        if (train->currentStop != STOPS) {
            for (count = 0; count < DOORS; count++) {
                if (sem_post(openSem) == -1) {
                    errorDetected("incrementing \"openSem\" Semaphore.");
                }
            }

            sleep(WAIT_TIME_TRAIN);
            printf("Exiting Station #%d\n", train->currentStop);
            sem_getvalue(openSem, &val);

            while (val != 0) {
                if (sem_wait(openSem) == -1) {
                    errorDetected("decrementing \"openSem\" Semaphore.");
                }

                sem_getvalue(openSem, &val);
            }
        } else {
            while (train->passengers != 0) {
                sem_getvalue(openSem, &val);

                if (val != DOORS) {
                    for (count = 0; count < DOORS - val; count++) {
                        if (sem_post(openSem) == -1) {
                            errorDetected("incrementing \"openSem\" Semaphore.");
                        }
                    }
                }
            }
        }
    }

    printf("Train voyage has ended.\n");

    if (sem_unlink(SEMAFORO_OPEN) == -1) {
        errorDetected("removing \"openSem\" Semaphore.");
    }

    if (sem_unlink(SEMAFORO_CONTROL) == -1) {
        errorDetected("removing \"openSem\" Semaphore.");
    }

    if (munmap(train, TAMANHO_MEMORIA_PARTILHADA) == -1) {
        errorDetected("disconnecting Shared Memory Area from the Process Address Space.");
    }

    if (close(fileDesc) == -1) {
        errorDetected("closing File Descriptor.");
    }

    if (shm_unlink(MEMORIA_PARTILHADA) == -1) {
        errorDetected("removing Memory Area from the File System.");
    }

    return 0;
}
