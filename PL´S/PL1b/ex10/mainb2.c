#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

char str1[1000];

void handle_USR1(int signo){
    if(str1[0] == '\0'){
       printf("\n\n Too slow, that is why the program will end \n\n");/*se a primeira posição está vazia então não foi introduzido nada*/
       exit(1);
    }
}
    


int main() {

struct sigaction act;

pid_t pid;
int n=0;

memset(&act,0,sizeof(struct sigaction));
sigemptyset(&act.sa_mask);


act.sa_handler = handle_USR1;
act.sa_flags = SA_SIGINFO;
sigaction(SIGUSR1, &act, NULL);

pid_t ppid = getpid();

pid = fork();

if(pid>0){
    printf("Insert the string\n");
    scanf("%s", str1);

    while( str1[n] != '\0'){ /* fim do "array" preenchido */
        n++;
    }
}else{
    sleep(10);
    kill(ppid, SIGUSR1);
    exit(1);
}


printf("String has %d characters\n",n);

sleep(20);

printf("\nSuccessful execution!\n");

return 0;



}