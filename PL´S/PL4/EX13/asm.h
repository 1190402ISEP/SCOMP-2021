#pragma once

#include <sys/types.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define ARR_SIZE 10
#define NB_CHILD 2
#define STR_SIZE 500
#define NB_EXCHANGE 30
#define INIT_VAL 23
#define INC 11

void handleSIGUSR1(int sig, siginfo_t *sigInfo, void *context);
void *createShm(char *shmName, int dataSize);
void creatSigUSR1();