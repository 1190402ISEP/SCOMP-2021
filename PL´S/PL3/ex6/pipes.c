#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#define TAMANHO_DO_ARRAY 10

typedef struct{

int numeros[TAMANHO_DO_ARRAY];
	
}Numeros;


#define LER 0
#define ESCREVER 1

//funcao que retorna o tempo de hoje
double what_time_is_it()
{
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec + now.tv_nsec*1e-9;
}


int main(void){

	//variaveis para contar os tempos de execucao de ambos os mecanismos de transferencia de dados (pipes e memoria partilhada)
	//terminacao do mecanismo de pipe
	double startPipe = what_time_is_it();

	//pipe e descritor de ficheiro
	pid_t p;
	int fd[2];
	
	//verificacao do pipe
    if (pipe(fd) == -1) {
        perror("Erro a criar o pipe!");
        exit(1);
    }
	
	//criacao do processo filho (o client)
    p = fork();

    if (p < 0) {
        perror("O fork falhou!");
        exit(EXIT_FAILURE);
    }
	
	
	 if (p > 0) { //caso seja o processo pai

		Numeros arrayParaReceber;
		int numeroTemp;
		
		//o pai vai ler por isso fechamos o descritor de escrita
	   close(fd[ESCREVER]);

		// o pai lê e o filho escreve para o pipe o array
		
		int i;
		for(i=0;i<TAMANHO_DO_ARRAY;i++){
			
		int verificacao = read(fd[LER],&numeroTemp,sizeof(int));
		
		//passagem do numero inserido para o array
		arrayParaReceber.numeros[i]=numeroTemp;
			
			if(verificacao==-1){
				perror("Read mal feito!");
				exit(EXIT_FAILURE);
			}
			
		}
      
        //close de tudo o que foi utilizado por segurança
		close(fd[LER]);
		

    } else { //se não é o processo pai é o processo filho

		//estrutura que vai receber o que se faz
		Numeros arrayParaEnviar;

		//geracao dos numeros para preenchimento do array
	
		int i;
		for(i=0;i<TAMANHO_DO_ARRAY;i++){
			arrayParaEnviar.numeros[i]=(rand()%33)+1;
		}
		
        /*o processo filho vai escrever pelo pipe o array fornecido*/
		
		
		//fechamos o descritor de leitura porque vamos escrever para o pipe
        close(fd[LER]);
		
		for(i=0;i<TAMANHO_DO_ARRAY;i++){
		
		int verificacao = write(fd[ESCREVER],&arrayParaEnviar.numeros[i],sizeof(int));
		
			if(verificacao==-1){
				perror("Erro na escrita para o pipe!");
				exit(EXIT_FAILURE);
			}
			
			
		}
		
        //closes por segurança e término do processo filho
        close(fd[ESCREVER]);
        exit(0);
    }
			
	//impressao do tempo com Pipe
	printf(" Tempo com Pipes: %.9lf segundos \n",what_time_is_it()-startPipe);


return 0;
}