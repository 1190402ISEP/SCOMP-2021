#include "asm.h"

int main() {
    //Estruturas de dados partilhados
    int fileDescriptor;
    sem_t *semaphore;
    estruturaMemoriaPartilhada *memoriaPartilhada;



    //Criar memória partilhada
    fileDescriptor = shm_open(MEMORIA_PARTILHADA, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);

    if (fileDescriptor == -1) {
        perror("ERRO no shm_open()!\n\nA terminar execução!\n\n");
        exit(EXIT_FAILURE);
    }


    ftruncate(fileDescriptor, TAMANHO_MEMORIA_PARTILHADA);

    memoriaPartilhada = (estruturaMemoriaPartilhada *) mmap(NULL, TAMANHO_MEMORIA_PARTILHADA, PROT_READ | PROT_WRITE,
                                                            MAP_SHARED, fileDescriptor, 0);

    //Verificar se a criação de memória partilhada foi bem sucedida
    if (memoriaPartilhada == NULL) {
        perror("ERRO no mmap()!\n\nA terminar execução!\n\n");
        exit(EXIT_FAILURE);
    }

    // Criar o Semáforo
    if ((semaphore = sem_open(SEMAFORO, O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Erro no sem_open()");
        exit(EXIT_FAILURE);
    }


    //Criar Processos Filho

    pid_t pid;

    srand(time(0));


    for (int i = 0; i < NUMERO_PROCESSOS_FILHO; i++) {
        pid = fork();

        if (pid < 0) {
            perror("Erro no fork()\n\n");
            exit(EXIT_FAILURE);
        }

        if (pid > 0) { /*Processo Pai*/
            wait(NULL); //deve aguardar

        } else if (pid == 0) { /*Processo Filho ->Dá-se a entrada na zona crítica é necessário bloquear o acesso*/

            //Bloquear acesso por outros processos

            sem_wait(semaphore);

            //Operações

            sprintf(memoriaPartilhada->string[memoriaPartilhada->indice], "%s %d",
                    "I’m the Father - with PID ", getpid());

            memoriaPartilhada->indice++;

            if (memoriaPartilhada->indice >= NUMERO_DE_STRINGS) {
                memoriaPartilhada->indice = 0;
            }

            sleep((rand() % (TEMPO_MAXIMO_ESPERA)) + 1);


            //Libertar processos à espera pelo Semáforo ( increments (unlocks) the semaphore)
            sem_post(semaphore);
            /*
                * closes the named semaphore referred to by semaphore,
                * allowing any resources that the system has allocated to the
                * calling process for this semaphore to be freed.
            */
            sem_close(semaphore);
            exit(EXIT_SUCCESS);
        }
    }
    //Imprimir tudo da memória partilhada (Processo Pai)
    int j;
    for (j = 0; j < memoriaPartilhada->indice; j++) {
        printf("%s \n", memoriaPartilhada->string[j]);
    }

    //LIBERTAR MEMÓRIAS,UNLINK...

    if (shm_unlink(MEMORIA_PARTILHADA) < 0) {
        printf("Error at shm_unlink()!\n");
        exit(EXIT_FAILURE);
    }

    if (munmap(memoriaPartilhada, TAMANHO_MEMORIA_PARTILHADA) < 0) {
        printf("Error  munmap()!\n");
        exit(EXIT_FAILURE);
    }

    if (close(fileDescriptor) < 0) {
        printf("Error  close()!\n");
        exit(EXIT_FAILURE);
    }

    sem_unlink(SEMAFORO);

    return EXIT_SUCCESS;
}

/**
 *Site Consultado:
 *
 * https://man7.org/linux/man-pages/man3/
*/