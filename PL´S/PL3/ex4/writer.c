#include "asm.h"

int main() {
    shm_unlink(FICHEIRO_PARTILHADO);

    int fileDescriptor = shm_open(FICHEIRO_PARTILHADO, OPEN_WRITER_OFLAG, OPEN_WRITER_MODE);

    int MAX = 26;

    char alphabet[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G',
                        'H', 'I', 'J', 'K', 'L', 'M', 'N',
                        'O', 'P', 'Q', 'R', 'S', 'T', 'U',
                        'V', 'W', 'X', 'Y', 'Z' };

    if (fileDescriptor == -1) {
        perror("ERRO no shm_open()!\n\nA terminar execução!\n\n");
        exit(EXIT_FAILURE);
    }
    //Definir tamanho da memória partilhada (tamanho da estrutura)

    ftruncate(fileDescriptor, TAMANHO_ESTRUTURA);

    //Obter o pointer para os dados

    String *memoriaPartilhada = (String *) mmap(NULL, TAMANHO_ESTRUTURA, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor,
                                          0);

    srand((unsigned) time(NULL));
    char *ptr = memoriaPartilhada->array;
    int i;
    for (i = 0; i < NUMERO_CHARS; i++) {
        char temp = alphabet[rand() % MAX];
        *ptr =  temp;
        ptr++;
    }


    //Desfazer o Mapping
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

    return EXIT_SUCCESS;
}