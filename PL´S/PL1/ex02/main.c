/**
Write a program that creates the process tree below and where each process executes the printf() calls as
depicted.
 **/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    printf("I'm...\n");
    pid_t p = fork();

    if (p > 0) {
        printf("the..\n");
        p = fork();
    }

    if (p > 0) {
        printf("father!\n");
        p = fork();
    }

    if (p == 0) {
        printf("i will never join you\n");
    }
    return 0;
}