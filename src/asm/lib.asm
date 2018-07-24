global write_port_8
global read_port_8
global entry_64

extern kernel_main

section .text
bits 64
write_port_8:
    mov rax, 0
    mov rdx, rdi ;参数一,端口地址,16bit
    mov rax, rsi ;参数二,8bit
    out dx, al
    ret

read_port_8:
    mov rax, 0
    mov rdx, 0
    mov rdx, rdi ;参数一,端口地址,16bit
    in  al, dx   ;rax 返回 8bit数
    ret

error:
    mov dword [0xb8000], 0x4f524f45
    mov dword [0xb8004], 0x4f3a4f52
    mov dword [0xb8008], 0x4f204f20
    mov ax,0x2f61
    mov word  [0xb800a], ax
    hlt

entry_64:
    mov rax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    call kernel_main
    hlt