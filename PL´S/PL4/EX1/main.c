#include "asm.h"

/*
 * 1. Implement a program that creates 8 new processes.
• Each process reads 200 Numbers from a text file (“Numbers.txt”) and write those numbers to the file “Output.txt”;
• At any time only one child should be able to read/write into the files (one process in each of the files);
• At the end, the father process should print the content of the file “Output.txt”.
 */

int spawn_childs(int n);

int main() {

    sem_t *semaforo;
    FILE *input, *output;
    char ch;
    if ((semaforo = sem_open(SEMAPHORE, O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }
    int id = spawn_childs(NUMERO_DE_PROCESSOS_FILHO);
    if (id > 0) {
        sem_wait(semaforo);
        input = fopen(FILE_TO_READ, "r");
        if (input == NULL) {
            printf("Error!");
            exit(1);
        }
        output = fopen(FILE_TO_WRITE, "a");
        char c;
        c = fgetc(input);
        while (c != EOF) {
            fprintf(output, "%c", c);
            c = fgetc(input);
        }
        fclose(input);
        fclose(output);
        sem_post(semaforo);
        exit(0);
    }
int temp = NUMERO_DE_PROCESSOS_FILHO;

    do{
        wait(NULL);
        temp--;
    }while(temp>0);

    sem_unlink(SEMAPHORE);
    sem_close(semaforo);

    output = fopen(FILE_TO_WRITE, "r");

    while ((ch = fgetc(output)) != EOF) {
        printf("%c", ch);
    }
    printf("\n\n");
    return 0;
}

int spawn_childs(int n) {

    pid_t pid;
    int i;
    for (i = 0; i < n; i++) {
        pid = fork();
        if (pid < 0)
            return -1;
        else if (pid == 0)
            return i + 1;
    }
    return 0;
}
