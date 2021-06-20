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

#define SEMAFORO_S "EX8_S"
#define SEMAFORO_C "EX8_C"

#define TOTAL_SEMAFOROS 2
#define TOTAL_PROCESSOS 2