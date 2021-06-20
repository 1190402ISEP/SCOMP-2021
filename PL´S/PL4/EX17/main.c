#include "asm.h"

int main() {

    /*Inicializar os semáforos*/


    int valoresIniciaisSemaforos[4] = {0, 0, 0, CAPACIDADE_MAXIMA};
    char nomesSemaforos[NUMERO_DE_SEMAFOROS][80] = {NOME_SEMAFORO_VIP, NOME_SEMAFORO_ESPECIAL, NOME_SEMAFORO_NORMAL,
                                                NOME_SEMAFORO_ENTRADAS};

    sem_t *semaforos[NUMERO_DE_SEMAFOROS];

    int i;

    for (size_t i = 0; i < NUMERO_DE_SEMAFOROS; i++) {
        if ((semaforos[i] = sem_open(nomesSemaforos[i], O_CREAT | O_EXCL, 0644, valoresIniciaisSemaforos[i])) ==
            SEM_FAILED) {
            perror("Erro no sem_open()!\n");
            exit(EXIT_FAILURE);
        }
    }

    /*Cálculo do total de espetadores por tipo*/
    int k, totalEspetadoresVip = 0, totalEspetadoresEspeciais = 0, totalEspetadoresNormais = 0;

    for (k = 0; k < TOTAL_ESPETADORES; k++) {
        if (k % 3 == ESPETADOR_VIP) {
            totalEspetadoresVip++; /* aumenta os VIPs na fila de espera */
        } else if (k % 3 == ESPETADOR_ESPECIAL) {
            totalEspetadoresEspeciais++; /* aumenta os especias na fila de espera aumenta */
        } else {
            totalEspetadoresNormais++;
        }
    }

    pid_t pid;

    int contador, tipoEspetador;
    for (contador = 0; contador < TOTAL_ESPETADORES; contador++) {
        pid = fork();

        if (pid == -1) {
            perror("Erro no fork()\n");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) { //Processo Filho

            while (1) {
                tipoEspetador = contador % 3;
                if (tipoEspetador == ESPETADOR_VIP) {
                    sem_post(semaforos[0]); //Deixa entrar VIP (desbloqueia semáforo)
                    sem_wait(semaforos[0]); //bloqueia semáforo
                    sem_wait(semaforos[3]); //Menos lugares disponíveis
                    printf("+1 VIP- id:%d\n", contador);
                    totalEspetadoresVip--; //Menos 1 vip para entrar
                } else if (tipoEspetador == ESPETADOR_ESPECIAL) {
                    sem_post(semaforos[1]); //Deixa entrar ESPECIAL (desbloqueia semáforo)
                    sem_wait(semaforos[1]); //bloqueia semáforo
                    sem_wait(semaforos[3]); //Menos lugares disponíveis

                    printf("+1 Especial- id:%d\n", contador);
                    totalEspetadoresEspeciais--; //Menos 1 especial para entrar
                } else {                           //Espetador Normal
                    sem_post(semaforos[2]); //Deixa entrar NORMAL (desbloqueia semáforo)
                    sem_wait(semaforos[2]); //bloqueia semáforo
                    sem_wait(semaforos[3]); //Menos lugares disponíveis
                    printf("+1 Normal- id:%d\n", contador);
                    totalEspetadoresNormais--; //Menos 1 normal para entrar
                }
                /*Garantir que eles ficam algum tempo a executar*/
                sleep(TEMPO_VISUALIZACAO_FILME);

                /*Quando termina de ver o filme sai-> +1 lugar disponivel*/
                sem_post(semaforos[3]);

                printf("+1 lugar disponível, o espetador %d terminou de ver o filme\n\n", contador);

                /*Por fim deixamos entrar mais um porque temos +1 lugar*/
                //primeiro vip:
                if (totalEspetadoresVip > 0) {
                    sem_post(semaforos[0]); //+1 vip
                } else if (totalEspetadoresEspeciais > 0 && totalEspetadoresVip <= 0) { //Segundo Especial
                    sem_post(semaforos[1]);
                } else { //Por fim normais
                    sem_post(semaforos[2]);
                }

                exit(EXIT_SUCCESS);
            }
        }
    }
    /*Processo pai espera pelos filhos*/
    for (i = 0; i < TOTAL_ESPETADORES; i++) {
        wait(NULL);
    }

    /*Unlink dos Semáforos*/
    for (i = 0; i < NUMERO_DE_SEMAFOROS; i++) {
        if (sem_unlink(nomesSemaforos[i]) == -1) {
            perror("Erro no sem_unlink()!\n");
            exit(EXIT_FAILURE);
        }
    }

    return EXIT_SUCCESS;
}