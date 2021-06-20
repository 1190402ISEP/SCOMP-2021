/**6. Given two integer arrays vec1 and vec2, with 1000 elements each, write a program that *creates 5 child processes that communicate 
* with the parent through a pipe, to concurrently sum *the two arrays. 
* Each child should calculate tmp += vec1[i] + vec2[i] on 200 elements, then *sending tmp to its parent through the pipe. 
* Ensure that each child computes different positions of the arrays. The parent process should wait for the 5 partial 
* sums and then calculate the final result. There must be only one pipe, shared by all processes.
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

//create n child processes
int spawn_childs(int n)
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
        {
            return i + 1;
        }
    }
    return 0;
}

int main(void)
{
    const int LEITURA = 0;
    const int ESCRITA = 1;
    const int NR_PROCESSOS = 5;

    const int ARRAY_SIZE = 1000;

    int vec1[ARRAY_SIZE];
    int vec2[ARRAY_SIZE];

    time_t t;

    srand((unsigned)time(&t));

    //iniciar os arrays com números random

    int i;

    for (i = 0; i < ARRAY_SIZE; i++)
    {
        vec1[i] = rand() % (256);
    }

    for (i = 0; i < ARRAY_SIZE; i++)
    {
        vec2[i] = rand() % (256);
    }

    int fd[2];
    if (pipe(fd) == -1)
    {
        perror("Pipe Failed");
        return 1;
    }

    int id = spawn_childs(NR_PROCESSOS);

   if (id == 0)
    {//PAI
        close(fd[ESCRITA]);
        int final = 0;
        int tmp;
        for (i = 0; i < 5; i++)
        {
            int status;
            wait(&status);
            if (WIFEXITED(status))
            {
                read(fd[LEITURA], &tmp, 4);
                final += tmp;
            }
        }
        printf("Final result is %d\n", final);
    }
    if (id > 0)
    {
        close(fd[LEITURA]);
        int tmp = 0;
        int start = id * 200;
        int finish = start + 200;
        int i;
        for (i = start; i < finish; i++)
        {
            tmp = tmp + vec1[i] + vec2[i];
        }
        write(fd[ESCRITA], &tmp, 4);
        close(fd[ESCRITA]);
    }
    return 0;
}

/**
 * As the pipe is shared among all child processes, when a child calculates it's part of the array,
 * it sends it through the pipe to the parent process.
 * Also, each child opens the descriptor to the pipe to read in it in order, making the data on pipe to
 * be the exact array ordered. 
 * In another side, the parent process will wait unitl all processes had finish and then will receive
 * the data in the desirable order, so there's no need for a shyncronization mechanism.
**/