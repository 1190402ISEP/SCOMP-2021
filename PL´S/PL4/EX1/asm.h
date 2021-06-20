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

#define SEMAPHORE "/EX1"

#define NUMERO_DE_PROCESSOS_FILHO 8
#define QTD_NUMEROS_PARA_PROCESSAR 200


#define FILE_TO_READ "Numeros.txt"
#define FILE_TO_WRITE "Output.txt"

