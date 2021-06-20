/**7. Given the code below, write the code necessary to create a child process to find how many times the number n is
 * found in half of the elements of the numbers array. While the child is processing, the parent process should
 * search the other half. After both finish processing their half, the parent computes and presents how many times n
 * was found in the entire array (assume that no number is repeated more than 255 times).
 **/

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>


#define ARRAY_SIZE 1000

int main() {
    int numbers[ARRAY_SIZE]; /* array to lookup */
    int n; /* the number to find */
    time_t t; /* needed to init. the random number generator (RNG)
*/
    int i;

    /* intializes RNG (srand():stdlib.h; time(): time.h) */
    srand((unsigned) time(&t));

    /* initialize array with random numbers (rand(): stdlib.h) */
    for (i = 0; i < ARRAY_SIZE; i++) {
        numbers[i] = rand() % 10000;
    }


    /* initialize n */
    n = rand() % 10000;
    int count = 0;

    pid_t pid = fork();

    if (pid == 0) { //filho
        for (i = 0; i < ARRAY_SIZE / 2; i++) {
            if (numbers[i] == n) {
                count++;
            }
        }
        exit(count);
    }

    for (int i = ARRAY_SIZE / 2; i < ARRAY_SIZE; i++) {
        if (numbers[i] == n) {
            count++;
        }
    }

    int estado; //se retornou sem erros
    waitpid(pid, &estado, 0); //guarda em estado o estado do processo

    if (WIFEXITED(estado)) {//verifica se o processo filho saiu com sucesso
        count += WEXITSTATUS(estado);//vai buscar o valor passado pelo exit
    }
    printf("Total %d\n\n", count);

    return 0;
}
