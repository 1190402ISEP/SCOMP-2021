/*
*8. Write a program that creates 10 child processes to play the game “Win the pipe!”. There must be only one pipe, 
*shared by all processes. The game’s rules are as follow:
*
*
*a. The parent process fills, each 2 seconds, a structure with a message “Win” and the round’s number (1 to 
*10) and writes this data in the pipe;
*
*
*b. Each of child processes is trying to read data from the pipe. The child that succeeds should print the 
*winning message and the round’s number, ending its execution with an exit value equal to the winning 
*round’s number;
*
*
*c. The remaining child processes continue to try to read data from the pipe;
*
*
*d. After all child processes terminate, the parent process should print the PID and the winning round of each 
*child.
*/
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

int spawn_childs(int k)
{
    pid_t pid;
    int i;
    for (i = 0; i < k; i++)
    {
        pid = fork();
        if (pid < 0)
            return -1;
        else if (pid == 0)
            return i + 1;
    }
    return 0;
}

int main(void)
{
    const int LEITURA = 0;
    const int ESCRITA = 1;
    const int BUFFER_SIZE = 50;

    const int NUMBER_OF_CHILDREN = 10;

    struct s1
    {
        char message[4];
        int round_number;
    };

    int fd[2];//descriptor

    if (pipe(fd) == -1)
    {
        perror("Pipe failed");
        return 1;
    }

    int id = spawn_childs(NUMBER_OF_CHILDREN); //create 10 childs

    if (id == 0)//PAI
    {
        close(fd[LEITURA]);
        int i;

        for (i = 0; i < NUMBER_OF_CHILDREN; i++)
        {
            struct s1 s;
            strcpy(s.message, "WIN");
            s.round_number = i + 1;
            sleep(1);
            write(fd[ESCRITA], &s, sizeof(s));
        }

        close(fd[ESCRITA]);
    }

    if (id > 0)
    {//FILHO
        close(fd[ESCRITA]);
        struct s1 s;
        read(fd[LEITURA], &s, BUFFER_SIZE);
        close(fd[LEITURA]);
        printf("%s", s.message);
        printf(" ronda nº:%d\n", s.round_number);
        exit(s.round_number);
    }

    int status;
    int i;
    for (i = 0; i < NUMBER_OF_CHILDREN; i++)
    {
        pid_t pid = wait(&status);
        if (WIFEXITED(status))
        {
            //imprimir para cada filho a ronda que venceu
            int exit_round = WEXITSTATUS(status);
            printf("O filho com o PID %d venceu a ronda nº: %d\n", pid, exit_round);
        }
    }
    return 0;
}