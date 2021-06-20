#include "asm.h"

int main() {

    const char NOME_SEMAFOROS[NUMERO_DE_SEMAFOROS][80] = {"NOME_SEMAFORO_1", "NOME_SEMAFORO_2"};
    const int SEMAFOROS_VALOR_INICIAL[NUMERO_DE_SEMAFOROS] = {0,1};
    sem_t *semaforos[NUMERO_DE_SEMAFOROS];

    //inicializar semaforos
    int i;
    for (i = 0; i < NUMERO_DE_SEMAFOROS; i++){
        if ((semaforos[i] = sem_open(NOME_SEMAFOROS[i], O_CREAT, 0644, SEMAFOROS_VALOR_INICIAL[i])) == SEM_FAILED){
            perror("Erro em sem_open() seller!\n");
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

    int time = 0;

    for (i = 0; i < SIZE_ARRAY; i++) {
        time = rand() % 10 + 1;
    }

    do{
        memoriaPartilhada->numeroClientes--;
        memoriaPartilhada->numeroBilhetes++;
        __semPOST(semaforos[0]);
        __semWAIT(semaforos[1]);
        sleep(2);
    } while (memoriaPartilhada->numeroClientes > 0);

    // Close all semaforos
    for (i = 0; i < NUMERO_DE_SEMAFOROS; i++){
        if (sem_close(semaforos[i]) == -1){
            perror("Erro no sem_close()!\n");
            exit(EXIT_FAILURE);
        }
    }

    // Remove semaforos from system
    for (i = 0; i < NUMERO_DE_SEMAFOROS; i++){
        if (sem_unlink(NOME_SEMAFOROS[i]) == -1){
            perror("Erro no sem_unlink()!\n");
            printf("Error: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    // Undo mapping
    if (munmap(memoriaPartilhada, sizeof(memoriaPartilhadaType)) == -1)
    {
        perror("Munmap falhou!\n");
        exit(EXIT_FAILURE);
    }

     // Close file descriptor
    if (close(fd) == -1){
        perror("Close falhou!\n");
        exit(EXIT_FAILURE);
    }

    // Remove file from system
    if (shm_unlink(FICHEIRO_PARTILHADO) < 0){
        perror("Erro no unlink!\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

