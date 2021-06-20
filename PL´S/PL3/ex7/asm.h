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

#define TOTAL_NUMEROS 10
typedef struct {
    int n[10];
} Numbers;


#define FICHEIRO_PARTILHADO "/EX07"
#define TAMANHO_ESTRUTURA sizeof(Numbers)

#define OPEN_WRITER_OFLAG O_CREAT | O_EXCL | O_RDWR
#define OPEN_WRITER_MODE S_IRUSR | S_IWUSR

#define OPEN_READER_OFLAG O_EXCL | O_RDWR
#define OPEN_READER_MODE S_IRUSR | S_IWUSR