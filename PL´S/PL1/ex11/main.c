/**
 * 11. Write a program that initializes an array numbers with 1000 random integers in the range [0,255].
 * The program should:
* a) create 5 child processes that will concurrently find the maximum value of 1/5 of the array;
* b) after computing the maximum value of the entire array, the parent process should create one child process
* to perform the following calculation result[i]=((int) numbers[i]/max_value)*100 on half of the
* array and print the result;
* c) the parent process should perform the same calculation on the other half of the array;
* d) both child and parent process must perform the computation concurrently, but the output must be sorted by
* the array indexes.
 **/

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define TAMANHO_DO_ARRAY 1000
#define NUMERO_DE_FILHOS 5

int create_childs(int n)
{
    pid_t pid;
    int i;
    for (i = 0; i < n; i++)
    {
        pid = fork();
        if (pid < 0)
        {
            return -1;
        }
        else if (pid == 0)
            return i + 1;
    }
    return 0;
}

int get_max(int *numbers, int start, int finish)
{
    int max = 0;
    int j;
    for (j = start; j < finish; j++)
    {
        if (numbers[j] > max)
        {
            max = numbers[j];
        }
    }
    return max;
}

int main()
{
    int numbers[TAMANHO_DO_ARRAY];

    time_t t; //for srand

    srand((unsigned)time(&t));

    int i;
    for (i = 0; i < TAMANHO_DO_ARRAY; i++)
    {
        numbers[i] = rand() % (256);
    }
    int id = create_childs(NUMERO_DE_FILHOS);
    //a)
    if (id > 0)
    {
        int min = (id - 1) * 200;
        int max = min + 200;
        exit(get_max(numbers, min, max));
    }

    //b)
    int max = 0;
    for (i = 0; i < NUMERO_DE_FILHOS; i++)
    {
        int status;
        wait(&status);
        if (WIFEXITED(status))
        {
            if (max < WEXITSTATUS(status))
            {
                max = WEXITSTATUS(status);
            }
        }
    }
    int result[TAMANHO_DO_ARRAY];

    pid_t pid = fork();
    int l;
    if (pid == 0)
    {
        for (l = 0; l < TAMANHO_DO_ARRAY / 2; l++)
        {
            result[l] = (((int)numbers[l] * 100) / max);
        }
        exit(0);
    }
    else if (pid == -1)
    {
        perror("Erro ao criar o filho!");
    }
    else
    {
        /*
         * c)
            int status;
            wait(&status);
            if (WIFEXITED(status)) {
                for (l = ARRAY_SIZE / 2; l < ARRAY_SIZE; l++) {
                    result[l] = (((int) numbers[l] * 100) / max);
                    printf("Posição: %d |Valor: %d \n", l, result[l]);
                }
            }
        }
        */

        for (l = TAMANHO_DO_ARRAY / 2; l < TAMANHO_DO_ARRAY; l++)
        {
            result[l] = (((int)numbers[l] * 100) / max);
        }
    }

    int status;
    wait(&status);
    if (WIFEXITED(status))
    {
        for (l = 0; l < TAMANHO_DO_ARRAY; l++)
        {
            printf("Posição: %d |Valor: %d \n", l, result[l]);
        }
    }

    return 0;
}