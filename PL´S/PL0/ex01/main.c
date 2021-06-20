/*1. Usando aritmética de apontadores, implemente uma função que recebendo como
parâmetros o endereço de um vector de inteiros e o seu número de elementos,
devolva a soma dos elementos pares do vector.
 */


#include <stdio.h>

int vec[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
int num = 10;


int sum_even(int *ptr, int num) {
    int soma = 0;

    int i = 0;

    while (i < num) {
        if (*(ptr + i) % 2 == 0) {
            soma += *(ptr + i);
        }
        i++;
    }
    return soma;
}


int main() {

    int total = 0;


    int *ptr = vec;
    total = sum_even(ptr, num);

    printf("%d\n\n", total);
    return 0;
}



