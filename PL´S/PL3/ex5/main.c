#include "asm.h"

int main(void) {

    int fd = shm_open(FICHEIRO_DA_MEMORIA_PARTILHADA, OPEN_WRITER_OFLAG, OPEN_WRITER_MODE);

    if (fd == -1) {
        printf("Erro ao criar memoria partilhada, verificar codigo");
        return -1;
    }

    ftruncate(fd, TAMANHO_DA_ESTRUTURA);

    Integers *estrutura = mmap(NULL, TAMANHO_DA_ESTRUTURA, MAP_READ_WRITE,
                               MAP_SHARED, fd, 0);

    estrutura->number1 = 8000;
    estrutura->number2 = 200;
    int i;

    pid_t pid = fork();

    if (pid == -1) {
        printf("Erro no fork");
        return -1;
    }

    if (pid > 0) {
        for (i = 0; i < TOTAL_ITERACOES; i++) {
            estrutura->number1 += 1;
            estrutura->number2 -= 1;
        }
    } else {
        for (i = 0; i < TOTAL_ITERACOES; i++) {
            estrutura->number1 -= 1;
            estrutura->number2 += 1;
        }

        munmap(estrutura, TAMANHO_DA_ESTRUTURA);
        close(fd);
        exit(2);
    }


    printf("Número 1: %d <-> Número 2: %d \n", estrutura->number1, estrutura->number2);


    if (shm_unlink(FICHEIRO_DA_MEMORIA_PARTILHADA) < 0) {
        perror("Erro ao fechar");
        return -1;
    }


    return 0;
}
/*
 * Os cálculos não devem estar certos já que não havia um mecanismo que controlasse o acesso aos dados e os processos
 * estavam a executar concorrentemente.
 */