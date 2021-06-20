#include "asm.h"

long long tempo_em_milissegundos() {
    struct timeval te;
    gettimeofday(&te, NULL);
    long long milissegundos = te.tv_sec * 1000LL + te.tv_usec / 1000;
    return milissegundos;
}

int main() {

    Isep array[NUMERO_ELEMENTOS_ARRAY * TAMANHO_ESTRUTURA];

    const int BUFFER_SIZE = sizeof(array);

    long long tempoInicial;
    long long tempoFinal;

    int fileDescriptor[2];

    if (pipe(fileDescriptor) == -1) {
        perror("Pipe Error!\n");
        exit(EXIT_FAILURE);
    }

    tempoInicial = tempo_em_milissegundos();
    printf("Tempo de Escrita: %lld\n", tempoInicial);
    //Criar novo processo
    pid_t pid = fork();

    if (pid > 0) {//Processo Pai
        waitpid(pid, NULL, 0);

        close(fileDescriptor[READ]);
        printf("Preenchendo o array de struct com %d elementos ...\n", NUMERO_ELEMENTOS_ARRAY);
        int i;
        for (i = 0; i < NUMERO_ELEMENTOS_ARRAY; i++) {
            array[i].n = i;
            strcpy(array[i].string, "ISEP - SCOMP 2020");
        }

        tempoFinal = tempo_em_milissegundos();
        printf("Tempo Total Leitura : %lld\n", tempoFinal);

        write(fileDescriptor[WRITE], array, BUFFER_SIZE);
        close(fileDescriptor[WRITE]);
        printf("Tempo Total: %lld milissegundos\n", tempoFinal - tempoInicial);

    } else if (pid == 0) {//Filho
        close(fileDescriptor[WRITE]);
        Isep arrayParaLeitura[TAMANHO_ESTRUTURA];
        read(fileDescriptor[READ], &arrayParaLeitura, BUFFER_SIZE);
        close(fileDescriptor[READ]);

        printf("Operação leitura array[10]\n%d\n%s", arrayParaLeitura->n, arrayParaLeitura->string);

        exit(EXIT_SUCCESS);
    }

    return EXIT_SUCCESS;
}