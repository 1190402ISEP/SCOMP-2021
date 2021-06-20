#include "asm.h"

int main() {

    const int SIZE_ARRAY = 1000;
    int i;
    int numbers[SIZE_ARRAY];
    time_t t; /* needed to init. the random number generator (RNG)*/

    int filedescriptor[2];

    /* intializes RNG (srand():stdlib.h; time(): time.h) */
    srand((unsigned) time(&t));

    //Create an array of 1000 integers, initializing it with random values between 0 and 1000;
    for (i = 0; i < SIZE_ARRAY; i++) {
        numbers[i] = rand() % 1001;
    }

    //Create a shared memory area to store an array of 10 integers, each containing the local maximum of 100 values;
    if (pipe(filedescriptor) == -1) {
        perror("Erro ao criar o pipe!\n\n");
        exit(EXIT_FAILURE);
    }

    int filho;

    for (filho = 0; filho < NUMERO_DE_FILHOS; filho++) {
        pid_t pid = fork();
        if (pid == 0) {//Processo Filho
		
			//O filho apenas vai escrever para o pipe
			if(close(filedescriptor[READ])==-1){
				perror("Close falhou!\n\n");
				exit(EXIT_FAILURE);
			}

            //Calculate the largest element in the 100 positions;
            int indiceMinimoRelativo = 100 * filho;
            int indiceMaximoRelativo = (100 * filho) + 100;
            int maximoLocal = 0;
			int posicao;
			
            for ( posicao = indiceMinimoRelativo; posicao < indiceMaximoRelativo; posicao++) {
                if (numbers[posicao] > maximoLocal) {
                    maximoLocal = numbers[posicao];
                }
            }

            //escrever o máximo local para dentro do pipe
			int verificacao = write(filedescriptor[WRITE],&maximoLocal,sizeof(int));
          
		  if(verificacao==-1){
			perror("Erro de escrita no pipe!\n\n");
			exit(EXIT_FAILURE);
		  }

			verificacao=close(filedescriptor[WRITE]);
			
		 if(verificacao==-1){
			perror("Erro ao fechar o pipe!\n\n");
			exit(EXIT_FAILURE);
		  }
			
            exit(EXIT_SUCCESS);
			
        } else if (pid > 0) {
            //Wait until the 10 child processes finish the search for the local maximum;
            wait(NULL);
        }
    }

    //o pai recebe do pipe cada maximo local, por isso só vai ler, daqui que fechamos o descritor de escrita
    if(close(filedescriptor[WRITE])==-1){
	perror("Error ao fechar o file descriptor!\n");
        exit(EXIT_FAILURE);
    }
	
	
    //calculo do maximo global;
    int indice, maximoGlobal = 0,maximoLocal=0;
    for (indice = 0; indice < TAMANHO_DO_ARRAY; indice++) {
	int verificacao = read(filedescriptor[READ],&maximoLocal,sizeof(int));
	if(verificacao==-1){
		perror("Error ao ler do pipe!\n");
		exit(EXIT_FAILURE);
	}
	
        if (maximoLocal > maximoGlobal){
		maximoGlobal = maximoLocal;
	}
    }

    printf("Valor Máximo Obtido: %d\n\n", maximoGlobal);

    if (close(filedescriptor[READ]) == -1) {
        perror("Error ao fechar o file descriptor!\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}