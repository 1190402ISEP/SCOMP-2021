#include "asm.h"

int main() {
    circular_buffer *circularBuffer;

    //abre o ficheiro de memoria partilhada
    int r, fd = shm_open("/shmtest", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    //especifica o tamanho
    ftruncate(fd, TAMANHO_ESTRUTURA);
    //abre o mapa
    circularBuffer = (circular_buffer *) mmap(NULL, TAMANHO_ESTRUTURA, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0); 

    circularBuffer->bufferLength = 0;
    circularBuffer->write = 0;
    circularBuffer->read = 0;

    int count = 0;
    pid_t pid = fork();

    if (pid < 0) {
        perror("Erro no fork!");
    }

    if (pid > 0) {
        //producer
        while (count != 30) {
	    //verifica se pode escrever ou seja se ainda tem espaço (10 lugares)
            while (circularBuffer->bufferLength >= TAMANHO_DO_BUFFER); 
            circularBuffer->buffer[circularBuffer->write] = count;
            circularBuffer->bufferLength++;
            circularBuffer->write++;

            if (circularBuffer->write == TAMANHO_DO_BUFFER) {
                circularBuffer->write = 0;
            }
            count++;
        }

    } else {
        //consumer
        while (count != 30) { //ate fazer as 30 vezes
            while (circularBuffer->bufferLength == 0); //verifica se pode ler
            printf("O valor é: %d\n", circularBuffer->buffer[circularBuffer->read]);
            circularBuffer->bufferLength--;
            circularBuffer->read++;

            if (circularBuffer->read == TAMANHO_DO_BUFFER) {
                circularBuffer->read = 0;
            }
            count++;
        }
    }

    r = munmap(circularBuffer, TAMANHO_ESTRUTURA);
    //verificar se fechou direito o mapa
    if (r < 0) { exit(1); } 
    r = close(fd);
    //da unlink
    r = shm_unlink("/shmtest"); 

    return 0;
}
