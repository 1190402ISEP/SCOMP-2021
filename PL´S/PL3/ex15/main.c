#include "asm.h"

int main() {

    int fileDescriptor[2];

    if (pipe(fileDescriptor) == -1) {
        perror("Falha na funcao pipe()");
        return EXIT_FAILURE;
    }

    /*Através da criação de dois processos poderemos simular 1 produtor e 1 consumidor*/
    int value = 0;
    pid_t pid = fork();

    if (pid < 0) {
        perror("Erro no fork()\n\nA abortar!!\n");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        wait(NULL);
        //Consumidor (Processo Filho)
        close(fileDescriptor[ESCRITA]);

        //Lê os 30 números e imprimir na consola

        while (read(fileDescriptor[LEITURA], &value, BUFFER_SIZE)) {
            printf("Número Lido: %d\n", value);
        }
        close(fileDescriptor[LEITURA]); //Libertar Recursos

        exit(EXIT_SUCCESS);
    } else if (pid == 0) {

        //Produtor (Processo Pai)
        close(fileDescriptor[LEITURA]);

        for (value = 0; value < NUMERO_DE_TRANSFERENCIAS; value++) { //Números Ordenados

            //escrever os números no pipe
            write(fileDescriptor[ESCRITA], &value, BUFFER_SIZE);
        }

        close(fileDescriptor[ESCRITA]); //Libertar Recursos
    }

    return EXIT_SUCCESS;
}