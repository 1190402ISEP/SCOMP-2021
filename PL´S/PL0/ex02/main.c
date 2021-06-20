/*
 * 2. Usando aritmética de apontadores, implemente uma função que recebendo como
parâmetros o endereço de um vector de inteiros, o seu número de elementos e um
determinado valor, subtraia esse valor a todos os elementos do vector.
 */

#include <stdio.h>

int vec[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
int num = 10;
int n = 2;


void sub_n(int *ptr, int num, int n) {

    int i = 0;
    while (i < num) {
        int temp = *(ptr + i);
        *(ptr + i) = temp - n;
        i++;
    }
}


int main() {

    int *ptr = vec;
    sub_n(ptr, num, n);
    int j;
    for (j = 0; j < num; j++) {
        printf("%d\n\n", *(ptr + j));
    }
    return 0;
}



