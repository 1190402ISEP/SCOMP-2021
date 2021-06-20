#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h> /* For constants O_* */
#include <sys/stat.h> /* For constants “mode” */

typedef struct {
    int number1;
    int number2;
} Integers;

#define TOTAL_ITERACOES 1000000
#define FICHEIRO_DA_MEMORIA_PARTILHADA "/EX05"
#define TAMANHO_DA_ESTRUTURA sizeof(Integers)

#define OPEN_WRITER_OFLAG O_CREAT | O_EXCL | O_RDWR
#define OPEN_WRITER_MODE S_IRUSR | S_IWUSR

#define OPEN_READER_OFLAG O_EXCL | O_RDWR
#define OPEN_READER_MODE S_IRUSR | S_IWUSR

#define MAP_READ_WRITE PROT_READ | PROT_WRITE
