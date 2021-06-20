/*9. A retail company stores the sales of the previous month in a sales array and wants to know which products have
*sold more than 20 units in a single sale. As the array size is quite large (50 000 elements) a concurrent solution to
the problem is necessary. Write a program that creates 10 child processes, with each one responsible for searching
*5000 sale records. Consider that each sale record is as follows:
*    typedef struct{
*    int customer_code;
*    int product_code;
*    int quantity;
*    }
*Whenever a child finds a product that meets the search criteria it should send the product’s code to the parent
*process. The parent process should record those codes in the array products, printing it whenever all child processes have been terminated. 
*The values on the sales array should be filled with random values.
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


typedef struct
{
    int customer_code;
    int product_code;
    int quantity;
} sale;

#define READ 0
#define WRITE 1
#define PRO_CHILD 10
#define PIPES 10
#define PRODUCTS 50000
#define MAX 20

int main()
{
    int i;
	int k;
	int idxProduct = 0;
	int codeProduct;
	int state_child;
	
    pid_t pid;
	
	int productsProcess = PRODUCTS/PRO_CHILD;

    int fd[PIPES * 2];

    sale sales[PRODUCTS];

    int products[PRODUCTS];

    for (i = 0; i < PRODUCTS; i++){
        sales[i].customer_code = rand()%256;
        sales[i].product_code = rand()%256;
        sales[i].quantity = rand()%256;		
        products[i] = 0;
    }

    for (i = 0; i < PIPES; i++){
        if (pipe(fd + i * 2) == -1){
            perror("Pipe failed\n");
            return 1;
        }
    }

	for (i = 0; i < PRO_CHILD; i++){
        pid = fork();
        if(pid==0){ 
            close(fd[i*2]);
            int first = (productsProcess * i)+1;
            int last = first + productsProcess;
            for (k = first; k < last; k++){
                if (sales[k].quantity > MAX){
                    write(fd[i*2+WRITE], &sales[k].product_code, sizeof(sales[k].product_code));
                }
            }
            close(fd[i*2+WRITE]);
            exit(0);
        }else if (pid>0){ 
            close(*(fd + i * 2 + WRITE)); 
            while ((read(fd[i*2], &codeProduct, sizeof(codeProduct))) > 0){
                products[idxProduct] = codeProduct;
                codeProduct=0;
                idxProduct++;
            }
            close(fd[i*2]); 
        }
    }

    wait(&state_child);

    if (WIFEXITED(state_child)){
        for (i = 0; i < idxProduct ; i++){
			if (products[i] > 0){
			    printf("Product Code with more than 20 sales: %d;\n", products[i]); 
            }
        }
    }
    return 0;
}
