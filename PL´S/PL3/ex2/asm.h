#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h> /* For constants O_* */
#include <sys/stat.h> /* For constants “mode” */

typedef struct {
    char code[10];
    char description[80];
    float price;
} Product;

#define FICHEIRO_PARTILHADO "/EX02_SHARED_FILE"
#define TAMANHO_ESTRUTURA sizeof(Product)

#define OPEN_WRITER_OFLAG O_CREAT | O_EXCL | O_RDWR
#define OPEN_WRITER_MODE S_IRUSR | S_IWUSR

#define OPEN_READER_OFLAG O_EXCL | O_RDWR
#define OPEN_READER_MODE S_IRUSR | S_IWUSR