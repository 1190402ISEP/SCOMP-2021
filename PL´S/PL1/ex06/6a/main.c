#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    const int NR_OF_CHILDREN = 4;

    int i;

    for (i = 0; i < NR_OF_CHILDREN; i++)
    {
        if (fork() == 0)
        {
            sleep(1); /*sleep(): unistd.h*/
        }
    }

    printf("This is the end.\n");

    return 0;
}

/*
* a) 15 processos filho e 1 processo pai.
*
*/
