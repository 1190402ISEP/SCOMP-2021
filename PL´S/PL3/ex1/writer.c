#include "asm.h"

int main() {

    int fileDescriptor = shm_open(FICHEIRO_PARTILHADO, OPEN_WRITER_OFLAG, OPEN_WRITER_MODE);

    if (fileDescriptor == -1) {
        perror("ERRO no shm_open()!\n\nA terminar execução!\n\n");
        exit(EXIT_FAILURE);
    }
    //Definir tamanho da memória partilhada (tamanho da estrutura)

    ftruncate(fileDescriptor, TAMANHO_ESTRUTURA);

    //Obter o pointer para os dados

    Student *memoriaPartilhada = (Student *) mmap(NULL, TAMANHO_ESTRUTURA, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);

    //Interação com o Utilizador começa aqui:

    printf("Insira o nº do Aluno:\n");
    scanf("%d", &memoriaPartilhada->number);

    printf("Insira o Nome do Aluno:\n");
    scanf("%s", memoriaPartilhada->name);

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