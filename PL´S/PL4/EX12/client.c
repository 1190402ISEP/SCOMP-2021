#include "asm.h"

int main() {

    const char NOME_SEMAFOROS[NUMERO_DE_SEMAFOROS][80] = {"NOME_SEMAFORO_1", "NOME_SEMAFORO_2"};
    const int SEMAFOROS_VALOR_INICIAL[NUMERO_DE_SEMAFOROS] = {0,1};
    sem_t *semaforos[NUMERO_DE_SEMAFOROS];

    //inicializar semaforos
    int i;
    for (i = 0; i < NUMERO_DE_SEMAFOROS; i++){
        if ((semaforos[i] = sem_open(NOME_SEMAFOROS[i], O_CREAT, 0644, SEMAFOROS_VALOR_INICIAL[i])) == SEM_FAILED){
            perror("Erro em sem_open() cliente!\n");
            exit(EXIT_FAILURE);
        }
    }

    memoriaPartilhadaType* memoriaPartilhada;
    int fd;

    //OPEN SHARED MEMORY
    if((fd = shm_open(FICHEIRO_PARTILHADO, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR)) == -1){
        perror("Shm_open erro!\n");
        exit(1);
    }

    //TUNCATE SHARED MEMORY SIZE
    if(ftruncate(fd,sizeof(memoriaPartilhada)) == -1){
        perror("Ftruncate erro!\n");
        exit(1);
    }

    //Map da shared memory
    if((memoriaPartilhada = (memoriaPartilhadaType*) mmap(NULL, sizeof(memoriaPartilhada), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED){
        perror("Mmap erro!\n");
        exit(1);
    }

    /*
     * Faço um loop que percorre o um valor igual ao numero de clientes, para cada cliente fazer o print do
     * seu bilhete.o semWait é para esperar o vendedor vender e o semPost é para avisar ao vendedor que o cliente
     * ja foi atendido e o próximo cliente pode ser atendido.
     */
    int count = 0;
    do{
        memoriaPartilhada->numeroClientes++;
        __semWAIT(semaforos[0]);
        printf("O numero do bilhete é %d\n", memoriaPartilhada->numeroBilhetes);
        __semPOST(semaforos[1]);
        count++;
    }while(count != NUMERO_DE_CLIENTES);

    return EXIT_SUCCESS;
}

