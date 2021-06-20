/**
 * 6. Usando aritmética de apontadores, implemente uma função que recebendo como
 * parâmetro o endereço de duas strings, copia o conteúdo da primeira string para a
 * segunda string.
 */

#include <stdio.h>

void copyString(char *s1, char *s2, int size) {
    while (size > 0) {
        char c = *s1;
        *s2 = c;
        s1++;
        s2++;
        size--;
    }
}

int main() {
    int size = 4;

    char s1[] = "ISEP";
    char s2[size];

    char *ptr1 = s1;
    char *ptr2 = s2;


    copyString(ptr1, ptr2, size);

    ptr1 = s1;
    ptr2 = s2;

    int i;
    for (i = 0; i < size; i++) {
        printf(" %c | %c \n", *(ptr1 + i), *(ptr2 + i));
    }

    return 0;
}