/**
 * 5. Consider the following code:
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
a. On a terminal compile and run the program. On the same terminal press
*CTRL-C. What happened? For the explanation bear in mind the reception
*of a signal and action performed
*/
/*
 * O comando acima referido envia um sinal para que o processo pare.
 */