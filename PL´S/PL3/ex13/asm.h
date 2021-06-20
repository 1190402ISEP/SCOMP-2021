#include <stdio.h>
#include<sys/wait.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>

#define FICHEIRO_PARTILHADO "/EX13"


#define TAMANHO_MAXIMO_STRING_CAMINHO 10000000 //tamanho do caminho para o ficheiro
#define TAMANHO_MAXIMO_STRING_PALAVRA_A_PESQUISAR 10

typedef struct {
    char word[TAMANHO_MAXIMO_STRING_PALAVRA_A_PESQUISAR];
    char filePath[TAMANHO_MAXIMO_STRING_CAMINHO];
    int numOcurr;
} ProcessoFilho1;

typedef struct {
    ProcessoFilho *pf1;
    ProcessoFilho *pf2;
    ProcessoFilho *pf3;
    ProcessoFilho *pf4;
    ProcessoFilho *pf5;
    ProcessoFilho *pf6;
    ProcessoFilho *pf7;
    ProcessoFilho *pf8;
    ProcessoFilho *pf9;
    ProcessoFilho *pf10;
} DadosPartilhados;

#define TAMANHO_MEMORIA_PARTILHADA sizeof(DadosPartilhados)
#define NUMERO_PROCESSOS_FILHO 10

#define PALAVRA_A_PESQUISAR "ISEP"

#define FILE_FILHO1 "PF1.txt"
#define FILE_FILHO2 "PF2.txt"
#define FILE_FILHO3 "PF3.txt"
#define FILE_FILHO4 "PF4.txt"
#define FILE_FILHO5 "PF5.txt"
#define FILE_FILHO6 "PF6.txt"
#define FILE_FILHO7 "PF7.txt"
#define FILE_FILHO8 "PF8.txt"
#define FILE_FILHO9 "PF9.txt"
#define FILE_FILHO10 "PF10.txt"

#define LEITURA 0
#define ESCRITA 1