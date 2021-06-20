#include "asm.h"

void errorDetected(char *message) {
    perror("Erro!!!\n\n");
    exit(EXIT_FAILURE);
}

int main() {
    int entryStop;
    int fileDesc;

    estruturaTrain *train;
    sem_t *openSem;
    sem_t *controlSem;

    if (access("/dev/shm/sem.openSem", R_OK | W_OK) == 0 && access("/dev/shm/sem.controlSem", R_OK | W_OK) == 0 &&
        access("/dev/shm/EX11_SHM", R_OK | W_OK) == 0) {
        if ((openSem = sem_open(SEMAFORO_OPEN, O_CREAT)) == SEM_FAILED) {
            errorDetected("opening \"openSem\" Semaphore.");
        }

        if ((controlSem = sem_open(SEMAFORO_CONTROL, O_CREAT)) == SEM_FAILED) {
            errorDetected("opening \"controlSem\" Semaphore.");
        }

        if ((fileDesc = shm_open(MEMORIA_PARTILHADA, O_CREAT | O_RDWR, 0600)) == -1) {
            errorDetected("opening Shared Memory Block.");
        }

        if (ftruncate(fileDesc, TAMANHO_MEMORIA_PARTILHADA) == -1) {
            errorDetected("allocating space to Shared Memory Block.");
        }

        if ((train = (estruturaTrain *) mmap(NULL, TAMANHO_MEMORIA_PARTILHADA, PROT_READ | PROT_WRITE, MAP_SHARED,
                                             fileDesc, 0)) == NULL) {
            errorDetected("mapping Shared Memory Block.");
        }

        if (train->currentStop != STOPS && train->passengers < CAP) {
            entryStop = train->currentStop;

            if (sem_wait(openSem) == -1) {
                errorDetected("decrementing \"openSem\" Semaphore.");
            }

            if (sem_wait(controlSem) == -1) {
                errorDetected("decrementing \"controlSem\" Semaphore.");
            }

            printf("Passenger has entered the train.\n");

            train->passengers++;

            if (sem_post(controlSem) == -1) {
                errorDetected("incrementing \"controlSem\" Semaphore.");
            }

            if (sem_post(openSem) == -1) {
                errorDetected("incrementing \"openSem\" Semaphore.");
            }

            srand(time(NULL));

            while (train->currentStop != STOPS) {
                if ((rand() % MAX_VAL + MIN_VAL) > EXIT_VAL && entryStop != train->currentStop) {
                    if (sem_wait(openSem) == -1) {
                        errorDetected("decrementing \"openSem\" Semaphore.");
                    }

                    if (sem_wait(controlSem) == -1) {
                        errorDetected("decrementing \"controlSem\" Semaphore.");
                    }

                    printf("Passenger has exited the train.\n");

                    train->passengers--;

                    if (sem_post(controlSem) == -1) {
                        errorDetected("incrementing \"controlSem\" Semaphore.");
                    }

                    if (sem_post(openSem) == -1) {
                        errorDetected("incrementing \"openSem\" Semaphore.");
                    }

                    break;
                }

                sleep(WAIT_TIME_PASSENGER);
            }

            if (train->currentStop == STOPS) {
                if (sem_wait(openSem) == -1) {
                    errorDetected("decrementing \"openSem\" Semaphore.");
                }

                if (sem_wait(controlSem) == -1) {
                    errorDetected("decrementing \"controlSem\" Semaphore.");
                }

                printf("Passenger has exited the train.\n");

                train->passengers--;

                if (sem_post(controlSem) == -1) {
                    errorDetected("incrementing \"controlSem\" Semaphore.");
                }

                if (sem_post(openSem) == -1) {
                    errorDetected("incrementing \"openSem\" Semaphore.");
                }
            }
        }

        if (munmap(train, TAMANHO_MEMORIA_PARTILHADA) == -1) {
            errorDetected("disconnecting Shared Memory Area from the Process Address Space.");
        }

        if (close(fileDesc) == -1) {
            errorDetected("closing File Descriptor.");
        }
    } else {
        perror("Passenger can't board a train that doesn't exist.\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}

