/**
*2. Write a program that creates a child process and establishes with it a communication channel through a pipe. The
*parent process reads an integer and a string from stdin and sends both through the pipe. The child process should
*read those values from the pipe and print them.
*a.Solve the exercise by first sending an integer and then a string in two separate write operations;
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    const int LEITURA = 0;
    const int ESCRITA = 1;
    const int BUFFER = 100;

    //reads an integer and a string from stdin

    char str_input[BUFFER];
    int i_input = 0;

    int fd[2];

    if (pipe(fd) == -1)
    {
        perror("Pipe Failed");
        return 1;
    }

    pid_t pid = fork();

    if (pid > 0)
    { //Pai
        close(fd[LEITURA]);
        printf("PAI_ Insira o numero:\n");
        scanf("%d", &i_input);
        printf("PAI_ Insira a String:\n");
        scanf("%s", str_input);

        write(fd[ESCRITA], &i_input, sizeof(i_input));
        write(fd[ESCRITA], &str_input, sizeof(str_input));
        close(fd[ESCRITA]);
    }

    if (pid == 0)
    { //Filho

        char str[BUFFER];
        int i = 0;

        close(fd[ESCRITA]);
        read(fd[LEITURA], &i, sizeof(i));
        read(fd[LEITURA], str, sizeof(str));
        close(fd[LEITURA]);
        printf("FILHO_ O número é: %d\n", i);
        printf("FILHO_ A string é: %s\n", str);
        exit(EXIT_SUCCESS);
    }

    return 0;
}
