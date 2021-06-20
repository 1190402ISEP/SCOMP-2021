#include "asm.h"

int main() {
    const int SIZE_ARRAY = 1000;
    int i;
    int numbers[SIZE_ARRAY];

    time_t t; /* needed to init. the random number generator (RNG)*/

    /* intializes RNG (srand():stdlib.h; time(): time.h) */
    srand((unsigned) time(&t));

    //Create an array of 1000 integers, initializing it with random values between 0 and 1000;
    for (i = 0; i < SIZE_ARRAY; i++) {
        numbers[i] = rand() % 1000;
    }

    //Create a shared memory area to store an array of 10 integers, each containing the local maximum of 100 values;
    int filedescriptor = shm_open(FICHEIRO_PARTILHADO, OPEN_WRITER_OFLAG, OPEN_WRITER_MODE);

    if (filedescriptor == -1) {
        perror("Erro ao abrir memória Partilhada!\n\n");
        exit(EXIT_FAILURE);
    }

    ftruncate(filedescriptor, TAMANHO_ESTRUTURA);

    Estrutura *memoriaPartilhada = mmap(NULL, TAMANHO_ESTRUTURA, MAP_READ_WRITE, MAP_SHARED, filedescriptor, 0);

    //Create 10 new processes;
    int j;

    for (j = 0; j < NUMERO_DE_FILHOS; j++) {
        pid_t pid = fork();
        if (pid == 0) {
            //Processo Filho

            //Calculate the largest element in the 100 positions;
            int min = 100 * j;
            int max = (100 * j) + 100;
            int valorMax = 0;
            for (int k = min; k < max; k++) {
                if (numbers[k] > valorMax) {
                    valorMax = numbers[k];
                }
            }

            //Write the value found in the position corresponding to the order number (0-9) in the array of local maximum.
            memoriaPartilhada->array[j] = valorMax;

            exit(EXIT_SUCCESS);
        } else if (pid > 0) {
            //Wait until the 10 child processes finish the search for the local maximum;
            wait(NULL);
        }
    }

    //Determine the global maximum;
    int l, globalMax = 0;
    for (l = 0; l < TAMANHO_DO_ARRAY; l++) {
        if (memoriaPartilhada->array[l] > globalMax)
            globalMax = memoriaPartilhada->array[l];
    }

    printf("Valor Máximo Obtido: %d\n\n", globalMax);

    //Eliminate the shared memory area.
    if (munmap(memoriaPartilhada, TAMANHO_ESTRUTURA) < 0) {
        perror("Desfazer o mapping!\n");
        exit(EXIT_FAILURE);
    }

    if (close(filedescriptor) < 0) {
        perror("Error ao fechar()!\n");
        exit(EXIT_FAILURE);
    }

    if (shm_unlink(FICHEIRO_PARTILHADO) < 0) {
        perror("Erro ao Eliminar a memória partilhada!\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}