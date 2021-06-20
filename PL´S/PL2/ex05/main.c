/*
*5. Two processes, client (the child process) and server (the parent process), communicate via *two pipes “Up” and“Down”.
*
*
*Server (parent process)
*^ Up 
*|      |
*       v Down
*Client (child process)
*
*
*The client reads a message from stdin and sends it to the server via the Up pipe, then waits *for the server's answer
*on the Down pipe. The server is specialized in converting characters from lower case to upper *case and vice-versa.
*Therefore, if the client sends the message "lower case" via the Up pipe, the server will read *the message, convert
*it, and send "LOWER CASE" via the Down pipe. When the client receives the message from the *server, it prints it
*out. You may assume the maximum size of any message is 256 bytes.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main()
{
    const int MAX_SIZE = 256;

    const int ESCRITA = 1;
    const int LEITURA = 0;

    int up[2], down[2], i, c = 0;

    char cha;
    pid_t p;

    char str[MAX_SIZE];
    char ltr[MAX_SIZE];
    char finaltr[MAX_SIZE];

    if (pipe(up) == -1)
    {
        printf("Pipe up failed");
        return 1;
    }
    if (pipe(down) == -1)
    {
        printf("Pipe down failed");
        return 1;
    }

    p = fork();

    if (p == 0)
    {
        close(up[LEITURA]);
        printf("Enter string: ");
        scanf("%s", str);
        write(up[ESCRITA], str, strlen(str) + 1);
        close(up[ESCRITA]);
    }
    else
    {
        //ler do teclado
        close(up[ESCRITA]);

        read(up[LEITURA], ltr, MAX_SIZE);


        printf("\n");

        //conversões UP->lower and lower->UP CASE

        while (ltr[c] != '\0')
        {
            cha = ltr[c];
            if (cha >= 'A' && cha <= 'Z')
            {
                ltr[c] = ltr[c] + 32;
            }
            if (cha >= 'a' && cha <= 'z')
            {
                ltr[c] = ltr[c] - 32;
            }
            c++;
        }

        //imprimir string alterada
                    printf("OUTPUT pai:\n");

        for (i = 0; i < strlen(ltr); i++)
        {

            printf("%c", ltr[i]);
        }
        printf("\n");
        close(up[LEITURA]);
    }

    if (p != 0)
    {
        close(down[LEITURA]);
        write(down[ESCRITA], ltr, strlen(ltr) + 1);
        close(down[ESCRITA]);
    }
    else
    {
        close(down[ESCRITA]);
        read(down[LEITURA], finaltr, MAX_SIZE);
        printf("OUTPUT filho:\n");
        for (i = 0; i < strlen(finaltr); i++)
        {
            printf("%c", finaltr[i]);
        }
        printf("\n");
        close(down[LEITURA]);
    }
}