#include "asm.h"

const char name[TOT_STR1][STR_SIZE] = {
        "Agera",
        "Regera",
        "Jesko",
        "Gemera",
        "One to 1",
        "Veyron",
        "Chiron",
        "Atlantic",
        "Gallardo",
        "Jappa",
        "Miura",
        "Centenario",
        "Diablo",
        "Murcielago",
        "Aventador",
        "Countach",
        "Huracan",
        "Veneno",
        "Enzo",
        "Rui",
        "Soares",
        "Antonio",
        "Fernandes"};

const char message[TOT_STR2][STR_SIZE] = {
        " seems to be enjoying the movie.",
        " fell asleep. The seats are just to comfortable.",
        " maybe just laughed too hard. Awkward...",
        " just bought a new bucket of popcorn.",
        " went to the bathroom. Quick the best part of the movie is coming up.",
        " jumped from the seat. Jump scares aren't funny.",
        " keeps texting. Rude."};

int main() {
    if (access("/dev/shm/sem.Sem_Door", R_OK | W_OK) == 0 && access("/dev/shm/ShowRoom", R_OK | W_OK) == 0) {
        sem_t *sem_Door;
        sem_t *sem_Control;

        if ((sem_Door = sem_open("Sem_Door", O_CREAT)) == SEM_FAILED) {
            perror("Error!");
            exit(EXIT_FAILURE);
        }

        if ((sem_Control = sem_open("Sem_Control", O_CREAT | O_EXCL, 0600, 1)) == SEM_FAILED) {
            perror("Error!");
            exit(EXIT_FAILURE);
        }


        ShowRoom *showRoom = openShm("/ShowRoom", sizeof(ShowRoom));

        int count = 1;
        pid_t pid;

        while (showRoom->showEnded == 0) {
            if ((CAP + showRoom->nbVisitors) - CAP * 2 != 0) {

                if ((pid = fork()) == -1) {
                    perror("Error!");
                    exit(EXIT_FAILURE);
                }

                ++count;

                if (pid == 0) {
                    break;
                }

                sleep(1);
            }
        }

        if (pid == 0) {
            time_t tps;

            srand(time(&tps) % getpid());

            int nameIndex = rand() % MAX_NAME + MIN_VAL;

            if (showRoom->showEnded == 0) {
                if (sem_wait(sem_Door) == -1) {
                    perror("Error!");
                    exit(EXIT_FAILURE);
                }

                if (showRoom->showEnded == 1) {
                    if (sem_post(sem_Door) == -1) {
                        perror("Error!");
                        exit(EXIT_FAILURE);
                    }

                    exit(EXIT_SUCCESS);
                }

                if (sem_wait(sem_Control) == -1) {
                    perror("Error!");
                    exit(EXIT_FAILURE);
                }

                showRoom->nbVisitors++;
                showRoom->totalNbVisitors++;

                if (sem_post(sem_Control) == -1) {
                    perror("Error!");
                    exit(EXIT_FAILURE);
                }

                printf("%s has entered the show room.\n", name[nameIndex]);

                while (rand() % MAX_EXIT + MIN_EXIT < EXIT_VAL && showRoom->showEnded == 0) {
                    sleep(rand() % 60 + 10);

                    char str[STR_SIZE];

                    printf(str, "%s %s", name[nameIndex], message[rand() % MAX_MESS + MIN_VAL]);
                    printf("%s\n", str);
                }

                if (sem_wait(sem_Control) == -1) {
                    perror("Error!");
                    exit(EXIT_FAILURE);
                }

                showRoom->nbVisitors--;

                if (sem_post(sem_Control) == -1) {
                    perror("Error!");
                    exit(EXIT_FAILURE);
                }

                if (showRoom->showEnded == 0) {
                    printf("%s seems to not have enjoyed the movie. %s left before the movie even finished.\n",
                           name[nameIndex], name[nameIndex]);

                    if (sem_post(sem_Door) == -1) {
                        perror("Error!");
                        exit(EXIT_FAILURE);
                    }
                } else {
                    printf("The movie ended and %s enjoyed the movie a lot.\n", name[nameIndex]);
                }

                if (munmap(showRoom, sizeof(ShowRoom)) == -1) {
                    perror("Error!");
                    exit(EXIT_FAILURE);
                }

                exit(EXIT_SUCCESS);
            }
        } else {
            int auxCount;

            for (auxCount = 0; auxCount < count; ++auxCount) {
                sleep(1);
            }

            if (munmap(showRoom, sizeof(ShowRoom)) == -1) {
                perror("Error!");
                exit(EXIT_FAILURE);
            }

            if (sem_unlink("Sem_Control") == -1) {
                perror("Error!");
                exit(EXIT_FAILURE);
            }
        }
    } else {
        printf("The show room has open yet please return later.\n");
    }

    return 0;
}

void *openShm(char *shmName, int dataSize) {
    int fileDesc;
    void *ptr;

    if ((fileDesc = shm_open(shmName, O_CREAT | O_RDWR, 0600)) == -1) {
        perror("Error!");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fileDesc, dataSize) == -1) {
        perror("Error!");
        exit(EXIT_FAILURE);
    }

    if ((ptr = mmap(NULL, dataSize, PROT_READ | PROT_WRITE, MAP_SHARED, fileDesc, 0)) == NULL) {
        perror("Error!");
        exit(EXIT_FAILURE);
    }

    if (close(fileDesc) == -1) {
        perror("Error!");
        exit(EXIT_FAILURE);
    }

    return ptr;
}