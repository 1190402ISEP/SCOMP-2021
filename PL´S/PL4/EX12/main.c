#include "asm.h"

int main() {

    //Criar file descriptor
    int fd = shm_open(FICHEIRO_PARTILHADO, OPEN_WRITER_OFLAG, OPEN_WRITER_MODE);

    //Verificação de Erros
    if (fd == -1) {
        perror("ERRO no shm_open()!\n\nA terminar execução!\n\n");
        exit(EXIT_FAILURE);
    }

    //Definir tamanho da memória partilhada (tamanho da estrutura)
    if (ftruncate(fd, TAMANHO_ESTRUTURA) == -1) {
        perror("ERRO no ftruncate()!\n\nA terminar execução!\n\n");
        exit(EXIT_FAILURE);
    }

    //Obter o pointer para os dados
    memoriaPartilhadaType *memoriaPartilhada;

    if ((memoriaPartilhada = (memoriaPartilhadaType *) mmap(NULL, TAMANHO_ESTRUTURA, PROT_READ | PROT_WRITE, MAP_SHARED,
                                                            fd, 0)) == MAP_FAILED) {
        perror("ERRO no mmap()!\n\nA terminar execução!\n\n");
        exit(EXIT_FAILURE);
    }

    //Memória partilhada criada
    memoriaPartilhada->numeroBilhetes = 0;
    memoriaPartilhada->numeroClientes = 0;

    int i;
    pid_t pid;

    for (i = 0; i < NUMERO_DE_CLIENTES; i++) {
        pid = fork();
        if (pid < 0) {
            perror("Erro no fork()");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) {


            execlp("./client > client.txt", "./client > client.txt", (char *) NULL);


        }
    }

    execlp("./seller > seller.txt", "./seller > seller.txt", (char *) NULL);

    return EXIT_SUCCESS;
}









