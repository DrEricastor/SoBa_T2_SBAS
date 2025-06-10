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
   0:   48 89 db                mov    %rbx,%rbx
   3:   49 89 dc                mov    %rbx,%r12
   6:   49 89 dd                mov    %rbx,%r13
   9:   49 89 de                mov    %rbx,%r14
   c:   49 89 df                mov    %rbx,%r15

   f:   4c 89 e3                mov    %r12,%rbx
  12:   4d 89 e4                mov    %r12,%r12
  15:   4d 89 e5                mov    %r12,%r13
  18:   4d 89 e6                mov    %r12,%r14
  1b:   4d 89 e7                mov    %r12,%r15

  1e:   4c 89 eb                mov    %r13,%rbx
  21:   4d 89 ec                mov    %r13,%r12
  24:   4d 89 ed                mov    %r13,%r13
  27:   4d 89 ee                mov    %r13,%r14
  2a:   4d 89 ef                mov    %r13,%r15

  2d:   4c 89 f3                mov    %r14,%rbx
  30:   4d 89 f4                mov    %r14,%r12
  33:   4d 89 f5                mov    %r14,%r13
  36:   4d 89 f6                mov    %r14,%r14
  39:   4d 89 f7                mov    %r14,%r15

  3c:   4c 89 fb                mov    %r15,%rbx
  3f:   4d 89 fc                mov    %r15,%r12
  42:   4d 89 fd                mov    %r15,%r13
  45:   4d 89 fe                mov    %r15,%r14
  48:   4d 89 ff                mov    %r15,%r15

## opcodes op (+)
   0:   48 01 db                add    %rbx,%rbx
   3:   49 01 dc                add    %rbx,%r12
   6:   49 01 dd                add    %rbx,%r13
   9:   49 01 de                add    %rbx,%r14
   c:   49 01 df                add    %rbx,%r15

   f:   4c 01 e3                add    %r12,%rbx
  12:   4d 01 e4                add    %r12,%r12
  15:   4d 01 e5                add    %r12,%r13
  18:   4d 01 e6                add    %r12,%r14
  1b:   4d 01 e7                add    %r12,%r15

  1e:   4c 01 eb                add    %r13,%rbx
  21:   4d 01 ec                add    %r13,%r12
  24:   4d 01 ed                add    %r13,%r13
  27:   4d 01 ee                add    %r13,%r14
  2a:   4d 01 ef                add    %r13,%r15

  2d:   4c 01 f3                add    %r14,%rbx
  30:   4d 01 f4                add    %r14,%r12
  33:   4d 01 f5                add    %r14,%r13
  36:   4d 01 f6                add    %r14,%r14
  39:   4d 01 f7                add    %r14,%r15

  3c:   4c 01 fb                add    %r15,%rbx
  3f:   4d 01 fc                add    %r15,%r12
  42:   4d 01 fd                add    %r15,%r13
  45:   4d 01 fe                add    %r15,%r14
  48:   4d 01 ff                add    %r15,%r15

## opcodes op (-)
   0:   48 29 db                sub    %rbx,%rbx
   3:   49 29 dc                sub    %rbx,%r12
   6:   49 29 dd                sub    %rbx,%r13
   9:   49 29 de                sub    %rbx,%r14
   c:   49 29 df                sub    %rbx,%r15

   f:   4c 29 e3                sub    %r12,%rbx
  12:   4d 29 e4                sub    %r12,%r12
  15:   4d 29 e5                sub    %r12,%r13
  18:   4d 29 e6                sub    %r12,%r14
  1b:   4d 29 e7                sub    %r12,%r15

  1e:   4c 29 eb                sub    %r13,%rbx
  21:   4d 29 ec                sub    %r13,%r12
  24:   4d 29 ed                sub    %r13,%r13
  27:   4d 29 ee                sub    %r13,%r14
  2a:   4d 29 ef                sub    %r13,%r15

  2d:   4c 29 f3                sub    %r14,%rbx
  30:   4d 29 f4                sub    %r14,%r12
  33:   4d 29 f5                sub    %r14,%r13
  36:   4d 29 f6                sub    %r14,%r14
  39:   4d 29 f7                sub    %r14,%r15

  3c:   4c 29 fb                sub    %r15,%rbx
  3f:   4d 29 fc                sub    %r15,%r12
  42:   4d 29 fd                sub    %r15,%r13
  45:   4d 29 fe                sub    %r15,%r14
  48:   4d 29 ff                sub    %r15,%r15

  ## opcodes op (*)
   0:   48 0f af db             imul   %rbx,%rbx
   4:   4c 0f af e3             imul   %rbx,%r12
   8:   4c 0f af eb             imul   %rbx,%r13
   c:   4c 0f af f3             imul   %rbx,%r14
  10:   4c 0f af fb             imul   %rbx,%r15

  14:   49 0f af dc             imul   %r12,%rbx
  18:   4d 0f af e4             imul   %r12,%r12
  1c:   4d 0f af ec             imul   %r12,%r13
  20:   4d 0f af f4             imul   %r12,%r14
  24:   4d 0f af fc             imul   %r12,%r15

  28:   49 0f af dd             imul   %r13,%rbx
  2c:   4d 0f af e5             imul   %r13,%r12
  30:   4d 0f af ed             imul   %r13,%r13
  34:   4d 0f af f5             imul   %r13,%r14
  38:   4d 0f af fd             imul   %r13,%r15

  3c:   49 0f af de             imul   %r14,%rbx
  40:   4d 0f af e6             imul   %r14,%r12
  44:   4d 0f af ee             imul   %r14,%r13
  48:   4d 0f af f6             imul   %r14,%r14
  4c:   4d 0f af fe             imul   %r14,%r15

  50:   49 0f af df             imul   %r15,%rbx
  54:   4d 0f af e7             imul   %r15,%r12
  58:   4d 0f af ef             imul   %r15,%r13
  5c:   4d 0f af f7             imul   %r15,%r14
  60:   4d 0f af ff             imul   %r15,%r15


## Perguntas
1. constante é sempre de um digito?
2. Padrao das instruções
3. Usar registradores e instruções 64 mesmo sendo 32