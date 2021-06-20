#include <semaphore.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

#define NUMERO_DE_SEMAFOROS 2
#define NUMERO_DE_CLIENTES 5

#define NOME_SEMAFORO_1 "CLIENT"
#define NOME_SEMAFORO_2 "SELLER"

typedef struct {
    int numeroBilhetes;
    int numeroClientes;
} memoriaPartilhadaType;

#define READ  0
#define WRITE 1

#define TAMANHO_ESTRUTURA sizeof(memoriaPartilhadaType)
#define FICHEIRO_PARTILHADO "/EX12"

#define OPEN_WRITER_OFLAG O_CREAT | O_EXCL | O_RDWR
#define OPEN_WRITER_MODE S_IRUSR | S_IWUSR
#define OPEN_READER_OFLAG O_EXCL | O_RDWR
#define OPEN_READER_MODE S_IRUSR | S_IWUSR
#define MAP_READ_WRITE PROT_READ | PROT_WRITE
