/**8.Compile and execute the code below, sending the main process to the background (in Linux, you tell the shell to
 * execute a process in the background by adding “&” at the end). Observe that the parent process will execute a
 * loop forever.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>


int main() {
    pid_t p;

    if (fork() == 0) {
        printf("PID = %d\n", getpid());
        exit(0);
    }

    if ((p = fork()) == 0) {
        printf("PID = %d\n", getpid());
        exit(0);
    }

    printf("Parent PID = %d\n", getpid());

    printf("Waiting... (for PID=%d)\n", p);
    waitpid(p, NULL, 0);

    printf("Enter Loop...\n");
    while (1); /* Infinite loop */

    return 0;
}
/**
 * a) In the shell, list the running processes with ps (look for the PIDs printed). Are all parent and child processes
 * listed? Why?
 */
/*
 *OUTPUT:
 * Parent PID = 21747
 * Waiting... (for PID=21749)
 * PID = 21748
 * PID = 21749
 *
 * SHELL
 *
 * PID TTY          TIME CMD
 * 3690 pts/0    00:00:00 bash
 * 21747 pts/0    00:03:04 main
 * 21748 pts/0    00:00:00 main <defunct>
 * 22119 pts/0    00:00:01 main
 * 22120 pts/0    00:00:00 main <defunct>
 * 22126 pts/0    00:00:00 ps
 *
 *
 * Imprime o valor do processo pai e de todos os outros processos porque no printf imprimimos o pid do processo que foi
 * criado no fork(), estando então listados 4 processos.
 */

/**
 * b) Is there anything particular about the child process? Explain.
 */
/* O processo filho vai estar em estado de waiting continuamente, pois pela funcao waitpid(), como a referência de
 * memória passada e nula, o processo pai fica à espera que o processo filho termine.
*/

/**
 * c) Kill the parent process: kill -KILL <PID>
 */
/*
*    PID TTY          TIME CMD
 * 3690 pts/0    00:00:00 bash
 * 23650 pts/0    00:00:45 main
 * 23651 pts/0    00:00:00 main <defunct>
 * 23674 pts/0    00:00:28 main
 * 23675 pts/0    00:00:00 main <defunct>
 * 23684 pts/0    00:00:19 main
 * 23685 pts/0    00:00:00 main <defunct>
 * 23694 pts/0    00:00:00 ps
* [3]   Terminado à força     ./main
*/


