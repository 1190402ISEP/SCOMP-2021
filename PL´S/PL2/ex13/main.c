#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

int machineFunction(int IDMachine, int productsReceived);
void handleSIGUSR1(int sig, siginfo_t *sigInfo, void *context);
void handleSIGUSR2(int sig, siginfo_t *sigInfo, void *context);

enum MachineCap
{
    Machine1 = 5,
    Machine2 = 5,
    Machine3 = 10,
    Machine4 = 100
};

static const enum MachineCap MachineCap_Map[] = {Machine1, Machine2, Machine3, Machine4};

int main(){

    const int TASKS 5;
    const int MACHINE 4;
    const int INVENTORY 1000;
    const int FD 2;
    const int STRING 500;

    int count;
    int readP = 0;
    int writeP = 1;
    int IDMachine = 1;
    int capacity;
    int inventory = INVENTORY;
    int fileDesc[TASKS][FD];
    int nbBytesWritten;
    int nbBytesRead;
    pid_t pid;
    pid_t parentPid = getpid();

    for (count = 0; count < TASKS; ++count)
    {
        if (pipe(fileDesc[count]) == -1)
        {
            printf("Error encountered while creating pipe.\n");
            return EXIT_FAILURE;
        }
    }

    for (count = 0; count < MACHINE; ++count)
    {
        if (parentPid == getpid())
        {
            pid = fork();
            if (pid == -1)
            {
                printf("Error encountered while creating Child Process.\n");
                return EXIT_FAILURE;
            }
        }

        if (pid == 0)
        {
            capacity = MachineCap_Map[count];
            break;
        }

        ++writeP;
        ++readP;
        ++IDMachine;
    }

    if (pid == 0)
    {
        close(fileDesc[writeP][0]);
        close(fileDesc[readP][1]);

        int productsReceived = 0;
        int tempVar;

        while (inventory > 0)
        {
            nbBytesRead = read(fileDesc[readP][0], &tempVar, sizeof(int));
            if (nbBytesRead == -1)
            {
                printf("Error encountered while reading to pipe.\n");
                kill(getppid(), SIGUSR2);
                exit(EXIT_FAILURE);
            }
            inventory -= tempVar;
            productsReceived += tempVar;
            if (productsReceived == capacity)
            {
                int workedProducts = machineFunction(IDMachine, productsReceived);
                nbBytesWritten = write(fileDesc[writeP][1], &workedProducts, sizeof(int));
                if (nbBytesWritten == -1)
                {
                    printf("Error encountered while writing to pipe.\n");
                    kill(getppid(), SIGUSR1);
                    exit(EXIT_FAILURE);
                }
                productsReceived = 0;
            } else
            {
                tempVar = 0;
                nbBytesWritten = write(fileDesc[writeP][1], &tempVar, sizeof(int));
                if (nbBytesWritten == -1)
                {
                    printf("Error encountered while writing to pipe.\n");
                    kill(getppid(), SIGUSR1);
                    exit(EXIT_FAILURE);
                }
            }
        }
        close(fileDesc[writeP][1]);
        close(fileDesc[readP][0]);
        exit(EXIT_SUCCESS);
    } else
    {
        struct sigaction sig1;
        struct sigaction sig2;
        memset(&sig1, 0, sizeof(struct sigaction));
        memset(&sig2, 0, sizeof(struct sigaction));

        sig1.sa_sigaction = handleSIGUSR1;
        sig1.sa_flags = SA_SIGINFO;
        sigfillset(&sig1.sa_mask);
        sigaction(SIGUSR1, &sig1, NULL);

        sig2.sa_sigaction = handleSIGUSR2;
        sig2.sa_flags = SA_SIGINFO;
        sigfillset(&sig2.sa_mask);
        sigaction(SIGUSR2, &sig2, NULL);

        writeP = 0;
        readP = MACHINE;

        close(fileDesc[writeP][0]);
        close(fileDesc[readP][1]);

        int nbProducts = 0;

        while (inventory > 0)
        {
            int tempVar;
            inventory -= MachineCap_Map[0];
            printf("\tSTORAGE\nPieces Transferred to MACHINE #1: %d\n\n", MachineCap_Map[0]);
            nbBytesWritten = write(fileDesc[writeP][1], &MachineCap_Map, sizeof(int));
            if (nbBytesWritten == -1)
            {
                printf("Error encountered while writing to pipe.\n");
                return EXIT_FAILURE;
            }
            nbBytesRead = read(fileDesc[readP][0], &tempVar, sizeof(int));
            if (nbBytesRead == -1)
            {
                printf("Error encountered while reading from pipe.\n");
                return EXIT_FAILURE;
            }
            nbProducts += tempVar;
            if (tempVar != 0)
            {
                printf("\tSTORAGE\nProducts Packaged Received: %d\n\n", nbProducts);
            }
        }
        close(fileDesc[writeP][1]);
        close(fileDesc[readP][0]);
    }
    return EXIT_SUCCESS;
}

int machineFunction(int IDMachine, int productsReceived) {
    int machineProduct;
    switch (IDMachine)
    {
        case 1:
            printf("\tMACHINE #1\nCutting %d pieces.\n\n", productsReceived);
            machineProduct = productsReceived;
            break;
        case 2:
            printf("\tMACHINE #2\nFolding %d pieces.\n\n", productsReceived);
            machineProduct = productsReceived;
            break;
        case 3:
            printf("\tMACHINE #3\nWelding %d pieces.\n\n", productsReceived);
            machineProduct = productsReceived;
            break;
        case 4:
            printf("\tMACHINE #4\n%d pieces packaged.\nTransferring package to storage.\n\n", productsReceived);
            machineProduct = 1;
            break;
    }
    return machineProduct;
}

void handleSIGUSR1(int sig, siginfo_t *sigInfo, void *context) {
    char str[STRING];
    sprintf(str, "\nChild Process with PID %d encountered an error while writing to the pipe.\n", sigInfo -> si_pid);
    write(STDOUT_FILENO, str, strlen(str));
    exit(EXIT_FAILURE);
}

void handleSIGUSR2(int sig, siginfo_t *sigInfo, void *context) {
    char str[STRING];
    sprintf(str, "\nChild Process with PID %d encountered an error while reading form the pipe.\n", sigInfo -> si_pid);
    write(STDOUT_FILENO, str, strlen(str));
    exit(EXIT_FAILURE);
}
