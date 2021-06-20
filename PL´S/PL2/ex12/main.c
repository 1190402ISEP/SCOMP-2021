/*12. Consider a supermarket with 5 barcode readers distributed along the shop. Every time a customer uses a barcode
*reader the product’s name and its price should be printed on the screen. Simulate this system through processes
*and pipes:
*
*a.The parent process has access to the products database;
*b.Each child process represents a barcode reader;
*c.There is a pipe shared by all 5 child processes to request the product’s information;
*d. The parent process replies to the requesting child with the corresponding product’s information through a pipe 
*that it shares only with it.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>

struct produto
{
    float preco;
    char nome[50];
};

struct produto_request
{
    char nome[50];
    int id;
};

int main(void)
{

    const int LEITURA = 0;
    const int ESCRITA = 1;
    const int NUMERO_DE_FILHOS = 5;

    int i, fd[5][2];
    int fdComum[2];

    struct produto_request pedidoProduto;
    struct produto produto;

    if (pipe(fdComum) == -1)
    {
        perror("PIPE FAILED");
        exit(EXIT_FAILURE);
    }

    struct produto_request produtos_r[5];

    produtos_r[0].id = 0;
    strcpy(produtos_r[0].nome, "Melancia");

    produtos_r[1].id = 1;
    strcpy(produtos_r[1].nome, "Maça");

    produtos_r[2].id = 2;
    strcpy(produtos_r[2].nome, "Cogumelo");

    produtos_r[3].id = 3;
    strcpy(produtos_r[3].nome, "Ervilha");

    produtos_r[4].id = 4;
    strcpy(produtos_r[4].nome, "Chocolate");

    for (i = 0; i < 5; i++)
    { 

        //criação do pipe
        if (pipe(fd[i]) == -1)
        {
            perror("Pipe Failed");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork(); 

        if (pid == -1)
        {
            perror("Fork Failed");
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            close(fd[i][ESCRITA]); 

            write(fdComum[ESCRITA], &produtos_r[i], sizeof(struct produto_request)); 

            if (read(fd[i][LEITURA], &produto, sizeof(struct produto)) > 0)
            {
                printf("Barcode: %d |Nome Produto: %s |Preco: %.2f \n", i + 1, produto.nome, produto.preco);
            }

            close(fd[i][LEITURA]);
            exit(EXIT_SUCCESS);
        }
    }

    close(fdComum[ESCRITA]);

    struct produto produtos[5];

    produtos[0].preco = 3;
    strcpy(produtos[0].nome, "Melancia");

    produtos[1].preco = 3;
    strcpy(produtos[1].nome, "Maça");

    produtos[2].preco = 1;
    strcpy(produtos[2].nome, "Cogumelo");

    produtos[3].preco = 1.5;
    strcpy(produtos[3].nome, "Ervilha");

    produtos[4].preco = 4;
    strcpy(produtos[4].nome, "Chocolate");

    for (i = 0; i < NUMERO_DE_FILHOS; i++)
    {

        close(fd[i][LEITURA]);

        if (read(fdComum[LEITURA], &pedidoProduto, sizeof(struct produto_request)) > 0)
        {

            write(fd[pedidoProduto.id][ESCRITA], &produtos[i], sizeof(struct produto)); //envia a informação sobre o produto no request
        }

        close(fd[i][ESCRITA]);
    }

    close(fdComum[ESCRITA]);

    return 0;
}
/*
*o fdComum é o pipe comum a todos filhos para efetuarem o pedido sobre um produto, sendo que cada um dispõe de um pipe próprio para
*posteriormente receber a informação do processo pai que tem acesso à base de dados.
*/