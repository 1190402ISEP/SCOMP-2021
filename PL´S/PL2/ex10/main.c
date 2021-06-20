/*10. Write a program that simulates a betting system. Assume that the child process starts the game with a credit of 20
*euros. The game has the following rules:
*
*
*a.The parent process generates a random number between 1 and 5;
*
*
*b. Then, it writes 1 in the pipe, notifying the child that it can make another bet, or 0, if the child’s credit ended;
*
*
*c.The child process reads the number sent by the parent process and makes a bet or terminates, accordingly.
*To make a bet, the child should generate a random number between 1 and 5 and send it to the parent process;
*
*
*d. The parent process waits for the child’s bet or by its termination, accordingly. If the child’s bet is right, its
*credit grows 10 euros. If the bet is wrong, its credit diminishes 5 euros;
*
*
*e.The parent process sends the new credit’s value to the child process. The child process prints it.
*/

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{

    const int LEITURA = 0;
    const int ESCRITA = 1;
    const int BUFFER_SIZE = sizeof(int);

    const int CREDITO_INICIAL = 20;

    //CONTROLO:

    const int CONTINUAR = 1;

    const int TERMINAR = 0;

    //Ganhar ou perder crédito

    const int GANHAR_CREDITO = 10;

    const int PERDER_CREDITO = -5;

    int fdDown[2];

    int fdUp[2];

    if (pipe(fdDown) == -1)
    {
        perror("Pipe Failed");

        exit(EXIT_FAILURE);
    }

    if (pipe(fdUp) == -1)
    {
        perror("Pipe Failed");

        exit(EXIT_FAILURE);
    }

    //Criar processo Filho:
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Fork Failed");
        exit(EXIT_FAILURE);
    }

    // If we are at the father process
    if (pid > 0)
    { //PAI

        srand(time(NULL)); //garante que rand funciona

        close(fdUp[ESCRITA]);

        close(fdDown[LEITURA]);

        int childCredits = CREDITO_INICIAL;

        while (childCredits > 0)
        {
            int numeroAleatorio = (rand() % 5) + 1; // random number between 1 and 5

            printf("Número Aleatório: %d\n", numeroAleatorio);

            write(fdDown[ESCRITA], &CONTINUAR, BUFFER_SIZE);

            int aposta;

            read(fdUp[LEITURA], &aposta, BUFFER_SIZE);

            if (aposta == numeroAleatorio)
            {
                printf("Venceu! A aposta era de: %d\n", aposta);
                childCredits = childCredits + GANHAR_CREDITO;
            }
            else
            {
                printf("Perdeu! A aposta era de: %d\n", aposta);
                childCredits = childCredits + PERDER_CREDITO;
            }
            printf("Saldo atual= %d\n", childCredits);
            printf("----------------------------------------\n");
        }
        write(fdDown[ESCRITA], &TERMINAR, BUFFER_SIZE);

        close(fdUp[LEITURA]);

        close(fdDown[ESCRITA]);
    }

    if (pid == 0)
    { //FILHO
        srand(time(NULL) * getpid());

        close(fdUp[LEITURA]);

        close(fdDown[ESCRITA]);

        int continuar = 1;

        while (continuar)
        {
            read(fdDown[LEITURA], &continuar, BUFFER_SIZE);

            int randomNumber = (rand() % 5) + 1; // random number between 1 and 5

            write(fdUp[ESCRITA], &randomNumber, BUFFER_SIZE);
        }

        close(fdUp[ESCRITA]);

        close(fdDown[LEITURA]);

        exit(EXIT_SUCCESS);
    }

    // Para o processo pai esperar pelo Filho
    int estado;

    pid_t pidExit = wait(&estado);

    int exitStatus = WEXITSTATUS(estado);

    printf("O processo %d terminou com %d\n", pidExit, exitStatus);

    exit(EXIT_SUCCESS);
    return 0;
}