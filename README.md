# SOBA T2
## ESPACOS DE DADOS E SUAS CORRESPONDENCIAS EM ASM

**Variaveis** <br />
v1 - rbx <br />
v2 - r12 <br />
v3 - r13 <br />
v4 - r14 <br />
v5 - r15 <br />
> TODAS TEM QUE SER SALVAS NO STACK PQ SAO CALLEE SAVED

**Parametros** <br />
p1 - rdi <br />
p2 - rsi <br />
p3 - rdx <br />
> SÃO OS ARGUMENTOS DA ARQUITETURA ASSEMBLY EM ORDEM

## COMANDOS E SUAS CORRESPONDENCIAS EM ASM
**var : varpc**
> mov varpc, var

**var = varc op varc**
> mov varc2, var <br />
> op varc1, var

**ret varc**
> mov varc, %rax <br />
> ret

**iflez var num**
> cmp var, $0 <br />
> jle num


# FODASE TUDO ISSO AQUI EM BAIXO USA r8 E r9 PRAS OPERAÇÕES KK
## opcodes ret
   0:   48 89 d8                mov    %rbx,%rax
   3:   4c 89 e0                mov    %r12,%rax
   6:   4c 89 e8                mov    %r13,%rax
   9:   4c 89 f0                mov    %r14,%rax
   c:   4c 89 f8                mov    %r15,%rax

   f:   48 c7 c0 0a 00 00 00    mov    $0xa,%rax
  16:   48 c7 c0 00 01 00 00    mov    $0x100,%rax
  1d:   48 c7 c0 ff ff ff ff    mov    $0xffffffffffffffff,%rax

  10:   c3                      ret

## opcodes mov
   0:   49 89 d8                mov    %rbx,%r8
   3:   4d 89 e0                mov    %r12,%r8
   6:   4d 89 e8                mov    %r13,%r8
   9:   4d 89 f0                mov    %r14,%r8
   c:   4d 89 f8                mov    %r15,%r8

   f:   49 89 d9                mov    %rbx,%r9
  12:   4d 89 e1                mov    %r12,%r9
  15:   4d 89 e9                mov    %r13,%r9
  18:   4d 89 f1                mov    %r14,%r9
  1b:   4d 89 f9                mov    %r15,%r9

  1e:   4c 89 c3                mov    %r8,%rbx
  21:   4d 89 c4                mov    %r8,%r12
  24:   4d 89 c5                mov    %r8,%r13
  27:   4d 89 c6                mov    %r8,%r14
  2a:   4d 89 c7                mov    %r8,%r15
  
  2d:   4c 89 cb                mov    %r9,%rbx
  30:   4d 89 cc                mov    %r9,%r12
  33:   4d 89 cd                mov    %r9,%r13
  36:   4d 89 ce                mov    %r9,%r14
  39:   4d 89 cf                mov    %r9,%r15

; Movimentos dos parâmetros (rdi, rsi, rdx) para r8
; movq %rdi, %r8
4C 89 C7
; movq %rsi, %r8
4C 89 CE
; movq %rdx, %r8
4C 89 C2

; Movimentos dos parâmetros (rdi, rsi, rdx) para r9
; movq %rdi, %r9
4C 89 CF
; movq %rsi, %r9
4C 89 C6
; movq %rdx, %r9
4C 89 CA

; Movimentos de r8 para os parâmetros (rdi, rsi, rdx)
; movq %r8, %rdi
4C 89 C0
; movq %r8, %rsi
4C 89 C0
; movq %r8, %rdx
4C 89 C2

; Movimentos de r9 para os parâmetros (rdi, rsi, rdx)
; movq %r9, %rdi
4C 89 C1
; movq %r9, %rsi
4C 89 C1
; movq %r9, %rdx
4C 89 CA

  3c:   c9                      leave
  3d:   c3                      ret

# Perguntas
1. tem problema eu adicionar o int diretamente no opcode de constantes?
2. Usar registradores e instruções 64 mesmo sendo 32
3. como eu boto label no opcode kk

# A Fazer
1. Remover "opcode para leave" 0xc9
2. remover push e pop e adaptar pra manipulação no stacks
3. erro em um dos opcodes da função opcodeMovPxr8
4. constantes
5. jlez
6. 2 vetores: 
  1- linhas: linhas[1] = 0x12345678 (endereco da linha 1)
  2- jlezEndDst: codigoIJlez (int) | dst (int)


Resposa ao jump: passagem dupla, deixa lacunas, preenche no final da primeira passagem.

v1 : v2
v3 = v2 + v3
1 mov v2, r8
1 mov r8, v1
2 mov r8, v2
2 mov r9, v3
2 add r8, r9

jlez 2


&codigo[4]