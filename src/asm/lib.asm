global write_mem_64
global write_port_8
global read_port_8
global sleep

section .text
bits 64

sleep: hlt

write_mem_64:
    mov rax, rdi
    mov rbx, rsi
    mov [rax], rbx
    ret

write_port_8:
    and rax,0
    mov rdx, rdi ;参数一,端口地址,16bit
    mov rax, rsi ;参数二,8bit
    out dx,al
    ret

read_port_8:
    and rax, 0
    mov rdx, rdi ;参数一,端口地址,16bit
    in al,dx    ;rax 返回 8bit数
    ret
