#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#define NR_PROCESSOS 5

volatile sig_atomic_t sincronizador = 5;

void handle(int sig, siginfo_t *info, void *context) {
    write(STDOUT_FILENO, "Child process has ended.\n", 25); //tem de ser um nr grande por causa do sistema operativo
    sincronizador--;
}

int main() {
    struct sigaction sig;

    memset(&sig, 0, sizeof(struct sigaction));
    sig.sa_sigaction = handle;
    sig.sa_flags = SA_NOCLDWAIT | SA_NOCLDSTOP | SA_SIGINFO;
    sigaction(SIGCHLD, &sig, NULL);

    int count;
    int count2 = 0;
    pid_t pid;

    for (count = 0; count < NR_PROCESSOS; ++count) {
        pid = fork();
        if (pid == 0) {
            break;
        }
        count2++;
    }

    if (pid == 0) {
        count = count2;
        for (count2 = count * 200; count2 <= (count + 5) * 200; count2++) {
            printf("Número: %d\n\n", count2);
        }
        exit(0);

    } else {
        while (sincronizador != 0) {
            printf("Child Number: %d\n", sincronizador);
            pause();
        }

        for (count = 0; count < NR_PROCESSOS; ++count) {
            wait(NULL);
        }
    }

    return 0;
}