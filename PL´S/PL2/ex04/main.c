/*4. Write a program that creates a child process and establishes with it a communicating *channel through a pipe. The parent process should send the contents of a text file to its *child through the pipe. The child should print all the
*received data. The parent must wait for its child to end.

The client reads a message from stdin and sends it to the server via the Up pipe, then waits for the server's answer
on the Down pipe. The server is specialized in converting characters from lower case to upper case and vice-versa.
Therefore, if the client sends the message "lower case" via the Up pipe, the server will read the message, convert
it, and send "LOWER CASE" via the Down pipe. When the client receives the message from the server, it prints it
out. You may assume the maximum size of any message is 256 bytes.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
    const int LEITURA = 0;
    const int ESCRITA = 1;
    const int BUFFER_SIZE = 100;
    const char *filename = "teste.txt";
    size_t read_s;
    size_t tam = 0;

    char *linha = NULL;
    FILE *fp;

    int fd[2];

    if (pipe(fd) == -1)
    {
        perror("Pipe failed");
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        return 1;
    }

    if (pid > 0)
    {
        close(fd[LEITURA]);
        fp = fopen(filename, "r");
        if (fp == NULL)
            exit(EXIT_FAILURE);
        while ((read_s = getline(&linha, &tam, fp)) != -1)
        {
            write(fd[ESCRITA], linha, read_s);
        }
        fclose(fp);
        close(fd[ESCRITA]);
        if (linha != NULL)
            free(linha);
        wait(NULL);
    }
    else
    {
        close(fd[ESCRITA]);
        char buffer[BUFFER_SIZE];
        int n;
        printf("filho recebeu:\n");
        while ((n = read(fd[LEITURA], buffer, BUFFER_SIZE - 1)))
        {
            buffer[n] = '\0';
            printf("%s", buffer);
        }
        close(fd[LEITURA]);
    }
    printf("\n");
    return 0;
}