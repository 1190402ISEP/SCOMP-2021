/*
*3. Write a program that creates a child process and establishes with it a communication channel through a pipe. The
*parent process should send two messages, “Hello World” and “Goodbye!”, and then closes the communication
*channel. The child process should wait for data and print it as soon as it is available, terminating after all data is
*received. The parent process must wait for its child to end, printing its PID and exit value.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>

#define FIRST_READ_SIZE 4
#define MESSAGE_SIZE 80

int main()
{
    const int LEITURA = 0;
    const int ESCRITA = 1;

    int fd[2];

    if (pipe(fd) == -1)
    {
        perror("Pipe Failed");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Fork Failed");
        exit(EXIT_FAILURE);
    }

    if (pid > 0)
    { //PAI

        printf("Processo pai tem o PID: %d\n", getpid());
        close(fd[LEITURA]);

        char firstMessage[MESSAGE_SIZE] = "Hello World!";
        char secondMessage[MESSAGE_SIZE] = "Goodbye!";

        write(fd[ESCRITA], &firstMessage, sizeof(firstMessage));
        write(fd[ESCRITA], &secondMessage, sizeof(secondMessage));

        close(fd[ESCRITA]);
    }

    if (pid == 0)
    { //FILHO
        printf("Filho criado com o PID: %d\n", getpid());
        close(fd[ESCRITA]);

        char firstMessage[MESSAGE_SIZE];
        char secondMessage[MESSAGE_SIZE];

        read(fd[LEITURA], firstMessage, sizeof(firstMessage));
        read(fd[LEITURA], secondMessage, sizeof(secondMessage));

        close(fd[LEITURA]);

        printf("Primeira Mensagem: %s\n", firstMessage);
        printf("Segunda  Mensagem: %s\n", secondMessage);

        exit(EXIT_SUCCESS);
    }
    int estado;
    pid_t pidexit = wait(&estado);
    int exitstatus = WEXITSTATUS(estado);

    printf("O processo com o PID: %d terminou com valor: %d \n\n", pidexit, exitstatus);

    exit(EXIT_SUCCESS);
}
