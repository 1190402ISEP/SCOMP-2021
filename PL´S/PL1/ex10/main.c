/**
 * 10. Write a program that initializes an array of 2 000 random integers. Then, the program should create 10 child
 * processes that will find the first occurrence of a number n and return the relative index where it was found within
 * the section of the array it computed (that is, the index returned is a value in the range [0,200[). If no number is
 * found, the child returns 255. The parent should wait for the child processes to finish and output the valid indexes
 * returned.
 **/

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define ARRAY_SIZE 2000
#define NUMBER_OF_CHILDREN 10

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

int main()
{

    int numbers[ARRAY_SIZE];
    int n; //number to find

    time_t t; //for srand

    ///* intializes RNG (srand():stdlib.h; time(): time.h) */
    srand((unsigned)time(&t));

    /* initialize array with random numbers (rand(): stdlib.h) */
    int i;
    for (i = 0; i < ARRAY_SIZE;
         i++)
    {
        numbers[i] = rand() % 100;
    }
    /* initialize n */
    n = rand() % 100;
    printf("Nº a procurar: %d\n", n);

    int id = create_childs(NUMBER_OF_CHILDREN);

    if (id > 0)
    {
        int start = id * 100;
        int finish = (id + 2) * 100;
        for (i = start; i < finish; i++)
        {
            if (numbers[i] == n)
                exit(i - start);
        }
        exit(255);
    }

    for (i = 0; i < NUMBER_OF_CHILDREN;
         i++)
    {
        int estado;
        pid_t pid = wait(&estado);
        if (WIFEXITED(estado))
        {
            int number = WEXITSTATUS(estado); //vai buscar o valor passado pelo exit
            if (number < 255 && number >= 0)
            {
                printf("|Processo %d | Posição no Array: %d\n", pid, number);
            }
        }
    }

    return 0;
}
