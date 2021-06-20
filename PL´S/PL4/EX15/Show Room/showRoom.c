#include "asm.h"

const char message[TOT_STR][STR_SIZE] = {
"The movie is about to begin.",
"Someone dropped popcorn on the ground.",
"Everybody cheered for the main character.",
"An unexpected plot twist shocked the audience.",
"A noisy teenager keeps talking during the movie.",
"The movie has ended and multiple positive reviews are flooding IMDB.",
"Many patrons left before the movie even ended. The reviews might not be positive for this movie.",};

int main() {
    sem_t *sem_Door;

    if ((sem_Door = sem_open("Sem_Door", O_CREAT | O_EXCL, 0600, CAP)) == SEM_FAILED) {
        perror("Error!");
        exit(EXIT_FAILURE);
    }

    ShowRoom *showRoom = createShm("/ShowRoom", sizeof(ShowRoom));

    showRoom->nbVisitors = 0;
    showRoom->totalNbVisitors = 0;
    showRoom->showEnded = 0;

    int duration;
    int prevVal = 0;
    char buf[STR_SIZE];
    regex_t regex;

    if (regcomp(&regex, "^[0-9]*\n$", 0) != 0) {
        perror("Error!");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    printf("Please insert the duration (in minutes) of the show: ");

    if (fgets(buf, sizeof(buf), stdin) != NULL) {
        if (regexec(&regex, buf, 0, NULL, 0) == 0) {
            duration = atoi(buf) * 60;
        } else {
            perror("Error!");
            exit(EXIT_FAILURE);
        }
    }

    time_t closeTime = time(0) + duration;

    printf("\n%s\n", message[0]);

    while (time(NULL) < closeTime) {
        int index;

        while ((index = rand() % MAX_VAL + MIN_VAL) == prevVal);

        sleep(20);
        printf("%s\n", message[index]);

        prevVal = index;
    }

    showRoom->showEnded = 1;

    int val;
    int count = 0;
    int nbVisEnd = showRoom->nbVisitors;

    sem_getvalue(sem_Door, &val);

    for (; count < nbVisEnd - val; ++count) {
        if (sem_post(sem_Door) == -1) {
            perror("Error!");
            exit(EXIT_FAILURE);
        }
    }

    while (showRoom->nbVisitors != 0);

    if (showRoom->totalNbVisitors <= nbVisEnd + MAX_EXIT) {
        printf("%s\n", message[5]);
    } else {
        printf("%s\n", message[6]);
    }

    if (munmap(showRoom, sizeof(ShowRoom)) == -1) {
        perror("Error!");
        exit(EXIT_FAILURE);
    }


    if (shm_unlink("/ShowRoom") == -1) {
        perror("Error!");
        exit(EXIT_FAILURE);
    }


    if (sem_unlink("Sem_Door") == -1) {
        perror("Error!");
        exit(EXIT_FAILURE);
    }

    return 0;
}

void *createShm(char *shmName, int dataSize) {
    int fileDesc;
    void *ptr;

    if ((fileDesc = shm_open(shmName, O_CREAT | O_EXCL | O_RDWR, 0600)) == -1) {
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
