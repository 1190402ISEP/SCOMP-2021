/**
*b. Change the program so that each time a SIGINT signal is received the “I
*won’t let the process end with CTRL-C!” message is displayed. Recompile
*and run the program and test by pressing several times CTRL-C.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>


void sig_handler(int signo) {
    if (signo == SIGINT) {
        printf("\nI won’t let the process end with CTRL-C!\n");
        write(STDOUT_FILENO, "\nCTRL-C!\n", 2);
    }
}

int main() {
    signal(SIGINT, sig_handler);

    for (;;) {
        signal(SIGINT, sig_handler);
        printf("I Love SCOMP!\n");
        sleep(1);
    }
    return 0;
}
/**
 * c. On the terminal where the process was launched in b. press the CTRL-\
keys. What happened? For the explanation bear in mind the reception of
a signal and action performed.
*/
/*
 *Recebe um SIGQUIT e aborta o programa
 */