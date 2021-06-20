#include "structs.h"

int main(int argc, char **argv) {

    int fdR = shm_open(MEMORIA_PARTILHADA_READERS, O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);

    int fdS = shm_open(MEMORIA_PARTILHADA_STRINGS, O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);

    if (fdS == -1 || fdR == -1) {
        perror("Erro a abrir a memória partilhada.\n");
        exit(0);
    }

    if (ftruncate(fdR, TAMANHO_MEMORIA_PARTILHADA_READERS) == -1 ||
        ftruncate(fdS, TAMANHO_MEMORIA_PARTILHADA_STRINGS)) {
        perror("Erro a definir o tamanho da memória partilhada.\n");
        exit(0);
    }

    readers *sharedData = (readers *) mmap(NULL, TAMANHO_MEMORIA_PARTILHADA_READERS, PROT_READ | PROT_WRITE, MAP_SHARED,
                                           fdR, 0);

    char *string = (char *) mmap(NULL, TAMANHO_MEMORIA_PARTILHADA_STRINGS, PROT_READ | PROT_WRITE, MAP_SHARED, fdS, 0);

    if (sharedData == NULL || string == NULL) {
        perror("Erro a mapear a memória partilhada.\n");
        exit(0);
    }

    sem_t *semWriter, *semReader;
    if ((semWriter = sem_open(SEM_WRITER, O_EXCL, 0644, 1)) == SEM_FAILED ||
        (semReader = sem_open(SEM_READER, O_EXCL, 0644, 1)) == SEM_FAILED) {
        perror("Erro a criar o semaforo.\n");
        exit(0);
    }

    printf("A espera que os outros writers ou readers terminem.\n");
    sem_wait(semWriter);

    time_t timeinfo;
    time(&timeinfo);
    char *time_str = ctime(&timeinfo);

    sprintf(string, "PID : %d Current Time: %s\n", getpid(), time_str);
    printf("Writer: escreveu %s\n", string);
    sleep(10);

    sem_post(semWriter);

    if (munmap(sharedData, TAMANHO_MEMORIA_PARTILHADA_READERS) == -1 ||
        munmap(string, TAMANHO_MEMORIA_PARTILHADA_STRINGS) == -1) {
        perror("Erro a deconectar da memoria partilhada.\n");
        exit(0);
    }

    if (close(fdR) == -1 || close(fdS) == -1) {
        perror("Erro a fechar a memoria partilhada.\n");
        exit(0);
    }

    return 0;
}

