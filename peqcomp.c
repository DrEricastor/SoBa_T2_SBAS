/* Ernesto Dornelles Ituassu 2411616 Turma: 3WC */
/* Nome_do_Aluno2 Matricula Turma */


// 1. atribuição
// 2. operação
// 3. desvio
// 4. retorno
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "peqcomp.h"


funcp peqcomp (FILE *f, unsigned char codigo[]) {
    printf("Entrando na peqcomp()\n");
    char bufferLinha[100];
    char *pbuff;
    int value;

    printf("Iniciando leitura do Arquivo\n\n");

    int numLinha = 1;
    while (fgets(bufferLinha, sizeof(bufferLinha), f) != NULL) {
        if (strlen(bufferLinha) == 0) {
            printf("Linha vazia encontrada. Parando a leitura.\n");
            break;
        }
        printf("%d: ", numLinha++);
        printf(bufferLinha);
    }
    printf("\n");
}