#include "asm.h"

int main() {

    int data_size = sizeof(Aluno);

    int fileDescriptor;


    fileDescriptor = shm_open(FICHEIRO_PARTILHADO, OPEN_WRITER_OFLAG, OPEN_WRITER_MODE);
    if (fileDescriptor == -1) {
        perror("Erro a criar zona de memória!");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fileDescriptor, data_size) == -1) {
        perror("Erro a definir tamanho a alocar na memória!");
        exit(EXIT_FAILURE);
    }

    Aluno *shared_data; // pointer de aluno
    shared_data = (Aluno *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
    shared_data->synch = 1;

    if (fork()) {
        while (shared_data->synch != 0);

        int max = INT_MIN;
        int min = INT_MAX;
        int avg = 0;
        int i;

        for (i = 0; i < 10; i++) {
            avg = avg + shared_data->disciplinas[i];
            if (shared_data->disciplinas[i] > max) {
                max = shared_data->disciplinas[i];
            }
            if (shared_data->disciplinas[i] < min) {
                min = shared_data->disciplinas[i];
            }
        }

        printf("Nota mais alta foi: %d\n", max);
        printf("Nota mais baixa foi: %d\n", min);
        printf("Média das notas foram: %d\n", avg / NR_DISC);

    } else {

        int i;

        printf("Insira o numero:\n");
        scanf("%d", &shared_data->numero);
        printf("Insira o nome:\n");
        scanf("%s", &shared_data->nome[0]);

        for (i = 0; i < NR_DISC; i++) {
            printf("Insira a nota numero %d: ", i + 1);
            scanf("%d", &shared_data->disciplinas[i]);
        }
        shared_data->synch = 0;
        wait(NULL);
    }

    if (munmap(shared_data, data_size) == -1) {
        perror("No munmap()");
        exit(0);
    }

    if (shm_unlink(FICHEIRO_PARTILHADO) == -1) {
        perror("No unlink()");
        exit(1);
    }

    if (close(fileDescriptor) == -1) {
        perror("No close()");
        exit(0);
    }

    return 0;
}