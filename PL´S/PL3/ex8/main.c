#include "asm.h"

#define VALOR_DEFEITO 100
#define NUMERO_DE_OPERACOES 1000000


int main() {

    int filedescriptor = shm_open(FICHEIRO_PARTILHADO, OPEN_WRITER_OFLAG, OPEN_WRITER_MODE);

    if (filedescriptor == -1) {
        perror("Erro ao abrir memória Partilhada!\n\n");
        exit(EXIT_FAILURE);
    }

    ftruncate(filedescriptor, TAMANHO_ESTRUTURA);

    Estrutura *memoriaPartilhada = mmap(NULL, TAMANHO_ESTRUTURA, MAP_READ_WRITE, MAP_SHARED, filedescriptor, 0);

    memoriaPartilhada->numero = VALOR_DEFEITO;

    //Criação do processo filho para ambos executarem as suas funções

    pid_t pid = fork();

    int inc;


    if (pid > 0) { //Processo Pai
        for (inc = 0; inc < NUMERO_DE_OPERACOES; inc++) {
            memoriaPartilhada->numero += 1;
            memoriaPartilhada->numero -= 1;
        }

        //...dúvida!!!


        printf("Número no Pai: %d\n", memoriaPartilhada->numero);


        if (shm_unlink(FICHEIRO_PARTILHADO) < 0) {
            perror("Erro ao Eliminar a memória partilhada!\n");
            exit(EXIT_FAILURE);
        }

    }

    if (pid == 0) {
        for (inc = 0; inc < NUMERO_DE_OPERACOES; inc++) {
            memoriaPartilhada->numero += 1;
            memoriaPartilhada->numero -= 1;
        }
        printf("Número no Filho: %d\n", memoriaPartilhada->numero);

    }


    if (munmap(memoriaPartilhada, TAMANHO_ESTRUTURA) < 0) {
        perror("Desfazer o mapping!\n");
        exit(EXIT_FAILURE);
    }

    if (close(filedescriptor) < 0) {
        perror("Error ao fechar()!\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
/*
 * Os resultados são inconsistentes devido ao acesso concorrente à memória partilhada, ora, como não há nenhum mecanismo
 * de gestão de memória
 */