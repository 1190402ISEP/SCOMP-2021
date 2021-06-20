#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

#define TAMANHO_DO_BUFFER 10

typedef struct {
    int buffer[TAMANHO_DO_BUFFER]; //onde guarda
    int bufferLength; //nr valores ocupados
    int write;
    int read;
} circular_buffer;

#define TAMANHO_ESTRUTURA sizeof(circular_buffer)

#define OPEN_WRITER_OFLAG O_CREAT | O_EXCL | O_RDWR
#define OPEN_WRITER_MODE S_IRUSR | S_IWUSR
#define OPEN_READER_OFLAG O_EXCL | O_RDWR
#define OPEN_READER_MODE S_IRUSR | S_IWUSR
#define MAP_READ_WRITE PROT_READ | PROT_WRITE

#define READ  0
#define WRITE 1