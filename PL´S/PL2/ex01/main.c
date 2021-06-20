/**
*1. Write a program that creates a child process and establishes with it a communication channel through a pipe. The 
*parent process starts by printing its PID and then sends it to its child through the pipe. The child process should 
*read the parent´s PID from the pipe and print it. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    const int READ = 0;
    const int WRITE = 1;

    int fd[2];

    if (pipe(fd) == -1)
    {
        perror("Erro ao Criar Pipe");
        return 1;
    }
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("Erro ao efetuar Fork");
        return 1;
    }

    if (pid > 0)
    { //PAI
        close(fd[READ]);
        pid_t pid_p = getpid();
        printf("Pai: %d\n", pid_p);
        write(fd[WRITE], &pid_p, sizeof(pid_p));
        close(fd[WRITE]);
    }

    if (pid == 0) //FILHO
    {
        close(fd[WRITE]);
        pid_t pid_pai = 0;
        read(fd[READ], &pid_pai, sizeof(pid_pai));
        close(fd[READ]);
        printf("Valor Lido Através do Pipe: %d\n\n", pid_pai);
    }
    return 0;
}