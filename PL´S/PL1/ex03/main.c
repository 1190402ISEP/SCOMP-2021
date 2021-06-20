/**
3. Analyse the following code.
**/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
    fork();
    fork();
    fork();
    printf("SCOMP!\n");
    return 0;
}

/**
 * a) How many processes are created by this code?
**/
/*
 * 7 processes and the father process.
 */
/**
 * b) Draw a process tree that describes the processes created.
 **/
/*
 *
 */
/**
 * c) How many times is “SCOMP” printed?
**/
/*
 * 8 vezes- 1 para o processo pai e 7 para os filhos
 */