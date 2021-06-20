/**
 9. Write a program that creates a process and:
− The parent process executes task A for 3 seconds, presenting the seconds
passed, and afterward sends a SIGUSR1 signal to its child.
 − The child process executes task B for 1 to 5 seconds, and the duration is
randomly generated.
− The child process will then execute task C. This task can only be executed
once there’s a notification that task A has ended.
Suggestion: To guarantee that all works the best way, a global volatile
sig_atomic_t variable type should be used to verify task A execution.
 */

#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>


volatile sig_atomic_t GLOBAL_COUNTER = 0;

void handle_USR1(int signo) {
    GLOBAL_COUNTER++;

    write(STDOUT_FILENO, "\nSIGUSR1 Handle Ativado.\n", 26);
}

void exeTaskA(pid_t pid) {
    struct timespec stamp;

    stamp.tv_sec = 3;
    stamp.tv_nsec = 0000000;

    printf("\n->A correr a Task A\n");

    nanosleep(&stamp, NULL);

    kill(pid, SIGUSR1);
}

void exeTaskB() {
    srand(time(NULL));

    const int VALOR_MAXIMO = 5;

    struct timespec stamp;

    int seconds = rand() % VALOR_MAXIMO + 1;

    stamp.tv_sec = seconds;
    stamp.tv_nsec = 0000000;

    printf("\n->A correr a Task B\n");

    nanosleep(&stamp, NULL);

}

void exeTaskC() {

    printf("\n->A correr a Task C\n");

}

int main() {
    struct sigaction action;

    memset(&action, 0, sizeof(struct sigaction));

    action.sa_sigaction = handle_USR1;
    action.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &action, NULL);

    pid_t pid = fork();

    if (pid > 0) {
        exeTaskA(pid);

        wait(NULL);
    }

    if (pid == 0) {
        exeTaskB();

        if (GLOBAL_COUNTER > 0) {
            exeTaskC();

            exit(EXIT_SUCCESS);
        }
    }

    return 0;
}



