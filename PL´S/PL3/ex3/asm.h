#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

typedef struct {
    int n;
    char string[18];
} Isep;

#define NUMERO_ELEMENTOS_ARRAY 100000

typedef struct {
    Isep estrutura[100000];
} MEMORIA_PARTILHADA_TYPE;


#define FICHEIRO_PARTILHADO "/EX03_SHARED_FILE"
#define TAMANHO_ESTRUTURA sizeof(MEMORIA_PARTILHADA_TYPE)


#define OPEN_WRITER_OFLAG O_CREAT | O_EXCL | O_RDWR
#define OPEN_WRITER_MODE S_IRUSR | S_IWUSR
#define OPEN_READER_OFLAG O_EXCL | O_RDWR
#define OPEN_READER_MODE S_IRUSR | S_IWUSR

#define READ  0
#define WRITE 1


//Funções Necessárias:
int writerProcessoPai();

int readerProcessoFilho();

int writerProcessoFilho();

int readerProcessoFilho();