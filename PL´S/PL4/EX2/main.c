#include "asm.h"

/*
 * 2. Change the last program in a way that the child process reads from the file “Numbers.txt” by order of its process
 * number (1 to 8). Use 8 semaphores to synchronize between the child processes
*/
int main(void) {
    sem_t *sem[NUMERO_DE_PROCESSOS_FILHO];
    char nomeSemaforo[100];
    FILE *numeros, *output;


    sprintf(nomeSemaforo, "%s%d", SEMAPHORE, 1);

    //Como no exemplo dos slides o primeiro semáforo inicializa a 1 e os restantes a 0

    if ((sem[0] = sem_open(nomeSemaforo, O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(EXIT_FAILURE);
    }

    int processN;

    for (processN = 1; processN < NUMERO_DE_PROCESSOS_FILHO; processN++) {
        sprintf(nomeSemaforo, "%s%d", SEMAPHORE, processN + 1);

        if ((sem[processN] = sem_open(nomeSemaforo, O_CREAT, 0644, 0)) == SEM_FAILED) {
            perror("Error in sem_open()");
            exit(EXIT_FAILURE);
        }
    }

    // Create clean output file
    output = fopen(FILE_TO_WRITE, "w");
    fclose(output);


    int posicao, numero, x;
    pid_t pid;
    // Creating 8 processes
    for (x = 0; x < NUMERO_DE_PROCESSOS_FILHO; x++) {
        pid = fork();
        if (pid > 0) {
            wait(NULL);
        } else if (pid == 0) {

            sem_wait(sem[x]);

            // Open input file for reading
            numeros = fopen(FILE_TO_READ, "r");
            if (numeros != NULL) {

                output = fopen(FILE_TO_WRITE, "a");

                for (posicao = 0; posicao < x * QTD_NUMEROS_PARA_PROCESSAR; posicao++) {
                    fscanf(numeros, "%d", &numero);
                }

                for (; posicao < (x + 1) * QTD_NUMEROS_PARA_PROCESSAR; posicao++) {
                    fscanf(numeros, "%d", &numero);
                    fprintf(output, "%d\n", numero);
                }

                fclose(output);

                fclose(numeros);
            }

            //Aqui vamos permitir ao processo seguinte a entrada na zona crítica
            if (x < NUMERO_DE_PROCESSOS_FILHO - 1) {
                sem_post(sem[x + 1]);
            }

            sem_close(sem[x]);

            sprintf(nomeSemaforo, "%s%d", SEMAPHORE, x + 1);
            sem_unlink(nomeSemaforo);

            exit(EXIT_FAILURE);

        } else {
            perror("Fork Failed!");
            exit(EXIT_FAILURE);
        }
    }
    // Open output file for reading
    output = fopen(FILE_TO_WRITE, "r");
    if (output == NULL) {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }

    while (fscanf(output, "%d", &numero) != EOF) {
        printf("%d\n", numero);

    }

    fclose(output);

    return EXIT_SUCCESS;
}