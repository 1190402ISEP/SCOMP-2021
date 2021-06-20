#include "asm.h"

long long

tempo_em_milissegundos() {
    struct timeval te;
    gettimeofday(&te, NULL);
    long long milissegundos = te.tv_sec * 1000LL + te.tv_usec / 1000;
    return milissegundos;
}


int writer() {
    int fileDescriptor = shm_open(FICHEIRO_PARTILHADO, OPEN_WRITER_OFLAG, OPEN_WRITER_MODE);

    if (fileDescriptor == -1) {
        perror("ERRO no shm_open()!\n\nA terminar execução!\n\n");
        exit(EXIT_FAILURE);
    }
    //Definir tamanho da memória partilhada (tamanho da estrutura)

    ftruncate(fileDescriptor, TAMANHO_ESTRUTURA);

    //Obter o pointer para os dados

    ArrayEstruturas *memoriaPartilhada = (ArrayEstruturas *) mmap(NULL, TAMANHO_ESTRUTURA, PROT_READ | PROT_WRITE,
                                                                  MAP_SHARED,
                                                                  fileDescriptor, 0);

    Isep *ptrIsep = memoriaPartilhada->estrutura;

    int i = 0;
    for (i = 0; i < NUMERO_ELEMENTOS_ARRAY; i++) {
        ptrIsep[i].n = i;
        strcpy(ptrIsep[i].string, "ISEP – SCOMP 2020");
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

int reader() {

    //Criar e abrir a memória partilhada
    int fileDescriptor = shm_open(FICHEIRO_PARTILHADO, OPEN_READER_OFLAG, OPEN_READER_MODE);

    if (fileDescriptor == -1) {
        perror("ERRO no shm_open()!\n\nA terminar execução!\n\n");
        exit(EXIT_FAILURE);
    }

    //Definir tamanho da memória partilhada (tamanho da estrutura)

    ftruncate(fileDescriptor, TAMANHO_ESTRUTURA);

    //Obter o pointer para os dados

    ArrayEstruturas *memoriaPartilhada = (ArrayEstruturas *) mmap(NULL, TAMANHO_ESTRUTURA, PROT_READ | PROT_WRITE, MAP_SHARED,
                                                  fileDescriptor, 0);

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

    //Remover o Ficheiro
    test = shm_unlink(FICHEIRO_PARTILHADO);

    if (test < 0) {
        perror("Erro ao remover o Ficheiro!\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}


int main() {

    long long tempoInicial;
    long long tempoFinal;

    int exitWriter = 0, exitReader = 0;

    tempoInicial = tempo_em_milissegundos();

    exitWriter = writer();
    //iniciar
    if (exitWriter < 0) {
        perror("Erro no Writer!!");
        exit(EXIT_FAILURE);
    }
    exitReader = writer();
    //

    if (exitReader < 0) {
        perror("Erro no Reader!!");
        exit(EXIT_FAILURE);
    }

    tempoFinal = tempo_em_milissegundos();
    //termina e mostra time

    long long tempoTotal;
    tempoTotal = tempoFinal - tempoInicial;
    printf("Tempo para SharedMemory: %lld \n", tempoTotal);

    return 0;
}