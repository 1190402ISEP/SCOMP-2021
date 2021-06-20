#include "asm.h"

/*                                                  ANÁLISE:
 * ORDEM DE EXECUÇÃO NECESSÁRIA:
 *
 *P1-A  |-> printf(“Sistemas ”);
 *P2-A  |-> printf(“de ”);
 *P3-A  |-> printf(“Computadores -”);
 *P1-B  |-> printf(“a ”);
 *P2-B  |-> printf(“melhor “);
 *P3-B  |-> printf(“disciplina! ”);

Precisamos de 3 semáforos que controlem o fluxo do printf() passando pela ordem já referida e no esquema em anexo.
Cada um dos semáforos irá controlar o fluxo de execução do programa.

 */
int spawn_childs(int n);


int main() {

    char idSemaforo[TOTAL_SEMAFOROS_CONTROLO_FLUXO][80]={SEMAFORO_1,SEMAFORO_2,SEMAFORO_3}; //Nome do ficheiro do semáforo no asm.h, o id é o nome e não
    //o id para o sistema

    sem_t *arraySemaforos[TOTAL_SEMAFOROS_CONTROLO_FLUXO];


    //Inicializar os semáforos

    //O semáforo 1 vai inicializar com o valor 1 para iniciar desbloqueado os restantes iniciam a 0

    if ((arraySemaforos[0] = sem_open(idSemaforo[0], O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
        perror("Erro no sem_open()\n\n");
        exit(EXIT_FAILURE);
    }

    if ((arraySemaforos[1] = sem_open(idSemaforo[1], O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
        perror("Erro no sem_open()\n\n");
        exit(EXIT_FAILURE);
    }
    if ((arraySemaforos[2] = sem_open(idSemaforo[2], O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
        perror("Erro no sem_open()\n\n");
        exit(EXIT_FAILURE);
    }

    //Inicializar os processos filhos

    int ordemCriacao = spawn_childs(NUMERO_DE_PROCESSOS_FILHOS);


    /*
     * Como no exemplo dos slides um processo quando termina deve desbloquear outro e esperar que o 3 o volte a
     * desbloquear no caso do 1 ou seja é quase que uma operação em ciclo p1->p2->p3->p1->p2->p3
     */

    if (ordemCriacao == 1) {
        sem_wait(arraySemaforos[ID_SEMAFORO_1]);

        printf("Sistemas \n");

        sem_post(
                arraySemaforos[ID_SEMAFORO_2]); //desbloqueamos o segundo e o primeiro fica bloqueado ate o 3 o desbloquear
        //e assim sucessivamente
        //-----------------------------------------------
        sem_wait(arraySemaforos[ID_SEMAFORO_1]);

        printf("a \n");

        sem_post(arraySemaforos[ID_SEMAFORO_2]);

        exit(1);
    }

    if (ordemCriacao == 2) {
        sem_wait(arraySemaforos[ID_SEMAFORO_2]);

        printf("de \n");

        sem_post(arraySemaforos[ID_SEMAFORO_3]);
        //-----------------------------------------------
        sem_wait(arraySemaforos[ID_SEMAFORO_2]);

        printf("melhor \n");

        sem_post(arraySemaforos[ID_SEMAFORO_3]);

        exit(2);
    }

    if (ordemCriacao == 3) {
        sem_wait(arraySemaforos[ID_SEMAFORO_3]);

        printf("Computadores - \n");

        sem_post(arraySemaforos[ID_SEMAFORO_1]);//Voltamos a iniciar o ciclo no p1
        //-----------------------------------------------
        sem_wait(arraySemaforos[ID_SEMAFORO_3]);

        printf("disciplina! \n");

        exit(3);
    }
    //O pai deve aguardar por todos os filhos

    for (int i = 0; i < NUMERO_DE_PROCESSOS_FILHOS; i++) {
        wait(NULL);
    }

    //Fechar os semáforos

    if (sem_close(arraySemaforos[0]) == -1) {
        perror("Erro no sem_close()!\n");
        exit(EXIT_FAILURE);
    }
    if (sem_close(arraySemaforos[1]) == -1) {
        perror("Erro no sem_close()!\n");
        exit(EXIT_FAILURE);
    }
    if (sem_close(arraySemaforos[2]) == -1) {
        perror("Erro no sem_close()!\n");
        exit(EXIT_FAILURE);
    }


    //Unlink

    if (sem_unlink(idSemaforo[0]) == -1) {
        perror("Erro no sem_unlink()!\n");
        exit(EXIT_FAILURE);
    }

    if (sem_unlink(idSemaforo[1]) == -1) {
        perror("Erro no sem_unlink()!\n");
        exit(EXIT_FAILURE);
    }

    if (sem_unlink(idSemaforo[2]) == -1) {
        perror("Erro no sem_unlink()!\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}


int spawn_childs(int n) {
    pid_t pid;
    int i;
    for (i = 0; i < n; i++) {
        pid = fork();
        if (pid < 0)
            return -1;
        else if (pid == 0)
            return i + 1;
    }
    return 0;
}