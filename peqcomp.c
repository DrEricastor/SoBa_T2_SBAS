/*
 * Arquivo: peqcomp.c
 * Autor: Ernesto Dornelles Ituassu - 2411616 - Turma: 3WC
 * Descrição: Versão reestruturada do micro-compilador para SBas.
 * Foco: Uso correto da pilha de execução (Registro de Ativação) e
 * geração de código byte a byte para x86-64.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "peqcomp.h"

// Assinaturas das funções que tratam cada comando SBas
static void handle_att(char* linha, unsigned char codigo[], int* idx);
static void handle_op(char* linha, unsigned char codigo[], int* idx);
static void handle_iflez(char* linha, unsigned char codigo[], int* idx, int* mapa_linhas);
static void handle_ret(char* linha, unsigned char codigo[], int* idx);

/*
 * Função: prologo
 * Descrição: Gera o código de prólogo padrão de uma função x86-64.
 * - Salva o ponteiro de base antigo.
 * - Define o novo ponteiro de base.
 * - Aloca espaço na pilha para as 5 variáveis locais (v1 a v5).
 */
static void prologo(unsigned char codigo[], int* idx) {
    // push %rbp  (Salva o ponteiro de base)
    codigo[(*idx)++] = 0x55;

    // mov %rsp, %rbp (Define o novo ponteiro de base)
    codigo[(*idx)++] = 0x48;
    codigo[(*idx)++] = 0x89;
    codigo[(*idx)++] = 0xe5;

    // sub $20, %rsp (Aloca 20 bytes para 5 variáveis de 32 bits)
    codigo[(*idx)++] = 0x48;
    codigo[(*idx)++] = 0x83;
    codigo[(*idx)++] = 0xec;
    codigo[(*idx)++] = 0x14; // 20 em hexadecimal
}

/*
 * Função: epilogo
 * Descrição: Gera o código de epílogo padrão.
 * - Libera a pilha (desfaz o prólogo).
 * - Retorna da função.
 */
static void epilogo(unsigned char codigo[], int* idx) {
    // leave (Restaura a pilha: mov %rbp, %rsp; pop %rbp)
    codigo[(*idx)++] = 0xc9;
    // ret (Retorna para o chamador)
    codigo[(*idx)++] = 0xc3;
}

/*
 * Função: calcula_tamanho_instrucao
 * Descrição: Calcula o tamanho exato em bytes que o código de máquina
 * para uma dada linha SBas irá ocupar. Esta função é o coração
 * da primeira passada do compilador.
 */
static int calcula_tamanho_instrucao(char* linha) {
    // --- Instrução 'ret' ---
    if (strncmp(linha, "ret", 3) == 0) {
        // Epílogo (leave + ret) sempre tem 2 bytes.
        if (strchr(linha, '$')) {
            // ret $cst -> movl $cst, %eax (5 bytes) + epílogo (2 bytes)
            return 7;
        } else { // ret vN
            // ret vN -> movl mem, %eax (3 bytes) + epílogo (2 bytes)
            return 5;
        }
    }

    // --- Instrução 'iflez' ---
    if (strncmp(linha, "iflez", 5) == 0) {
        // cmpl $0, mem (4 bytes) + jle rel32 (6 bytes)
        return 10;
    }

    // --- Instrução de Atribuição (':') ---
    if (strchr(linha, ':') != NULL) {
        if (strchr(linha, '$')) {
            // v_dst : $cst -> movl $cst, mem (7 bytes)
            return 7;
        }
        if (strchr(linha, 'p')) {
            // v_dst : p_src -> movl reg, mem (3 bytes)
            return 3;
        }
        // v_dst : v_src -> movl mem,reg (3) + movl reg,mem (3)
        return 6;
    }

    // --- Instrução de Operação ('=') ---
    if (strchr(linha, '=') != NULL) {
        char op1_char = ' ';
        char op2_char = ' ';
        char op_char = ' ';

        // Encontra os caracteres que definem a operação
        char* eq_ptr = strchr(linha, '=');
        char* op_ptr = strpbrk(eq_ptr, "+-*");
        if (eq_ptr && op_ptr) {
            op_char = *op_ptr;
            // Verifica o caractere depois do '=' para saber o tipo do op1
            for (char* p = eq_ptr + 1; *p; p++) {
                if (*p == 'v' || *p == '$') {
                    op1_char = *p;
                    break;
                }
            }
            // Verifica o caractere depois do operador para saber o tipo do op2
            for (char* p = op_ptr + 1; *p; p++) {
                if (*p == 'v' || *p == '$') {
                    op2_char = *p;
                    break;
                }
            }
        }
        
        int load_size = (op1_char == 'v') ? 3 : 5; // mov mem,eax (3) ou mov $c,eax (5)
        int store_size = 3; // mov eax,mem (3)
        int op_size = 0;

        if (op2_char == 'v') { // Operando 2 é variável
            op_size = (op_char == '*') ? 4 : 3; // imull (4), add/sub (3)
        } else { // Operando 2 é constante
            op_size = (op_char == '*') ? 6 : 5; // imull (6), add/sub (5)
        }

        return load_size + op_size + store_size;
    }

    return 0; // Linha não reconhecida
}

/* =================================================================
 * Funções "Handle" para cada tipo de instrução SBas
 * ================================================================= */

/*
 * Função: handle_att
 * Descrição: Trata a instrução de atribuição 'var : varpc'.
 * A lógica se divide em 3 casos, dependendo da origem do dado.
 */
static void handle_att(char* linha, unsigned char codigo[], int* idx) {
    int dst_idx, src_idx, cst_val;
    char dst_offset;

    // Caso 1: A origem é uma variável (ex: v1 : v2)
    if (sscanf(linha, " v%d : v%d", &dst_idx, &src_idx) == 2) {
        // Lógica: Não podemos mover de memória para memória (pilha -> pilha).
        // Usamos um registrador temporário, o %eax, como intermediário.
        // 1. movl [rbp - src_idx*4], %eax  (da pilha para o registrador)
        // 2. movl %eax, [rbp - dst_idx*4]  (do registrador para a pilha)

        char src_offset = (char)(-src_idx * 4);
        dst_offset = (char)(-dst_idx * 4);

        // movl src_offset(%rbp), %eax -> opcode: 8b 45 XX
        codigo[(*idx)++] = 0x8b;
        codigo[(*idx)++] = 0x45;
        codigo[(*idx)++] = src_offset;

        // movl %eax, dst_offset(%rbp) -> opcode: 89 45 XX
        codigo[(*idx)++] = 0x89;
        codigo[(*idx)++] = 0x45;
        codigo[(*idx)++] = dst_offset;

    // Caso 2: A origem é um parâmetro (ex: v1 : p2)
    } else if (sscanf(linha, " v%d : p%d", &dst_idx, &src_idx) == 2) {
        // Lógica: Mover diretamente do registrador do parâmetro para a pilha.
        dst_offset = (char)(-dst_idx * 4);

        switch (src_idx) {
            case 1: // Origem é p1 (%edi)
                // movl %edi, dst_offset(%rbp) -> opcode: 89 7d XX
                codigo[(*idx)++] = 0x89;
                codigo[(*idx)++] = 0x7d;
                codigo[(*idx)++] = dst_offset;
                break;
            case 2: // Origem é p2 (%esi)
                // movl %esi, dst_offset(%rbp) -> opcode: 89 75 XX
                codigo[(*idx)++] = 0x89;
                codigo[(*idx)++] = 0x75;
                codigo[(*idx)++] = dst_offset;
                break;
            case 3: // Origem é p3 (%edx)
                // movl %edx, dst_offset(%rbp) -> opcode: 89 55 XX
                codigo[(*idx)++] = 0x89;
                codigo[(*idx)++] = 0x55;
                codigo[(*idx)++] = dst_offset;
                break;
        }

    // Caso 3: A origem é uma constante (ex: v1 : $50)
    } else if (sscanf(linha, " v%d : $%d", &dst_idx, &cst_val) == 2) {
        // Lógica: Mover o valor constante diretamente para a posição na pilha.
        dst_offset = (char)(-dst_idx * 4);

        // movl $constante, dst_offset(%rbp) -> opcode: c7 45 XX YY YY YY YY
        // XX é o offset, YYs são os 4 bytes da constante
        codigo[(*idx)++] = 0xc7;
        codigo[(*idx)++] = 0x45;
        codigo[(*idx)++] = dst_offset;
        memcpy(&codigo[*idx], &cst_val, 4); // Copia os 4 bytes da constante
        *idx += 4;

    } else {
        // Se chegou aqui, o formato da linha é inesperado.
        fprintf(stderr, "Erro de sintaxe na linha de atribuicao: %s", linha);
    }
}

/*
 * Função: handle_op
 * Descrição: Trata a instrução de operação 'var = varc op varc'.
 */
static void handle_op(char* linha, unsigned char codigo[], int* idx) {
    int dst_idx;
    char op1_str[10], op2_str[10], op_char;

    // 1. Parsing da linha de operação
    if (sscanf(linha, " v%d = %s %c %s", &dst_idx, op1_str, &op_char, op2_str) != 4) {
        fprintf(stderr, "Erro de sintaxe na linha de operacao: %s", linha);
        return;
    }

    // 2. Carregar o primeiro operando (op1) para o registrador %eax
    int val;
    if (op1_str[0] == 'v') { // Operando 1 é uma variável
        sscanf(op1_str, "v%d", &val);
        char offset = (char)(-val * 4);
        // movl offset(%rbp), %eax -> 8b 45 XX
        codigo[(*idx)++] = 0x8b;
        codigo[(*idx)++] = 0x45;
        codigo[(*idx)++] = offset;
    } else { // Operando 1 é uma constante
        sscanf(op1_str, "$%d", &val);
        // movl $val, %eax -> b8 YY YY YY YY
        codigo[(*idx)++] = 0xb8;
        memcpy(&codigo[*idx], &val, 4);
        *idx += 4;
    }

    // 3. Executar a operação usando o segundo operando (op2)
    if (op2_str[0] == 'v') { // Operando 2 é uma variável
        sscanf(op2_str, "v%d", &val);
        char offset = (char)(-val * 4);
        switch(op_char) {
            case '+': // addl offset(%rbp), %eax -> 03 45 XX
                codigo[(*idx)++] = 0x03;
                codigo[(*idx)++] = 0x45;
                codigo[(*idx)++] = offset;
                break;
            case '-': // subl offset(%rbp), %eax -> 2b 45 XX
                codigo[(*idx)++] = 0x2b;
                codigo[(*idx)++] = 0x45;
                codigo[(*idx)++] = offset;
                break;
            case '*': // imull offset(%rbp), %eax -> 0f af 45 XX
                codigo[(*idx)++] = 0x0f;
                codigo[(*idx)++] = 0xaf;
                codigo[(*idx)++] = 0x45;
                codigo[(*idx)++] = offset;
                break;
        }
    } else { // Operando 2 é uma constante
        sscanf(op2_str, "$%d", &val);
        switch(op_char) {
            case '+': // addl $val, %eax -> 05 YY YY YY YY
                codigo[(*idx)++] = 0x05;
                break;
            case '-': // subl $val, %eax -> 2d YY YY YY YY
                codigo[(*idx)++] = 0x2d;
                break;
            case '*': // imull $val, %eax, %eax -> 69 c0 YY YY YY YY
                codigo[(*idx)++] = 0x69;
                codigo[(*idx)++] = 0xc0;
                break;
        }
        memcpy(&codigo[*idx], &val, 4);
        *idx += 4;
    }

    // 4. Armazenar o resultado de %eax na variável de destino
    char dst_offset = (char)(-dst_idx * 4);
    // movl %eax, dst_offset(%rbp) -> 89 45 XX
    codigo[(*idx)++] = 0x89;
    codigo[(*idx)++] = 0x45;
    codigo[(*idx)++] = dst_offset;
}

static void handle_iflez(char* linha, unsigned char codigo[], int* idx, int mapa_linhas[]) {
    int var_idx, target_line_num;
    sscanf(linha, "iflez v%d %d", &var_idx, &target_line_num);

    // cmpl $0, offset(%rbp) -> 83 7d XX 00
    char offset = (char)(-var_idx * 4);
    codigo[(*idx)++] = 0x83;
    codigo[(*idx)++] = 0x7d;
    codigo[(*idx)++] = offset;
    codigo[(*idx)++] = 0x00;

    // jle rel32 -> 0f 8e offset(4 bytes)
    codigo[(*idx)++] = 0x0f;
    codigo[(*idx)++] = 0x8e;
    
    int target_address = mapa_linhas[target_line_num - 1];
    // O endereço atual (*idx) é o local onde o offset será escrito.
    // O endereço da próxima instrução é, portanto, (*idx) + 4.
    int address_of_next_instruction = *idx + 4;
    int relative_offset = target_address - address_of_next_instruction;
    
    memcpy(&codigo[*idx], &relative_offset, 4);
    *idx += 4;
}

/*
 * Função: handle_ret
 * Descrição: Trata a instrução de retorno 'ret varc'.
 * O valor de retorno deve ser colocado em %eax antes do epílogo.
 */
static void handle_ret(char* linha, unsigned char codigo[], int* idx) {
    int val;

    // Caso 1: Retornar uma constante (ex: ret $100)
    if (sscanf(linha, "ret $%d", &val) == 1) {
        // Objetivo: movl $constante, %eax
        
        // movl $VAL, %eax   (opcode: b8, seguido do valor em 4 bytes)
        codigo[(*idx)++] = 0xb8;
        memcpy(&codigo[*idx], &val, 4); // Copia os 4 bytes do inteiro
        *idx += 4;
        
    // Caso 2: Retornar uma variável (ex: ret v2)
    } else if (sscanf(linha, "ret v%d", &val) == 1) {
        // Objetivo: movl -N*4(%rbp), %eax
        char offset = (char)(-val * 4);
        
        // movl offset(%rbp), %eax -> opcode: 8b 45 XX
        codigo[(*idx)++] = 0x8b;
        codigo[(*idx)++] = 0x45;
        codigo[(*idx)++] = offset;
    }
    
    // Após colocar o valor de retorno em %eax, geramos o epílogo.
    epilogo(codigo, idx);
}


// Dentro de peqcomp...
funcp peqcomp (FILE *f, unsigned char codigo[]) {
    int mapa_linhas[30]; // O nosso "mapa" para os endereços
    int num_linhas = 0;
    int current_address = 8; //adicionando o tamanho do prologo
    char linha[100];

    // ==========================================================
    // PRIMEIRA PASSADA: Calcular tamanhos e preencher o mapa
    // ==========================================================
    while (fgets(linha, sizeof(linha), f) != NULL) {
        if (strlen(linha) <= 1) continue;
        mapa_linhas[num_linhas] = current_address;
        current_address += calcula_tamanho_instrucao(linha);
        num_linhas++;
    }

    // REBOBINAR O ARQUIVO para ler do início na segunda passada
    fseek(f, 0, SEEK_SET);

    // ==========================================================
    // SEGUNDA PASSADA: Gerar o código de fato
    // ==========================================================
    int codigoI = 0;
    prologo(codigo, &codigoI);

    while (fgets(linha, sizeof(linha), f) != NULL) {
        if (strlen(linha) <= 1) continue;

        if (strncmp(linha, "ret", 3) == 0) {
            handle_ret(linha, codigo, &codigoI);
        } else if (strncmp(linha, "iflez", 5) == 0) {
            // Agora, handle_iflez precisa do mapa!
            handle_iflez(linha, codigo, &codigoI, mapa_linhas);
        } else if (strchr(linha, ':') != NULL) {
            handle_att(linha, codigo, &codigoI);
        } else if (strchr(linha, '=') != NULL) {
            handle_op(linha, codigo, &codigoI);
        }
    }

    return (funcp)codigo;
}