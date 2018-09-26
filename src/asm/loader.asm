global outportb
global inportb
global get_cs
global load_idt
global load_gdt
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

get_cs:
    mov rax,0
    mov rax,cs
    ret

load_idt:
    lidt [rdi]
    ret

load_gdt:
    lgdt [rdi]
    ret

global _int_xx_handler
_int_xx_handler:
    mov rsi,[rsp]
    mov rdx,[rsp+8]
    mov rcx,[rsp+16]
    mov r8,[rsp+24]
    mov r9,[rsp+32]
    mov rdi,IgnoreError
    extern printf
    call printf
.lp: hlt
    loop .lp
    iretq

global _int_0x0_handler
_int_0x0_handler:
    mov rsi,[rsp]
    mov rdi,DivideByZero
    extern printf
    call printf
    iretq

global _int_0x1_handler
_int_0x1_handler:
    mov rsi,[rsp]
    mov rdi,Debug
    extern printf
    call printf
    iretq

global _int_0x3_handler
_int_0x3_handler:
    mov rsi,[rsp]
    mov rdi,Breakpoint
    extern printf
    call printf
    iretq

global _int_0x8_handler
_int_0x8_handler:
    mov rsi,[rsp]
    mov rdx,[rsp+8]
    mov rdi,DoubleFault
    extern printf
    call printf
.lp: hlt
    loop .lp
    iretq

global _int_0xd_handler
_int_0xd_handler:
    pop rsi
    mov rdx,[rsp]
    mov rdi,GeneralProtectionFault
    extern printf
    call printf
    iretq

global _int_0xe_handler
_int_0xe_handler:
    pop rsi
    mov rdx,[rsp]
    mov rdi,PageFault
    extern printf
    call printf
    iretq


section .data
IgnoreError:db "ERROR at IP:0x%x,CS:0x%x,RFLG:0x%x,SP:0x%x,SS:0x%x",0xa,0
Breakpoint:db "EXCEPTION:BREAKPOINT at IP:0x%x",0xa,0
DivideByZero:db "EXCEPTION:DivideByZero at IP:0x%x",0xa,0
Debug:db "EXCEPTION:Debug at IP:0x%x",0xa,0
DoubleFault:db "ABORT：DF [Code:0x%x] at IP:0x%x",0xa,0
PageFault:db "EXCEPTION：PF [Code:0x%x] at IP:0x%x",0xa,0
GeneralProtectionFault:db "EXCEPTION：GP [Code:0x%x] at IP:0x%x",0xa,0