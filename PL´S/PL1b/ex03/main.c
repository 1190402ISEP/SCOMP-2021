
/**
 *3. Check the differences between the signal and sigaction functions.
 */

/**
 * a. Which is most suitable to handle signals? Justify your choice.
 */
/*
 * Um sinal é a notificação de um evento enviado a um processo específico.
 * A função signal permite enviar vários tipos de sinais a um determinado processo,
 * porém, é necessário esperar pelo retorno desta para enviar mais sinais.
 * Em contraste, a função sigaction permite que outros sinais cheguem enquanto que o sinal atual ainda é
 * processado, ou seja, necessariamente, não bloqueia os sinais, daí que sigaction seja uma melhor escolha.
 */

/**
 * b. Document with detail all the parameters of sigaction as well as its
 * possible values.
 */
/* int sigaction(intsig,conststructsigaction* act,structsigaction* oact);
 *
 * act sets the action details for signal sig, and
 * oact (if not NULL) is used to store the previously set action details
 *
*/

/**
 * c. Write a program that when the SIGUSR1 signal is received the message “SIGUSR1 has been captured and was sent
 * by the process with PID XX” where XX is the process PID which sent the SIGUSR1 signal.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>

void handle_USR1(int signo, siginfo_t *sinfo, void *context) {
/* Don’t use printf: it is not safe!!! */
/* See man sigaction for list of safe functions. */
    printf("Signal %d sent by process %d\n",
           sinfo->si_signo, sinfo->si_pid);
}

int main() {
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask); /* No signals blocked */
    act.sa_sigaction = handle_USR1;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);
/* SIGUSR1 will now be captured and handled (POSIX). */
/* (...) */
    while (1) {
        printf("Running!\n");
        sleep(4);
    }
    return 0;

}


/*
 *

Two Linux-specific methods are SA_SIGINFO and signalfd(),
which allows programs to receive very detailed information about signals sent, including the sender's PID.
Call sigaction() and pass to it a struct sigaction which has the desired signal handler in sa_sigaction and
the SA_SIGINFO flag in sa_flags set. With this flag, your signal handler will receive three arguments,
one of which is a siginfo_t structure containing the sender's PID and UID.

Call signalfd() and read signalfd_siginfo structures from it (usually in some kind of a select/poll loop).
The contents will be similar to siginfo_t.
Which one to use depends on how your application is written;
 *
 * */

/**
*  d. On the function code which handles the signal is it possible to use global or static variables safely?
 *  Justify your answer.
 */
/* Não, pois isso pode causar que a função deixe de ser recorrente,
* o que pode fazer com que a função passe a ter um comportamento não determínistico.
*/

/**
 * e. Which functions can be called in an adequate manner on a signal
handler? (Sugestion: “man signal-safety”).
 */
/*
 *
 * An async-signal-safe function is one that can be safely called from
 * within a signal handler. Many functions are not async-signal-safe. In
 * particular, nonreentrant functions are generally unsafe to call from a
 * signal handler.
 *
 * In general, a function is async-signal-safe either because it is
 * reentrant or because it is atomic with respect to signals (i.e., its
 * execution can't be interrupted by a signal handler).
 *
 */
