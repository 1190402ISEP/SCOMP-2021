#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>

volatile sig_atomic_t quantidadeDeProcessos;
volatile sig_atomic_t aguardar = 0;
volatile sig_atomic_t sucesso = -1;
volatile sig_atomic_t filhoBemSucedido;
volatile sig_atomic_t filhoTerminou;
volatile sig_atomic_t ordemDeEducacao = -1;

int filhos[50];

int sigaction(int sig, const struct sigaction *act, struct sigaction *oact);

void handle_USR2(int signo, siginfo_t *sinfo, void *context) {
    quantidadeDeProcessos++;
    char *str = "\n------------------Processo com falha!-----------------\n";
    write(STDOUT_FILENO, str, strlen(str));
}

void handle_USR1(int signo, siginfo_t *sinfo, void *context) {
    sucesso = 0;
    filhoBemSucedido++;
    quantidadeDeProcessos++;

    char *str = "\n------------------Processo retornou com Sucesso!-----------------\n";
    write(STDOUT_FILENO, str, strlen(str));
    aguardar = 1;
}

void handle_CONT(int signo, siginfo_t *sinfo, void *context) {

    char *str = "\n------------------Processo filho continua!-----------------\n";
    write(STDOUT_FILENO, str, strlen(str));
    ordemDeEducacao = 1;
}

void handle_CHLD(int signo, siginfo_t *sinfo, void *context) {
    filhoTerminou++;
    char *str = "\n------------------Filho Terminou!-----------------\n";
    write(STDOUT_FILENO, str, strlen(str));
}


int simulate1(int nbrChld) {
    int pid = getpid();
    int upper = 10000;
    int lower = 0;

    printf("Filho: %d a executar a função SIMULATE1\n", nbrChld);
    fflush(stdout);

    for (int i = 0; i < 100; i++) {
        if (ordemDeEducacao != 1) {
            int rnd = (int) (rand() % (upper - lower + 1)) + lower;
            if (rnd == pid) {
                return 0;
            }
        } else {
            return 1;
        }

    }
    return -1;
}

int simulate2(int nbrChld) {
    int pid = getpid();
    int upper = 10000;
    int lower = 0;

    printf("Filho: %d executando SIMULATE2\n", nbrChld);
    fflush(stdout);

    for (int i = 0; i < 1000; i++) {
        int rnd = (int) (rand() % (upper - lower + 1)) + lower;
        if (rnd == pid) {
            return 0;
        }
    }
    return -1;
}

void stopEveryThing() {
    printf("Algoritmo Ineficiente!\n");
    fflush(stdout);
    for (int i = 0; i < 50; i++) {
        kill(filhos[i], SIGINT);
    }
}

void newTask() {
    printf("Resume on Simulate2\n");
    fflush(stdout);
    for (int i = 0; i < 50; i++) {
        kill(filhos[i], SIGCONT);
    }
}

int main(void) {

    struct sigaction act;

    memset(&act, 0, sizeof(struct sigaction));

    sigfillset(&act.sa_mask);


    act.sa_sigaction = handle_USR1;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);


    act.sa_sigaction = handle_USR2;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR2, &act, NULL);


    act.sa_sigaction = handle_CONT;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGCONT, &act, NULL);


    act.sa_sigaction = handle_CHLD;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGCHLD, &act, NULL);


    pid_t pid;

    quantidadeDeProcessos = 0;
    filhoTerminou = 0;
    filhoBemSucedido = 0;

    for (int i = 0; i < 50; i++) {
        pid = fork();


        if (pid == -1) {
            perror("Fork Failed!");
            exit(EXIT_SUCCESS);
        }

        if (pid > 0) {

            filhos[i] = pid;
        } else {
            printf("Filho %d parte 1\n", i);
            fflush(stdout);
            int parentPID = getppid();
            int valido1 = simulate1(i + 1);
            sleep(2);
            if (valido1 == 0) {
                kill(parentPID, SIGUSR1);
            } else if (valido1 == -1) {
                kill(parentPID, SIGUSR2);
            } else if (valido1 == 1) {
                printf("Father said to move on!");
                fflush(stdout);
            }

            aguardar = 0;
            while (!aguardar);
            printf("Filho %d parte 2\n", i);
            int valido2 = simulate2(i + 1);
            if (valido2 == 0) {
                kill(parentPID, SIGUSR1);
            } else {
                kill(parentPID, SIGUSR2);
            }
            exit(EXIT_SUCCESS);
        }
    }

    while (filhoTerminou != 50) {
        printf("Quantidade de processos = %d\n", quantidadeDeProcessos);
        fflush(stdout);
        if (quantidadeDeProcessos == 25 && sucesso == -1) {
            printf("Entrou 1\n");
            fflush(stdout);
            stopEveryThing();
        } else if (quantidadeDeProcessos < 25 && sucesso == 0) {
            printf("Entrou 2\n");
            fflush(stdout);
            newTask();
        }
        pause();
    }

    while ((pid = wait(NULL)) >= 0);
}
