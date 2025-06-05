# SOBA T2
## ESPACOS DE DADOS E SUAS CORRESPONDENCIAS EM ASM

**Variaveis**
v1 - rbx
v2 - r12
v3 - r13
v4 - r14
v5 - r15
> TODAS TEM QUE SER SALVAS NO STACK PQ SAO CALLEE SAVED

**Parametros**
p1 - rdi
p2 - rsi
p3 - rdx
> SÃO OS ARGUMENTOS DA ARQUITETURA ASSEMBLY EM ORDEM

## COMANDOS E SUAS CORRESPONDENCIAS EM ASM
**var : varpc**
> mov varpc, var

**var = varc op varc**
> mov varc2, var \n
> op varc1, var

**ret varc**
> mov varc, %rax \n
> ret

**iflez var num**
> cmp var, $0 \n
> jle num
