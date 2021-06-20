/**
 * 4. Analyse the following code.
 **/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
    int a = 0, b, c, d;
    b = (int) fork();
    c = (int) getpid(); /* getpid(), getppid(): unistd.h*/
    d = (int) getppid();
    a = a + 5;
    printf("\na=%d, b=%d, c=%d, d=%d\n", a, b, c, d);
    return 0;
}
/**
 * a) Which of the variables a, b, c and d will have the same value in both processes?
 **/
/*
 * O valor de a ficará constante porque a linha se encontra no final e será corrida da mesma forma para cada processo.
 */
/**
 * b) Draw a process tree that describes the processes created.
 **/
/*
 * Anexo.
 */