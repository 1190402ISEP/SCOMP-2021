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

#define TOTAL_SEMAFOROS_CONTROLO_FLUXO 3
#define NUMERO_DE_PROCESSOS_FILHOS 3

#define SEMAFORO_1 "EX7-SEM1"
#define SEMAFORO_2 "EX7-SEM2"
#define SEMAFORO_3 "EX7-SEM3"

#define ID_SEMAFORO_1 0
#define ID_SEMAFORO_2 1
#define ID_SEMAFORO_3 2