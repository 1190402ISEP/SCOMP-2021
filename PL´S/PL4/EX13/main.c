#include "asm.h"

int main() {

    sem_t *sem_Write;
    sem_t *sem_Read;
    sem_t *sem_Access;

    if ((sem_Write = sem_open("Sem_Write", O_CREAT | O_EXCL, 0600, ARR_SIZE)) == SEM_FAILED) {
        perror("Erro ");
        exit(EXIT_FAILURE);
    }

    if ((sem_Read = sem_open("Sem_Read", O_CREAT | O_EXCL, 0600, 0)) == SEM_FAILED) {
        perror("Erro ");
        exit(EXIT_FAILURE);
    }

    if ((sem_Access = sem_open("Sem_Access", O_CREAT | O_EXCL, 0600, 1)) == SEM_FAILED) {
        perror("Erro ");
        exit(EXIT_FAILURE);
    }

    int readIndex = 0;

    int *writeIndex = createShm("/WriteIndex", sizeof(int));
    int *buffer = createShm("/Buffer", sizeof(int) * ARR_SIZE);
    int *nbExchange = createShm("/Exchange", sizeof(int));
    int *prevVal = createShm("/PrevVal", sizeof(int));
    int *maxVal = createShm("/MaxVal", sizeof(int));

    *writeIndex = 0;
    *nbExchange = 0;
    *prevVal = 0;
    *maxVal = INIT_VAL;

    int count;
    int auxCount1;
    int auxCount2;
    pid_t pid;

    for (count = 1; count <= NB_CHILD; ++count) {
        if ((pid = fork()) == -1) {
            perror("Erro ");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            break;
        }
    }
    if (pid == 0) {
        srand(time(NULL));

        while (*nbExchange < NB_EXCHANGE) {
            if (sem_wait(sem_Write) == -1) {
                perror("Erro No filho");
                exit(EXIT_FAILURE);
            }

            if (sem_wait(sem_Access) == -1) {
                perror("Erro No filho");
                exit(EXIT_FAILURE);
            }

            if (*writeIndex == ARR_SIZE) {
                *writeIndex = 0;
            }

            for (auxCount1 = 0; auxCount1 < *writeIndex; ++auxCount1) {
                buffer++;
            }

            while ((*buffer = rand() % *maxVal) <= *prevVal);

            *prevVal = *buffer;
            *maxVal += INC;

            for (auxCount1 = 0; auxCount1 < *writeIndex; ++auxCount1) {
                buffer--;
            }

            (*writeIndex)++;
            (*nbExchange)++;

            if (sem_post(sem_Access) == -1) {
                perror("Erro No filho");
                exit(EXIT_FAILURE);
            }

            if (sem_post(sem_Read) == -1) {
                perror("Erro No filho");
                exit(EXIT_FAILURE);
            }
        }

        if (munmap(buffer, sizeof(int) * ARR_SIZE) == -1) {
            perror("Erro No filho");
            exit(EXIT_FAILURE);
        }

        if (munmap(prevVal, sizeof(int)) == -1) {
            perror("Erro No filho");
            exit(EXIT_FAILURE);
        }

        if (munmap(writeIndex, sizeof(int)) == -1) {
            perror("Erro No filho");
            exit(EXIT_FAILURE);
        }

        if (munmap(nbExchange, sizeof(int)) == -1) {
            perror("Erro No filho");
            exit(EXIT_FAILURE);
        }

        if (munmap(maxVal, sizeof(int)) == -1) {
            perror("Erro No filho");
            exit(EXIT_FAILURE);
        }
    } else {
        creatSigUSR1();

        for (auxCount1 = 0; auxCount1 < NB_EXCHANGE; ++auxCount1) {
            if (sem_wait(sem_Read) == -1) {
                perror("Erro ");
                exit(EXIT_FAILURE);
            }

            if (readIndex == ARR_SIZE) {
                readIndex = 0;
            }

            for (auxCount2 = 0; auxCount2 < readIndex; ++auxCount2) {
                buffer++;
            }

            printf("Value: %d\n", *buffer);

            for (auxCount2 = 0; auxCount2 < readIndex; ++auxCount2) {
                buffer--;
            }

            ++readIndex;

            if (sem_post(sem_Write) == -1) {
                perror("Erro ");
                exit(EXIT_FAILURE);
            }
        }

        if (munmap(buffer, sizeof(int) * ARR_SIZE) == -1) {
            perror("Erro No filho.");
            kill(getppid(), SIGUSR1);
            exit(EXIT_FAILURE);
        }

        if (munmap(prevVal, sizeof(int)) == -1) {
            perror("Erro No filho.");
            kill(getppid(), SIGUSR1);
            exit(EXIT_FAILURE);
        }

        if (munmap(writeIndex, sizeof(int)) == -1) {
            perror("Erro No filho.");
            kill(getppid(), SIGUSR1);
            exit(EXIT_FAILURE);
        }

        if (munmap(maxVal, sizeof(int)) == -1) {
            perror("Erro No filho.");
            kill(getppid(), SIGUSR1);
            exit(EXIT_FAILURE);
        }

        if (munmap(nbExchange, sizeof(int)) == -1) {
            perror("Erro No filho.");
            kill(getppid(), SIGUSR1);
            exit(EXIT_FAILURE);
        }


        if (shm_unlink("/Buffer") == -1) {
            perror("Erro no shm_unlink()");
            exit(EXIT_FAILURE);
        }

        if (shm_unlink("/PrevVal") == -1) {
            perror("Erro no shm_unlink()");
            exit(EXIT_FAILURE);
        }

        if (shm_unlink("/WriteIndex") == -1) {
            perror("Erro no shm_unlink()");
            exit(EXIT_FAILURE);
        }

        if (shm_unlink("/Exchange") == -1) {
            perror("Erro no shm_unlink()");
            exit(EXIT_FAILURE);
        }

        if (shm_unlink("/MaxVal") == -1) {
            perror("Erro no shm_unlink()");
            exit(EXIT_FAILURE);
        }

        if (sem_unlink("Sem_Write") == -1) {
            perror("Erro no sem_unlink()");
            exit(EXIT_FAILURE);
        }

        if (sem_unlink("Sem_Read") == -1) {
            perror("Erro no sem_unlink()");
            exit(EXIT_FAILURE);
        }

        if (sem_unlink("Sem_Access") == -1) {
            perror("Erro no sem_unlink()");
            exit(EXIT_FAILURE);
        }
    }

    return EXIT_SUCCESS;
}


void creatSigUSR1() {
    struct sigaction sig;

    memset(&sig, 0, sizeof(struct sigaction));

    sig.sa_sigaction = handleSIGUSR1;
    sig.sa_flags = SA_SIGINFO;
    sigfillset(&sig.sa_mask);
    sigaction(SIGUSR1, &sig, NULL);
}

void handleSIGUSR1(int sig, siginfo_t *sigInfo, void *context) {
    char message[STR_SIZE];

    sprintf(message, "Child Process with PID %d encountered an error.\n", sigInfo->si_pid);

    write(STDOUT_FILENO, message, strlen(message));

    exit(EXIT_FAILURE);
}

void *createShm(char *shmName, int dataSize) {
    int fileDesc;
    void *ptr;

    if ((fileDesc = shm_open(shmName, O_CREAT | O_EXCL | O_RDWR, 0600)) == -1) {
        perror("Erro ");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fileDesc, dataSize) == -1) {
        perror("Erro ");
        exit(EXIT_FAILURE);
    }

    if ((ptr = mmap(NULL, dataSize, PROT_READ | PROT_WRITE, MAP_SHARED, fileDesc, 0)) == NULL) {
        perror("Erro ");
        exit(EXIT_FAILURE);
    }

    if (close(fileDesc) == -1) {
        perror("Erro ");
        exit(EXIT_FAILURE);
    }

    return ptr;
}
