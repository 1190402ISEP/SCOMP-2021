/**
 * 5.Write a program that creates two child processes, the father waits for each one of them to terminate and then
 * outputs their exit value. The first child process created should sleep for 1 second and then exit with a return value
 * of 1. The second child created should sleep for 2 seconds and exit returning 2.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    const int NR_OF_CHILDREN = 2;
    pid_t pids[NR_OF_CHILDREN];

    int i;

    for (i = 0; i < NR_OF_CHILDREN; i++) {
        pids[i] = fork();

        if (pids[i] < 0) {
            perror("Erro ao efetuar o Fork");
            exit(-1);
        }

        if (pids[i] == 0) {
            int timeToSleep = i + 1;
            printf("Filho #%d criado com o PID= %d\nTempo de Espera: %d segundos\n",
                   i, getpid(), timeToSleep);
            sleep(timeToSleep);
            exit(timeToSleep);
        }


    }
    pid_t p;
    int estado;
    int j;
    printf("\n\n\n");

    for (j = 0; j < NR_OF_CHILDREN; j++) {
        p = waitpid(pids[j], &estado, 0);
        if (WIFEXITED(estado)) {//se retornar normalmente
            printf("O filho #%d com o PID %d terminou e retornou: %d\n", j, p, WEXITSTATUS(estado));
        }
    }
    return 0;
}
