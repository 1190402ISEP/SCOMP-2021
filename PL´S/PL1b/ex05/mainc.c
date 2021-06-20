/**
 * d. Change the program so that each time a SIGQUIT signal is received the “I
*won’t let the process end by pressing CTRL-\!” message is displayed.
*Recompile and run the program and test by pressing several times
*CTRL-\.
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>


void sig_handler(int signo) {
    if (signo == SIGQUIT) {
        printf("\nI won’t let the process end by pressing CTRL-\\!\n");
        write(STDOUT_FILENO, "\nCTRL-\\\n", 3);
    }

    if (signo == SIGINT) {
        printf("\nI won’t let the process end with CTRL-C!\n");
        write(STDOUT_FILENO, "\nCTRL-C!\n", 2);
    }
}

int main() {
    signal(SIGINT, sig_handler);

    for (;;) {
        signal(SIGQUIT, sig_handler);
        printf("I Love SCOMP!\n");
        sleep(1);
    }
    return 0;
}
/**
 * e. On another terminal send, by using the kill command a SIGINT and a
 * SIGQUIT signal to the process created in d. What happened?
*/
/*
*Os sinais são ignorados e o programa continua a avançar:
 *
 * OUTPUT:
 *
 *I Love SCOMP!
 *I Love SCOMP!
 *I won’t let the process end with CTRL-C!
 *I Love SCOMP!
 *I Love SCOMP!
 *I won’t let the process end by pressing CTRL-\!
 *I Love SCOMP!
 */

/**
 * f. Press CTRL-Z to suspend the process. Access the process table to check
 * the state of the process as well as its PID. Use the kill command to kill the
 *suspended process.
 *Sugestion: the “kill %1” can be used. Execute the “jobs” command. What
 *is the meaning of “kill %1”?
*/
/*
 * Após suspender o processo com o ctrl-z o comando kill %1 elimina o processo que foi suspenso.
*/
/**
 * g. On the signal handlers instead of printf the write function should be used. Why is that?
*/
/*
 *System Call's in OS are used in interacting with the OS. E.g. Write() could be used something into the system or
 * into a program.

While Standard Library functions are program specific, E.g. printf() will print something out but it will only be in
 GUI/command line and wont effect system.
 */