/* Ernesto Dornelles Ituassu 2411616 Turma: 3WC */
/* João Henrique Pestana de Aguiar Godinho 2412107 Turma: 3WC */


// 1. atribuição
// 2. operação
// 3. desvio
// 4. retorno
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "peqcomp.h"

char varNum2opcMask(char num) {
    switch (num) {
        case '1': 
            //rbx
            break;
        case '2': 
            //r12
            break;
        case '3': 
            //r13
            break;
        case '4': 
            //r14
            break;
        case '5': 
            //r15
            break;
    }
}
char parNum2opcMask(int num) {
    switch (num) {
        case '1': 
            //rdi
            break;
        case '2': 
            //rsi
            break;
        case '3': 
            //rdx
            break;
    }
}

void prologo(unsigned char codigo[], int* codigoI) {
    
    //faz push em todos os registradores que nós vamos utilizar durante a função (eles são callee saved)
    codigo[(*codigoI)++] = (char) 0x53;
    codigo[(*codigoI)++] = (char) 0x4154;
    codigo[(*codigoI)++] = (char) 0x4155;
    codigo[(*codigoI)++] = (char) 0x4156;
    codigo[(*codigoI)++] = (char) 0x4157;

    return;
    
}
void epilogo(unsigned char codigo[], int* codigoI) {

    //faz pop em todos os registradores que nós utilizamos durante a função (eles são callee saved)
    codigo[(*codigoI)++] = (char) 0x415f;
    codigo[(*codigoI)++] = (char) 0x415e;
    codigo[(*codigoI)++] = (char) 0x415d;
    codigo[(*codigoI)++] = (char) 0x415c;
    codigo[(*codigoI)++] = (char) 0x5b;

    return;
}

void iflez(char* linha, unsigned char codigo[], int* codigoI) {
    return;
}


void ret(char* linha, unsigned char codigo[], int* codigoI) {
    //ret varc
    int num;
    if (linha[4] == '$') {
        if (linha[5] == '-') {
            num = (linha[6] - '0') *-1;
        } else {
            num = linha[5] - '0';
        }
        printf("constante = %d\n", num);
        // agora que descobrimos a constante, incluimos os opcodes das seguintes
        // instruções no vetor codigo:
        // mov num, %rax
        // ret

    } else {
        //entao ret recebe uma variavel, que vai estar dentro de um registrador
    }
} 


void attVar(char* linha, unsigned char codigo[], int* codigoI) {
    char dstNum = linha[1]; //onde botar?
    char srcNum = linha[6]; //onde pegar?
    

    //sabendo ambas as infos, ver o opcode dos comandos e botar em codigo
}
void attPar(char* linha, unsigned char codigo[], int* codigoI) {
    char dstNum = linha[1]; //onde botar?
    int srcNum = linha[6]; //onde pegar?

    //sabendo ambas as infos, ver o opcode dos comandos e botar em codigo
}
void attConst(char* linha, unsigned char codigo[], int* codigoI) {
    char dstNum = linha[1]; //onde botar?
    int num = linha[6] - '0'; //onde pegar?

    //sabendo ambas as infos, ver o opcode dos comandos e botar em codigo
}

void att(char* linha, unsigned char codigo[], int* codigoI) {
    //var : varpc
    //v1 : v2

    char srcTipo = linha[5];
    if (srcTipo == 'v') {
        attVar(linha, codigo, codigoI);
    } else if (srcTipo == 'p') {
        attPar(linha, codigo, codigoI);
    } else {
        attConst(linha, codigo, codigoI);
    }

    return;
}


void opVarVar(char* linha, unsigned char codigo[], int* codigoI) {
    char dstMask = varNum2opcMask(linha[1]); //onde botar?
    char src1mask = varNum2opcMask(linha[6]); //onde pegar primeiro?
    char src2mask = varNum2opcMask(linha[11]); //onde pegar segundo?

    //sabendo ambas as infos, ver o opcode dos comandos e botar em codigo
}
void opVarConst(char* linha, unsigned char codigo[], int* codigoI) {
    char dstMask = varNum2opcMask(linha[1]); //onde botar?
    char src1mask = varNum2opcMask(linha[6]); //onde pegar primeiro?
    int src2num = linha[11]-'0'; //onde pegar segundo?

    //sabendo ambas as infos, ver o opcode dos comandos e botar em codigo
}
void opConstVar(char* linha, unsigned char codigo[], int* codigoI) {
    char dstMask = varNum2opcMask(linha[1]); //onde botar?
    int src1num = linha[6]-'0'; //onde pegar primeiro?
    char src2mask = varNum2opcMask(linha[11]); //onde pegar segundo?

    //sabendo ambas as infos, ver o opcode dos comandos e botar em codigo
}
void opConstConst(char* linha, unsigned char codigo[], int* codigoI) {}

void op(char* linha, unsigned char codigo[], int* codigoI) {
    //var = varc op varc
    char src1tipo = linha[5];
    char src2tipo = linha[10];

    if (src1tipo == 'v' && src2tipo == 'v') {
        opVarVar(linha, codigo, codigoI);
    } else if (src1tipo = 'v' && src2tipo == '$') {
        opVarConst(linha, codigo, codigoI);
    } else if (src1tipo = '$' && src2tipo == 'v') {
        opConstVar(linha, codigo, codigoI);
    } else {
        opConstConst(linha, codigo, codigoI);
    }

    return;
}

void leArquivo(FILE* f, unsigned char codigo[], int* codigoI) {
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

        if(bufferLinha[0] == 'i') {
            //iflez();
        } else if (bufferLinha[0] = 'r') {
            ret(bufferLinha, codigo, codigoI);
        } else {
            if (bufferLinha[3] == ':') {
                //att();
            } else {
                //op();
            }
        }
    }
    printf("\n");
}

funcp peqcomp (FILE *f, unsigned char codigo[]) {
    int codigoI = 0; //indice do array de codigo atual
    leArquivo(f, codigo, &codigoI);
}
