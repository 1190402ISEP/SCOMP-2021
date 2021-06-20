#include "structs.h"

int main(int argc, char **argv) {
    sem_unlink(SEM_WRITER);
    sem_unlink(SEM_READER);
    shm_unlink(MEMORIA_PARTILHADA_STRINGS);
    shm_unlink(MEMORIA_PARTILHADA_READERS);

    int fdS = shm_open(MEMORIA_PARTILHADA_STRINGS, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    int fdR = shm_open(MEMORIA_PARTILHADA_READERS, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);

    if (fdS == -1 || fdR == -1) {
        perror("Erro a abrir a memória partilhada.\n");
        exit(0);
    }

    if (ftruncate(fdR, TAMANHO_MEMORIA_PARTILHADA_READERS) == -1 ||
        ftruncate(fdS, TAMANHO_MEMORIA_PARTILHADA_STRINGS) == -1) {
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
    if ((semWriter = sem_open(SEM_WRITER, O_CREAT, 0644, 1)) == SEM_FAILED ||
        (semReader = sem_open(SEM_READER, O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Erro a criar o semaforo.\n");
        exit(0);
    }

    if (munmap(sharedData, TAMANHO_MEMORIA_PARTILHADA_READERS) == -1 ||
        munmap(string, TAMANHO_MEMORIA_PARTILHADA_STRINGS) == -1) {
        perror("Erro a deconectar da memoria partilhada.\n");
        exit(0);
    }

    if (close(fdS) == -1 || close(fdR) == -1) {
        perror("Erro a fechar a memoria partilhada.\n");
        exit(0);
    }

    return 0;
}

