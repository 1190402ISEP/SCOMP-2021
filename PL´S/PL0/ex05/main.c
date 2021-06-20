/*
 * 5. Usando aritmética de apontadores, implemente uma função que recebendo como
parâmetros o endereço de um vector de inteiros, o seu número de elementos, coloque num segundo vector
 (o terceiro parâmetro da função) o inverso de cada um dos valores do primeiro vector.

 */
#include <stdio.h>
#include <math.h>


int vec[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
int numElems = 10;

float vec2[10];


void put_inv(int *ptr, int numElems, float *ptr2) {

    int i = 0;
    while (i < numElems) {

        float temp = *(ptr);
        *(ptr2) = 1 / temp;
        ptr++;
        ptr2++;
        i++;
    }

};

int main() {

    int i;
    int *ptr = vec;
    float *ptr2 = vec2;

    put_inv(ptr, numElems, ptr2);

    for (i = 0; i < sizeof(vec) / 4; i++) {
        printf("%f\n\n", *ptr2);
        ptr2++;
    }

    return 0;
}
