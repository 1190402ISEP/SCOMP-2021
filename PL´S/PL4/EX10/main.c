#include "asm.h"

int main(void) {
    int fileDescriptor1, fileDescriptor2;
    estruturaPessoa *memoriaPartilhada;

    //-------
    shm_unlink(MEMORIA_PARTILHADA); //remover memória

    fileDescriptor1 = shm_open(MEMORIA_PARTILHADA, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);

    if (fileDescriptor1 < 0) {
        perror("Failed to open shared memory");
        exit(-1);
    }

    fileDescriptor2 = ftruncate(fileDescriptor1, TAMANHO_MEMORIA_PARTILHADA);

    if (fileDescriptor2 < 0) {
        perror("Failed defining the size of the area");
        exit(-1);
    }

    memoriaPartilhada = (estruturaPessoa *) mmap(NULL, TAMANHO_MEMORIA_PARTILHADA, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor1, 0);
    //---------

    sem_t *sem1, *sem2, *sem3, *sem4;

    if ((sem1 = sem_open(SEMAFORO_1, O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
        perror("Erro no sem_open!\n");
        exit(EXIT_FAILURE);
    }

    if ((sem2 = sem_open(SEMAFORO_2, O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
        perror("Erro no sem_open!\n");
        exit(EXIT_FAILURE);
    }

    if ((sem3 = sem_open(SEMAFORO_3, O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
        perror("Erro no sem_open!\n");
        exit(EXIT_FAILURE);
    }

    if ((sem4 = sem_open(SEMAFORO_4, O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
        perror("Erro no sem_open!\n");
        exit(EXIT_FAILURE);
    }
    //-----------

    int op, numberUsers = 0, aux1 = 0, aux2 = 0, aux3 = 0, i, number;
    char aux_nome[SIZE_NAME], aux_morada[SIZE_ADDRESS];

    do {
        memoriaPartilhada->num = 0;

        printf("Opções\n\t1) Consultar utilizador\n\t2) Inserir\n\t3) Consultar todos\n\t0) Sair\n\n");
        scanf("%d", &op);

        switch (op) {
            case 1:

                if (numberUsers != 0) {
                    sem_wait(sem1);

                    printf("Insira o numero: ");
                    scanf("%d", &number);

                    aux1++;
                    if (aux1 == 1) {
                        sem_wait(sem3);
                        sem_wait(sem2);
                    }
                    for (i = 0; i < numberUsers; i++) {
                        if ((memoriaPartilhada + i)->num == number) {
                            printf("Pessoa nº %d:\nNome: %s\nMorada: %s\n", (memoriaPartilhada + i)->num, (memoriaPartilhada + i)->nome,
                                   (memoriaPartilhada + i)->morada);
                        }
                    }
                } else {
                    printf("Lista vazia.\n");
                }

                sem_post(sem1);

                aux1--;
                if (aux1 == 0) {
                    sem_post(sem3);
                    sem_post(sem2);
                }

                break;
            case 2:

                aux2++;
                if (aux2 == 1) {
                    sem_wait(sem3);
                    sem_wait(sem1);
                }

                sem_wait(sem2);

                printf("Nome: ");
                scanf("%s", aux_nome);
                strcpy((memoriaPartilhada + numberUsers)->nome, aux_nome);

                printf("Morada: ");
                scanf("%s", aux_morada);

                strcpy((memoriaPartilhada + numberUsers)->morada, aux_morada);
                (memoriaPartilhada + numberUsers)->num = numberUsers;

                numberUsers++;

                sem_post(sem2);

                aux2--;
                if (aux2 == 0) {
                    sem_post(sem3);
                    sem_post(sem1);
                }
                break;
            case 3:

                aux3++;
                if (aux3 == 1) {
                    sem_wait(sem1);
                    sem_wait(sem2);
                }

                sem_wait(sem3);
                if (numberUsers != 0) {
                    for (i = 0; i < numberUsers; i++) {
                        printf("Pessoa nº %d:\nNome: %s\nMorada: %s\n", (memoriaPartilhada + i)->num, (memoriaPartilhada + i)->nome,
                               (memoriaPartilhada + i)->morada);
                    }
                } else {
                    printf("Lista vazia!\n");
                }
                sem_post(sem3);

                aux3--;
                if (aux3 == 0) {
                    sem_post(sem1);
                    sem_post(sem2);
                }

                break;

            case 0:
                break;

            default:
                printf("Error!\n");
        }

    } while (op != 0);


    //Fechar e apagar os semaforos criados
    if (sem_close(sem1) == -1) {
        perror("Erro no sem_close!\n");
        exit(EXIT_FAILURE);
    }

    if (sem_unlink(SEMAFORO_1) == -1) {
        perror("Erro no sem_unlink!\n");
        exit(EXIT_FAILURE);
    }

    if (sem_close(sem2) == -1) {
        perror("Erro no sem_close!\n");
        exit(EXIT_FAILURE);
    }

    if (sem_unlink(SEMAFORO_2) == -1) {
        perror("Erro no sem_unlink!\n");
        exit(EXIT_FAILURE);
    }

    if (sem_close(sem3) == -1) {
        perror("Erro no sem_close!\n");
        exit(EXIT_FAILURE);
    }

    if (sem_unlink(SEMAFORO_3) == -1) {
        perror("Erro no sem_unlink!\n");
        exit(EXIT_FAILURE);
    }

    if (sem_close(sem4) == -1) {
        perror("Erro no sem_close!\n");
        exit(EXIT_FAILURE);
    }

    if (sem_unlink(SEMAFORO_4) == -1) {
        perror("Erro no sem_unlink!\n");
        exit(EXIT_FAILURE);
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

    if (close(fileDescriptor1) < 0) {
        printf("Error close()!\n");
        exit(EXIT_FAILURE);
    }

    if (close(fileDescriptor2) < 0) {
        printf("Error close()!\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}