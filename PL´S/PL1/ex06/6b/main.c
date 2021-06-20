#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{

	int i;

	for (i = 0; i < 4; i++)
	{

		if (fork() == 0)
		{
			sleep(1);
			exit(0);
		}
	}

	printf("Pai this is the end.\n");

	return 0;
}