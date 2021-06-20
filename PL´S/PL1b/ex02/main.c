/**
 * 2. Consider the following code:
**/
#include <stdio.h>
#include <unistd.h>

int main() {
    for (;;) {
        printf("I Love SCOMP!\n");
        sleep(1);
    }
    return 0;
}
/**
 * a) On a terminal, compile and run the program.
 */
/*
 * Para compilar basta executar o comando no diretório do source:
 * gcc -Wall main.c -o main
 *
 * Para correr o programa basta usar o comando:
 * ./main
 */
/**
 * b) Open another terminal and through the kill command send a signal that
 * suspends the process that is displaying the “I Love SCOMP!” message.
 * Check that the process became “stopped”. Which signal made this
 * possible?
 */
/*
 * kill -SIGSTOP 2464 (sendo 2464 o PID)
 */
/**
 * c) Now, send another signal that will allow the previous process to resume
 * its normal execution. Check that the process left the stopped state.
 * Which signal is used?
 */
/*
 * kill -SIGCONT 2464 (sendo 2464 o PID)
 */
/**
 * d) By using signal, end this process.
 */
/*
 * kill -SIGSTOP 2496 (sendo 2496 o PID)
 */
/**
 * e) Write a program that by using the signals primitives is able to send a
 * specific signal to a process. Both values should be read before sending.
 */
/*
 * ./program
 */
/**
 * f) Apply the previously implemented program in e. to replace the kill
 * command and repeat the item on a. and items b. through d.
 */
/*
 *
 */