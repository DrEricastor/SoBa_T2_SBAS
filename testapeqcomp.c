// testapeqcomp.c - Versão Universal

#include <stdio.h>
#include <stdlib.h> // Para atoi() e exit()
#include "peqcomp.h"

int main(int argc, char *argv[]) {
    // --- 1. Validação dos Argumentos de Linha de Comando ---
    if (argc < 2) {
        fprintf(stderr, "Erro: Argumentos insuficientes.\n");
        fprintf(stderr, "Uso: %s <arquivo.sbas> [param1] [param2] [param3]\n", argv[0]);
        return 1;
    }

    char* nome_arquivo = argv[1]; // O primeiro argumento é sempre o nome do arquivo.
    
    // --- 2. Compilação do Código SBas ---
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    unsigned char codigo[2048];
    funcp funcao_compilada_generica; // Usamos o tipo genérico retornado por peqcomp

    funcao_compilada_generica = peqcomp(arquivo, codigo);
    fclose(arquivo);

    // --- 3. Execução Dinâmica com Base no Número de Parâmetros ---
    int num_params = argc - 2; // Calcula quantos parâmetros para a função foram passados
    int resultado;

    printf("Executando '%s' com %d parametro(s)...\n", nome_arquivo, num_params);

    switch (num_params) {
        case 0: { // NENHUM PARÂMETRO
            funcp func = (funcp)funcao_compilada_generica;
            resultado = func();
            break;
        }
        case 1: { // UM PARÂMETRO
            funcp func = (funcp)funcao_compilada_generica;
            int p1 = atoi(argv[2]); // Converte o 1º param de string para int
            printf("  p1 = %d\n", p1);
            resultado = func(p1);
            break;
        }
        case 2: { // DOIS PARÂMETROS
            funcp func = (funcp)funcao_compilada_generica;
            int p1 = atoi(argv[2]);
            int p2 = atoi(argv[3]);
            printf("  p1 = %d, p2 = %d\n", p1, p2);
            resultado = func(p1, p2);
            break;
        }
        case 3: { // TRÊS PARÂMETROS
            funcp func = (funcp)funcao_compilada_generica;
            int p1 = atoi(argv[2]);
            int p2 = atoi(argv[3]);
            int p3 = atoi(argv[4]);
            printf("  p1 = %d, p2 = %d, p3 = %d\n", p1, p2, p3);
            resultado = func(p1, p2, p3);
            break;
        }
        default:
            fprintf(stderr, "Erro: Numero de parametros (%d) nao suportado.\n", num_params);
            return 1;
    }

    // --- 4. Exibição do Resultado ---
    printf("----------------------------------------\n");
    printf("Resultado retornado: %d\n", resultado);
    printf("----------------------------------------\n");

    return 0;
}