#pragma once

#include <semaphore.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <regex.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <string.h>

#define STR_SIZE 500
#define TOT_STR1 23
#define TOT_STR2 7

#define CAP 5
#define MAX_NAME 22
#define MAX_MESS 6
#define MIN_VAL 0
#define MAX_EXIT 23
#define MIN_EXIT 1
#define EXIT_VAL 21

typedef struct {
    int nbVisitors;
    int totalNbVisitors;
    int showEnded;
} ShowRoom;

void *openShm(char *shmName, int dataSize);