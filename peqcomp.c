/* Ernesto Dornelles Ituassu 2411616 Turma: 3WC */
/* Nome_do_Aluno2 Matricula Turma */


// 1. atribuição
// 2. operação
// 3. desvio
// 4. retorno

#include <peqcomp.h>
funcp peqcomp (FILE *f, unsigned char codigo[]) {
    char buffer[10000];
    char *pbuff;
    int value;

    while (1) {
        if (!fgets(buffer, sizeof buffer, stdin)) break;
        printf("Line contains");
        pbuff = buffer;
        while (1) {
            if (*pbuff == '\n') break;
            value = strtol(pbuff, &pbuff, 10);
            printf(" %d", value);
        }
    printf("\n");
  }
    fread
}   