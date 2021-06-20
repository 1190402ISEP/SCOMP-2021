/**
 * e) Write a program that by using the signals primitives is able to send a
 * specific signal to a process. Both values should be read before sending.
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int main() {
    int SINAL;
    int pid;

    printf("Insira o PID do Processo:\n");
    scanf("%d", &pid);
    pid_t id = (pid_t) pid;

    printf("Insira o Sinal:\n");
    scanf("%d", &SINAL);

    kill(id, SINAL);

    return 0;
}