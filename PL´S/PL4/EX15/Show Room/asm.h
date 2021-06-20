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
#define TOT_STR 7

#define CAP 5
#define MAX_VAL 4
#define MIN_VAL 1
#define MAX_EXIT 2

typedef struct {
    int nbVisitors;
    int totalNbVisitors;
    int showEnded;
} ShowRoom;

void *createShm(char *shmName, int dataSize);
