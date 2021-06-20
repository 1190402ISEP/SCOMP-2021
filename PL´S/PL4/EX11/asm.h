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

//Passenger
#define MAX_VAL 23
#define MIN_VAL 4
#define EXIT_VAL 18
#define CAP 200
#define WAIT_TIME_PASSENGER 2

//Train
#define DOORS 3
#define WAIT_TIME_TRAIN 4

//Both
#define STOPS 7

#define MEMORIA_PARTILHADA "/EX11_SHM"

#define SEMAFORO_OPEN "openSem"
#define SEMAFORO_CONTROL "controlSem"

typedef struct {
    int passengers;
    int currentStop;
} estruturaTrain;

#define TAMANHO_MEMORIA_PARTILHADA sizeof(estruturaTrain)


