/*
 * 3. Resolva o exercício 1 de uma forma recursiva.
 */


#include <stdio.h>

int vec[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
int num = 10;

int rec_sum_even(int *ptr, int num) {
    if (num == 0) {
        return 0;
    }

    if (*(ptr) % 2 == 0) {
        return *(ptr) + rec_sum_even(ptr + 1, num - 1);
    }

    return rec_sum_even(ptr + 1, num - 1);
}

void sum_even(int *ptr, int num) {
    printf("%d\n\n", rec_sum_even(ptr, num));
}

int main() {
    int *ptr = vec;
    sum_even(ptr, num);
    return 0;
}



