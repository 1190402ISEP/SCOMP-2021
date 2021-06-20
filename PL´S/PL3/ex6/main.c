#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h> /* For constants “mode” */
#include <fcntl.h> /* For constants O_* */
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

#define NR_OPERACOES 1000000

typedef struct {
    int numeros[NR_OPERACOES];
} Numeros;

#define TAMANHO_DA_ESTRUTURA sizeof(Numeros)

#define FICHEIRO_DA_MEMORIA_PARTILHADA "/EX6"			//nome do ficheiro de memoria partilhada, que comeca com '/'

int main(void) {
	
	//variaveis para contar os tempos de execucao de ambos os mecanismos de transferencia de dados (pipes e memoria partilhada)
	time_t startShm,endShm;
	double tempoShm;
	
	//criacao do processo filho
    pid_t pid = fork();

	//verificacao do fork
    if (pid == -1) {
        printf("Erro no fork");
        return -1;
    }

    if (pid == 0) {/*processo filho enche o array com os numeros aleatorios*/
			
			time (&startShm);
			//cria e abre a zona de memoria
			int fd = shm_open("/EX6", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);

			//verificacao da criacao da zona de memoria partilhada
			if (fd == -1) {
				perror("Memória partilhada mal criada!");
				return -1;
			}

			//alocacao do numero de bytes a utilizar na zona de memoria partilhada
			ftruncate(fd, TAMANHO_DA_ESTRUTURA);
	
			//mapeamento da zona de memoria
			Numeros *estrutura = (Numeros *) mmap(NULL, TAMANHO_DA_ESTRUTURA, PROT_READ | PROT_WRITE,MAP_SHARED, fd, 0);

			//declaracao da variavel iterativa e do apontador da estrutura
			int i;
			int *ptr = estrutura->numeros;
			
			//gera e coloca os numeros
			for (i = 0; i < NR_OPERACOES; i++) {
				*ptr = (rand() % 20) + 1;
				//printf("iteracao %d,nr gerado %d\n",i,*ptr);
				ptr++;
			}
			
			//desmapeamento da memoria
			munmap(estrutura, TAMANHO_DA_ESTRUTURA);
			close(fd);
			exit(1);
		
    } else{
			int estado_do_proceso_filho; 								//se retornou sem erros
			waitpid(pid, &estado_do_proceso_filho, 0);			 		//guarda em estado_do_proceso_filho o estado do processo filho /*processo pai apenas recebe os numeros e conta o tempo total*/
			
		if (WIFEXITED(estado_do_proceso_filho)){
				
			Numeros recetor;
			int *ptrRecetor = (int *) recetor.numeros;
			
			 //cria e abre a zona de memoria
			int fileDescriptor = shm_open(FICHEIRO_DA_MEMORIA_PARTILHADA,O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);

			//verificacao da criacao da zona de memoria partilhada
			if (fileDescriptor == -1) {
				perror("ERRO na abertura da memoria");
				exit(EXIT_FAILURE);
			}

			//alocacao do numero de bytes a utilizar na zona de memoria partilhada
			ftruncate(fileDescriptor, TAMANHO_DA_ESTRUTURA);

			//mapeamento da zona de memoria
			Numeros *memoriaPartilhada;
			memoriaPartilhada = (Numeros *) mmap(NULL, TAMANHO_DA_ESTRUTURA,PROT_READ|PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
			
			//declaracao da variavel iterativa e apontador da estrutura de memoriaPartilhada
			int i;
			int *ptr;
			ptr = (int *) memoriaPartilhada->numeros;
			
			//recebe os numeros
			for (i = 0; i < NR_OPERACOES; i++) {
				*ptrRecetor = *ptr;
				ptr++;
				ptrRecetor++;
			}
			
			//tempo final com memoria partilhada
			time (&endShm);
			
			//diferenca de tempo inicial e final que sera o tempo total de uso da memoria partilhada
			tempoShm = difftime (endShm,startShm);
			
			//impressao do tempo com Shm
			printf(" Tempo com shared memory: %.2f segundos \n",tempoShm/1000000000);//teve-se de dividir por 10^9 devido ao tempo devolvido ser em nanosegundos
	
			//desmapeamento da memoria
			munmap(memoriaPartilhada, TAMANHO_DA_ESTRUTURA);
			close(fileDescriptor);
			exit(2);
			
		}
		
    }
    
	//deslinka-se a memoria partilhada e verifica-se os erros por fim
	int verificacao = shm_unlink(FICHEIRO_DA_MEMORIA_PARTILHADA);
	
    if (verificacao == -1) {
        perror("Erro ao deslinkar a memoria(remover o ficheiro)");
        return -1;
    }


    return 0;
}
