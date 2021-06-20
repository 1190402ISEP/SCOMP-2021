#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>


void childhandler(int signo) {
    printf("Successful execution!\n\n");
    exit(EXIT_SUCCESS);
}

int main() {
	int TEMPO_ESPERA_SEGUNDOS = 10;
    const int TEMPO_ESPERA_SEGUNDOS_SUCESSO = 20;
    char word[500];

    pid_t pid;
    signal(SIGCHLD, childhandler); /* quando um processo filho termina */
    pid = fork(); /* envia ao pai o sinal SIGCHLD */
    if (pid == 0) {/* filho */
        printf("Insira o texto:\n\n");
        scanf("%500[^\n]", word);

        int counter = 0;
        counter = strlen(word);
        printf("%d\n\n", counter);
	    sleep(TEMPO_ESPERA_SEGUNDOS_SUCESSO);

    } else if (pid > 0) { /* pai */
        sleep(TEMPO_ESPERA_SEGUNDOS);
        printf("To slow, that is why the program will end!!\n\n");
        kill(pid, SIGKILL);
    }

    return 0;
}
