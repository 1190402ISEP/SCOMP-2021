#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <semaphore.h>

typedef struct {
    int nr_readers;
} readers;

#define MEMORIA_PARTILHADA_STRINGS "/Ex14_Strings_Shm"
#define MEMORIA_PARTILHADA_READERS "/Ex14_Readers_Shm"

#define TAMANHO_MEMORIA_PARTILHADA_READERS sizeof(readers)
#define TAMANHO_MEMORIA_PARTILHADA_STRINGS 100

#define SEM_WRITER "writerSem"
#define SEM_READER "readerSem"