movq %rdi, %r8
movq %rsi, %r8
movq %rdx, %r8

movq %rdi, %r9
movq %rsi, %r9
movq %rdx, %r9

movq %r8, %rdi
movq %r8, %rsi
movq %r8, %rdx

movq %r9, %rdi
movq %r9, %rsi
movq %r9, %rdx
leave
ret