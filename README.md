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
