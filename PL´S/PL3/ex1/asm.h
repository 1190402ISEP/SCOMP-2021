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
    char name[80];
    int number;
} Student;

#define FICHEIRO_PARTILHADO "/EX01_SHARED_FILE"
#define TAMANHO_ESTRUTURA sizeof(Student)

#define OPEN_WRITER_OFLAG O_CREAT | O_EXCL | O_RDWR
#define OPEN_WRITER_MODE S_IRUSR | S_IWUSR

#define OPEN_READER_OFLAG O_EXCL | O_RDWR
#define OPEN_READER_MODE S_IRUSR | S_IWUSR