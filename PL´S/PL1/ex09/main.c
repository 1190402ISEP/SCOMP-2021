/**
 * 9. Write a program that creates 10 child processes. Each child process should print 100 numbers according to its
 * creation order. That is the first child process created writes numbers 1..100, the second child process 101..200,
 * the third 201..300, and so on. The parent process should wait for all child processes to finish.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>


#define ARRAY_SIZE 10

int main() {
    int i;
    int j;

    pid_t pid[ARRAY_SIZE];

    for (i = 0; i < ARRAY_SIZE; i++) {
        pid[i] = fork();
        if (pid[i] == 0) {
            int primeiro_numero = (100 * i) + 1;
            int ultimo_numero = 100 * (i + 1);
            for (j = primeiro_numero; j < ultimo_numero; j++) {
                printf("%d\n", j);
            }
            printf("\n\n");
            exit(0);
        }
    }

    int estado;
    for (i = 0; i < ARRAY_SIZE; i++) {
        waitpid(pid[i], &estado, 0);
    }

    return 0;
}


/**
 * a) Is the output sorted? Can you guarantee it will be always sorted? Why?
**/
/*
 * Não podemos garantir que os prints serão ordenados porque não sabemos qual é o processo que corre primeiro.
 */