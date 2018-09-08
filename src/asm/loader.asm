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

setup_idt:
    extern ignore_init
    extern idt64_base
    lea rdx, ignore_init
    mov rax, 0x08 << 16
    mov ax,dx
    mov rcx, 0x8ee00 << 32
    add rax, rcx
    mov ecx, edx
    shr ecx, 16
    shl rcx, 48
    add rax, rcx
    shr rdx, 32
    lea rdi, idt64_base
    mov rcx, 256
rp_sidt:
    mov [rdi], rax
    mov [rdi+8], rdx
    add rdi, 0x10
    dec rcx
    jne rp_sidt

