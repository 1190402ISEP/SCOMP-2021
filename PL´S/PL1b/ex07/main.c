/**
*7. Using sigprocmask, sigismember and sigfillset functions implement a function
*that lists all signals blocked when a process receives the SIGUSR1 signal. Are all
*signals blocked? Justify your answer mentioning those who aren’t eventuallyblocked.
*Suggestion: use NULL as value for the second parameter of the sigprocmask
*function.
 */

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void handle_USR1() {
    const int TOTAL_NB_SIG = 32;
    int contador;

    sigset_t sinalBloqueado;

    sigprocmask(SIG_BLOCK, NULL, &sinalBloqueado);
    for (contador = 1; contador < TOTAL_NB_SIG + 1; ++contador) {
        if (sigismember(&sinalBloqueado, contador) == 1) {
            printf("Sinal Bloqueado: #%d - %s\n", contador, strsignal(contador));
            //write(STDOUT_FILENO, strsignal(contador), sizeof(strsignal(contador)));
            //write(STDOUT_FILENO, "\n", sizeof("\n"));

        }
    }
}

int main() {
    struct sigaction action;

    memset(&action, 0, sizeof(struct sigaction));

    sigfillset(&action.sa_mask);
    sigdelset(&action.sa_mask, SIGUSR1);

    action.sa_sigaction = handle_USR1;
    action.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &action, NULL);

    for (;;) {
        printf("Loop\n");

        sleep(1);
    }

    return 0;
}