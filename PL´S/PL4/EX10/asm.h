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

#define MEMORIA_PARTILHADA "/EX10_SHM"

#define SEMAFORO_1 "/ex9Sem1"
#define SEMAFORO_2 "/ex9Sem2"
#define SEMAFORO_3 "/ex9Sem3"
#define SEMAFORO_4 "/ex9Sem4"

#define MAX 100
#define SIZE_NAME 50
#define SIZE_ADDRESS 100

typedef struct {
    int num;
    char nome[SIZE_NAME];
    char morada[SIZE_ADDRESS];
} estruturaPessoa;

#define TAMANHO_MEMORIA_PARTILHADA sizeof(estruturaPessoa)