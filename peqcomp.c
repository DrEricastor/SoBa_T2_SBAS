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

void opcodeMovVxr8(int numV, unsigned char codigo[], int* codigoI) {
    int opcode = 0;
    switch (numV) {
        case 1: opcode = 0x4989d8;
        case 2: opcode = 0x4d89e0;
        case 3: opcode = 0x4d89e8;
        case 4: opcode = 0x4d89f0;
        case 5: opcode = 0x4d89f8;
    }

    codigo[(*codigoI)++] = opcode;
}
void opcodeMovVxr9(int numV, unsigned char codigo[], int* codigoI) {
    int opcode = 0;
    switch (numV) {
        case 1: opcode = 0x4989d9;
        case 2: opcode = 0x4d89e1;
        case 3: opcode = 0x4d89e9;
        case 4: opcode = 0x4d89f1;
        case 5: opcode = 0x4d89f9;
    }

    codigo[(*codigoI)++] = opcode;
}
void opcodeMovr8Vx(int numV, unsigned char codigo[], int* codigoI) {
    int opcode = 0;
    switch (numV) {
        case 1: opcode = 0x4c89c3;
        case 2: opcode = 0x4d89c4;
        case 3: opcode = 0x4d89c5;
        case 4: opcode = 0x4d89c6;
        case 5: opcode = 0x4d89c7;
    }

    codigo[(*codigoI)++] = opcode;
}
void opcodeMovr9Vx(int numV, unsigned char codigo[], int* codigoI) {
    int opcode = 0;
    switch (numV) {
        case 1: opcode = 0x4c89cb;
        case 2: opcode = 0x4d89cc;
        case 3: opcode = 0x4d89cd;
        case 4: opcode = 0x4d89ce;
        case 5: opcode = 0x4d89cf;
    }

    codigo[(*codigoI)++] = opcode;
}

void opcodeMovPxr8(int numP, unsigned char codigo[], int* codigoI) {
    int opcode = 0;
    switch (numP) {
        case 1: opcode = 0x4C89C7;
        case 2: opcode = 0x4C89CE;
        case 3: opcode = 0x4C89C2;
    }
    codigo[(*codigoI)++] = opcode;
}
void opcodeMovr8Px(int numP, unsigned char codigo[], int* codigoI) {
    int opcode = 0;
    switch (numP) {
        case 1: opcode = 0x4C89C0;
        case 2: opcode = 0x4C89C0; // TODO TEM UM ERRO AQUI
        case 3: opcode = 0x4C89C2;
    }
    codigo[(*codigoI)++] = opcode;
}

void prologo(unsigned char codigo[], int* codigoI) {
    
    //faz push em todos os registradores que nós vamos utilizar durante a função (eles são callee saved)
    codigo[(*codigoI)++] = 0x53;
    codigo[(*codigoI)++] = 0x4154;
    codigo[(*codigoI)++] = 0x4155;
    codigo[(*codigoI)++] = 0x4156;
    codigo[(*codigoI)++] = 0x4157;

    return;
}
void epilogo(unsigned char codigo[], int* codigoI) {

    //faz pop em todos os registradores que nós utilizamos durante a função (eles são callee saved)
    codigo[(*codigoI)++] = 0x415f;
    codigo[(*codigoI)++] = 0x415e;
    codigo[(*codigoI)++] = 0x415d;
    codigo[(*codigoI)++] = 0x415c;
    codigo[(*codigoI)++] = 0x5b;

    return;
}


void iflez(char* linha, unsigned char codigo[], int* codigoI) {
    return;
}


void opcodeRetConst(char* linha, unsigned char codigo[], int* codigoI) {
    int cst = 0;
    sscanf(linha, "ret $%d", &cst);
    int opcode = 0x48c7c0000000 + cst;
    codigo[(*codigoI)++] = opcode;
    codigo[(*codigoI)++] = 0xc9; // opcode para leave
    codigo[(*codigoI)++] = 0xc3; // opcode para ret

}
void opcodeRetVar(char* linha, unsigned char codigo[], int* codigoI) {
    int reg = 0;
    int opcode = 0;
    sscanf(linha, "ret v%d", &reg);
    switch (reg) {
        case 1: opcode = 0x4889d8;
        case 2: opcode = 0x4889e0;
        case 3: opcode = 0x4889e8;
        case 4: opcode = 0x4889f0;
        case 5: opcode = 0x4889f8;
    }
    codigo[(*codigoI)++] = opcode;
    codigo[(*codigoI)++] = 0xc9; // opcode para leave
    codigo[(*codigoI)++] = 0xc3; // opcode para ret


    
}

void ret(char* linha, unsigned char codigo[], int* codigoI) {
    //ret varc
    int num;
    if (linha[4] == '$') {
        opcodeRetConst(linha, codigo, codigoI);
    } else {
        opcodeRetVar(linha, codigo, codigoI);
    }
} 


void attVar(char* linha, unsigned char codigo[], int* codigoI) {
    char dstNum = 0; //onde botar?
    int srcNum = 0; //onde pegar?
    sscanf(linha, "v%d : v%d",&dstNum, &srcNum);
    
    opcodeMovVxr8(srcNum, codigo, codigoI); // manda pra r8
    opcodeMovr8Vx(dstNum, codigo, codigoI); // manda de r8 pra dst (var)

}
void attPar(char* linha, unsigned char codigo[], int* codigoI) {
    char dstNum = 0; //onde botar?
    int srcNum = 0; //onde pegar?
    sscanf(linha, "v%d : p%d",&dstNum, &srcNum);

    opcodeMovVxr8(srcNum, codigo, codigoI); // manda pra r8
    opcodeMovr8Px(dstNum, codigo, codigoI); // manda de r8 pra dst (par)

}
void attConst(char* linha, unsigned char codigo[], int* codigoI) {
    char dstNum = 0; //onde botar?
    int num = 0; //onde pegar?
    sscanf(linha, "v%d : $%d",&dstNum, &num);

    //TODO: sabendo ambas as infos, ver o opcode dos comandos e botar em codigo
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