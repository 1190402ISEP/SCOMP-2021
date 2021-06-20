#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <wait.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE sizeof(int)

#define NUMERO_DE_TRANSFERENCIAS 30

#define LEITURA 0
#define ESCRITA 1