/*
**b. Solve the exercise by using a structure to send both values in one write operation.
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
    struct s1
    {
        int a;
        char palavra[BUFFER - 4];
    };

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
        struct s1 s_pai;

        s_pai.a = i_input;
        strcpy(s_pai.palavra, str_input);

        write(fd[ESCRITA], &s_pai, sizeof(s_pai));
        close(fd[ESCRITA]);
    }

    if (pid == 0)
    { //Filho

        close(fd[ESCRITA]);

        struct s1 s_filho;

        read(fd[LEITURA], &s_filho, sizeof(s_filho));
        close(fd[LEITURA]);

        printf("FILHO_ O número é: %d\n", s_filho.a);
        printf("FILHO_ A string é: %s\n", s_filho.palavra);
        exit(EXIT_SUCCESS);
    }

    return 0;
}
