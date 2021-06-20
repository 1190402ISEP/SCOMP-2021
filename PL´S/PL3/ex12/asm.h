#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <wait.h>
#include <malloc.h>
#include <stdlib.h>
#include <limits.h>

#define STR_SIZE 50
#define NR_DISC 10

typedef struct {
    int numero;
    char nome[STR_SIZE];
    int disciplinas[NR_DISC];
    int synch;
} Aluno;

#define FICHEIRO_PARTILHADO "/EX12"

#define OPEN_WRITER_OFLAG O_CREAT | O_EXCL | O_RDWR
#define OPEN_WRITER_MODE S_IRUSR | S_IWUSR