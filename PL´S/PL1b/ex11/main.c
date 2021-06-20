#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>


char name[32];
char *cmdName = name;


const int SIZE = 2;

typedef struct {
    char cmd[32];
    int time;
} Comando;

void handleALRM() {
    const int ARR_SIZE = 250;

    char str[ARR_SIZE];

    sprintf(str, "\nO Command %s não terminou em tempo permitido!\n", cmdName);

    write(STDOUT_FILENO, str, sizeof(str));
}

void executeCmds(Comando listCmds[]) {
    int count;
    for (count = 0; count < SIZE; count++) {
        strcpy(cmdName, listCmds[count].cmd);

        execlp(listCmds[count].cmd, listCmds[count].cmd, (char *) NULL);
        sleep(listCmds[count].time);
        printf("\n>>Executing: %s\n", cmdName);
        pid_t pid = fork();

        if (pid == 0) {


            system(cmdName);
            printf("\n>>Execution ended!\n\n");
        }

        if (pid > 0) {
            alarm(listCmds[count].time);
        }
    }
}

int main() {
    struct sigaction action;

    memset(&action, 0, sizeof(struct sigaction));

    action.sa_sigaction = handleALRM;
    action.sa_flags = SA_SIGINFO;
    sigaction(SIGALRM, &action, NULL);

    Comando e[SIZE];

    for (int i = 0; i < SIZE; i++) {
        strcpy(e[i].cmd, "echo alo");
        e[i].time = 5;
    }

    executeCmds(e);

    return 0;
}
