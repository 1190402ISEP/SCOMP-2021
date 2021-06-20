/*7.Given two integer arrays vec1 and vec2, with 1000 elements each, write a program that creates 5 child processes to concurrently
*sum the two arrays.
*Each child should calculate vec1[i] + vec2[i] on 200 elements, sending all those values to its parent.
*Ensure that each child computes different positions of the arrays.
*The parent process should wait for all the 1000 values and then store them in a result array, in the correct position.
* Use 5 pipes, one for each child.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

int main(int argc, char const *argv[]) {
	int vec1[1000], vec2[1000], fd[5][2], res[1000];
	pid_t pid;
	int i, j, total = 0, soma = 0;

	/*Preenchimento dos vectores*/
	for (i = 0; i < 1000; ++i)	{
		vec1[i] = 1;
		vec2[i] = i;
	}

	for (i = 0; i < 5; i++)	{
		if (pipe(fd[i]) == -1)	{	
			printf("Pipe Failed!\n");
			return 0;
		}
		
		pid = fork();

		if (pid == -1)	{
			printf("Forking Error!\n");
			return 0;
		}

		if (pid == 0)	{
			close(fd[i][0]);
			for (j = (i*200); j < 200 + (i*200); j++){
				total = vec1[j] + vec2[j];
				write(fd[i][1], &total, sizeof(int));
			}
			close(fd[i][1]);
			exit(1);
		} else { 	
			close(fd[i][1]);
			for (j = (i*200); j < 200 + (i*200); j++){
				read(fd[i][0], &soma, sizeof(int));
				res[i] = soma;
				printf("Total: %d\n", res[i]);	
			}
			printf("\n");
		}
		close(fd[i][0]);		
	}
}
