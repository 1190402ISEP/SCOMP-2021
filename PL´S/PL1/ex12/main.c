/**
 * 12. Write a function spawn_childs(int n) which generates n child processes.
 * a) Ensure that the function returns to the child process an index (1 for the first child process created, 2 for the
 * second, 3 for the third and so on) and that to the parent process returns 0.
 * b) Write a program that uses the function spawn_childs() to create 6 child processes. Each child process
 * should exit returning its index value * 2. The parent should wait for all processes created.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int spawn_childs(int n)
{
    pid_t pid;
    int i;
    for (i = 0; i < n; i++)
    {
        pid = fork();
        if (pid < 0)
            return -1;
        else if (pid == 0)
            return i + 1;
    }
    return 0;
}

int main()
{
    int id = spawn_childs(6);
    if (id > 0)
    {
        exit(id * 2);
    }
    int estado;
    int i;
    for (i = 0; i < 6; i++)
    {
        wait(&estado);
        if (WIFEXITED(estado))
        {
            printf("%d\n", WEXITSTATUS(estado));
        }
    }
    return 0;
}