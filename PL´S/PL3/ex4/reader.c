#include "asm.h"

int main() {

    //Criar e abrir a memória partilhada
    int fileDescriptor = shm_open(FICHEIRO_PARTILHADO, OPEN_READER_OFLAG, OPEN_READER_MODE);

    if (fileDescriptor == -1) {
        perror("ERRO no shm_open()!\n\nA terminar execução!\n\n");
        exit(EXIT_FAILURE);
    }

    //Definir tamanho da memória partilhada (tamanho da estrutura)

    ftruncate(fileDescriptor, TAMANHO_ESTRUTURA);

    //Obter o pointer para os dados

    String *memoriaPartilhada = (String *) mmap(NULL, TAMANHO_ESTRUTURA, PROT_READ | PROT_WRITE, MAP_SHARED,
                                                fileDescriptor, 0);

    //Imprimir a informação no monitor
    char *ptr = memoriaPartilhada->array;
    double media = 0;
    for (int i = 0; i < NUMERO_CHARS; i++) {
        media += *ptr;
        printf("%c\n", *ptr);
        ptr++;
    }
    media = media / NUMERO_CHARS;

    printf("média= %.0f\n", media);    //Desfazer o Mapping
    int test = munmap((void *) memoriaPartilhada, TAMANHO_ESTRUTURA);

    if (test < 0) {
        perror("Erro ao Desfazer o Mapping!\n");
        exit(EXIT_FAILURE);
    }

    //Fechar o fileDescriptor

    test = close(fileDescriptor);

    if (test < 0) {
        perror("Erro ao fechar o FIle Descriptor!\n");
        exit(EXIT_FAILURE);
    }

    //Remover o Ficheiro
    test = shm_unlink(FICHEIRO_PARTILHADO);

    if (test < 0) {
        perror("Erro ao remover o Ficheiro!\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}