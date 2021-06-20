#include <semaphore.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MEMORIA_PARTILHADA "/EX4_SHM"

#define SEMAFORO "/EX4_SEM"

#define NUMERO_PROCESSOS_FILHO 5

#define NUMERO_DE_STRINGS 50

#define NUMERO_DE_CHARS 80

typedef struct{
    int indice;
    char string[NUMERO_DE_STRINGS][NUMERO_DE_CHARS];
}estruturaMemoriaPartilhada;

#define TAMANHO_MEMORIA_PARTILHADA sizeof(estruturaMemoriaPartilhada)

#define TEMPO_MAXIMO_ESPERA 5
#define TEMPO_DE_ESPERA_PARA_ELIMINAR_STRING 12