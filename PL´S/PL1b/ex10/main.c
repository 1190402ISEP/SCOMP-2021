
/**
  10. Write a program that:
− Reads a string from the keyboard-
− After being read, the program should count the number of characters and
display the number on the screen. After that the sleep(20) function
should be executed to simulate the execution of other operations and in
the end display the “Successful execution!” message.
− The user has 10 seconds to input the string. If that time passes and no
string is inputted the “To slow, that is why the program will end!”
message is displayed and execution ends.
a. Implement a solution without creating processes.
Suggestion: you can use the alarm() function.
b. Implement a solution that uses processes to manage the time limit. */

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>


//alinea a
int counter = 0;

void handle_ALARM() {
    if (counter == 0) {
        char e[] = "\nTo slow, that is why the program will end!!\n";
        write(STDOUT_FILENO, e, sizeof(e));
        exit(EXIT_FAILURE);
    }

}

int main() {

    char word[100];
    printf("Insira o texto:\n\n");

    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask);
    act.sa_handler = handle_ALARM;
    sigaction(SIGALRM, &act, NULL);

    alarm(10);
    scanf("%s", word);

    counter = strlen(word);
    printf("%d\n\n", counter);

    pause();
    sleep(20);
    printf("Successful execution!\n");
    return 0;
}

