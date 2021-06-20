#include <semaphore.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

#define CAPACIDADE_MAXIMA 300

#define ESPETADOR_VIP 0      //prioridade maior
#define ESPETADOR_ESPECIAL 1 //prioridade intermédia
#define ESPETADOR_NORMAL 2   //prioridade menor

#define NUMERO_DE_SEMAFOROS 4

/*Definir o nome dos semáforos */

/*Controlo de Entradas*/

//Controla a ocupação do cinema
#define NOME_SEMAFORO_ENTRADAS "SemaforoEntradas"

/*Por tipo de Espetador*/

// Controlar a entrada de espetadores do tipo VIP
#define NOME_SEMAFORO_VIP "SemaforoVip"
// Controlar a entrada de espetadores do tipo ESPECIAL
#define NOME_SEMAFORO_ESPECIAL "SemaforoEspecial"
// Controlar a entrada de espetadores do tipo NORMAL
#define NOME_SEMAFORO_NORMAL "SemaforoNormal"



/*Vamos definir um número maior que a capacidade máxima para comprovar que ao atingir
 o máximo não se permitem mais*/

#define TOTAL_ESPETADORES 350

#define TEMPO_VISUALIZACAO_FILME 3
