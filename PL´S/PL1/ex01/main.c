#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

/**
1. Analyse the following code.
 **/
int main(void) {
    int x = 1;
    pid_t p = fork(); /*pid_t: sys/types.h; fork(): unistd.h*/
    if (p == 0) {
        x = x + 1;
        printf("1. x = %d\n", x);
    } else {
        x = x - 1;
        printf("2. x = %d\n", x);
    }
    printf("3. %d; x = %d\n", p, x);
    return 0;
}

/**
 * a) Assume that the PID of the child process is 1234. What is the output of this code in the terminal? Justify your
answer.
**/

/*
 * O output vai ser um de dois 2,0,2 e 0,1234,0 ou 0,1234,0 e 2,0,2. Ao ser criado o fork p, vai ser criado um novo
 * processo. Esse processo e o seu pai vâo correr os dois até ao fim. Não sabemos é qual dos dois vai correr primeiro.
 * Pode correr primeiro o processo pai,nesse caso vamos ter o segundo output. Pode também correr primeiro o processo
 * filho, nesse caso vamos ter o segundo output.
 */

/**
 * b) Are you always guaranteed to see the output of the printf() in line 9 before the one in line 11? Explain.
**/

/*
 * Sim isso é garantido, pois caso a linha 9 seja executada esta a correr o processo pai, e dentro dum processo a
 * sequência de execução do código é aquela definida pelo programador.
 */