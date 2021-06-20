#include "asm.h"

/*
 * The parent process should:
• Create an area of shared memory. The memory area must contain, for each child process, the following
information:
– path to the file;
– word to search;
– an integer to store the number of occurrences.
• Create 10 new processes;
• Fill the shared memory area with the information for each child process;
• Wait until the child processes finish their search;
• Print the number of occurrences determined by each child;
• Eliminate the shared memory area.
Each child process should:
• Open the text file assigned to it by the parent (can/should be different for each child process);
• Determine the number of occurrences of the word to search;
• Write the number of occurrences in their position in the shared memory area.
 */


int main() {
    int validacaoErros = 0;
    validacaoErros = writerProcessoPai();//Cria a estrutura dos filhos
    if (validacaoErros == EXIT_FAILURE) {
        perror("Erro!");
        exit(EXIT_FAILURE);
    }
    validacaoErros = readerProcessoFilho();//Lê os dados
    if (validacaoErros == EXIT_FAILURE) {
        perror("Erro!");
        exit(EXIT_FAILURE);
    }

    validacaoErros = writerProcessoFilho();//Escreve as ocorrências
    if (validacaoErros == EXIT_FAILURE) {
        perror("Erro!");
        exit(EXIT_FAILURE);
    }
    validacaoErros = readerProcessoFilho();//lê e imprime as occorências
    if (validacaoErros == EXIT_FAILURE) {
        perror("Erro!");
        exit(EXIT_FAILURE);
    }
    return 0;
}

int writerProcessoPai() {
    MEMORIA_PARTILHADA_TYPE *memoriaPartilhada;
    int fileDescriptor;


    //Abrir a Memória Partilhada
    fileDescriptor = shm_open(FICHEIRO_PARTILHADO, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);

    if (fileDescriptor == -1) {
        perror("ERRO no shm_open()!\n\nA terminar execução!\n\n");
        exit(EXIT_FAILURE);
    }

    //Definir o tamanho da memória partilhada

    ftruncate(fileDescriptor, TAMANHO_ESTRUTURA);

    //Mapear a memória partilhada

    memoriaPartilhada = (MEMORIA_PARTILHADA_TYPE) *
                        mmap(NULL, TAMANHO_ESTRUTURA, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);


}


int readerProcessoFilho() {}

int writerProcessoFilho() {}

int readerProcessoFilho() {}