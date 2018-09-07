global outportb
global inportb
global entry_64

extern kernel_main

section .text
bits 64
entry_64:
    mov rax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    call kernel_main
loop: hlt
    jmp loop

; lib
outportb:
    mov rax, 0
    mov rdx, rdi ;参数一,端口地址,16bit
    mov rax, rsi ;参数二,8bit
    out dx, al
    ret

inportb:
    mov rax, 0
    mov rdx, 0
    mov rdx, rdi ;参数一,端口地址,16bit
    in  al, dx   ;rax 返回 8bit数
    ret