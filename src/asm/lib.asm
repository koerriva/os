global write_mem_64

section .text
bits 64
write_mem_64:
    mov rax, rdi
    mov rbx, rsi
    mov [rax], rbx
    hlt