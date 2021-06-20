/*
 * 4. Resolva o exercício 2 de uma forma recursiva.
 */
#include <stdio.h>

int vec[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
int num = 10;
int n = 2;


void sub_n(int *ptr, int num, int n) {

    if (num == 0) {
        return;
    }
    *ptr = (*ptr - n);
    sub_n(ptr + 1, num - 1, n);
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



