#include <stdio.h>

#include "peqcomp.h"

int main() {
    printf("Codigo iniciado\n");
    FILE* arquivo = fopen("testes SBAS/teste5.txt", "r");
    if (arquivo == NULL) {
        return -1;
    }
    printf("Arquivo aberto corretamente\n");

    char codigo[1000];
    printf("Vetor codigo[1000] criado\n");

    peqcomp(arquivo, codigo);

    printf("Fim do Teste \n \n");
}