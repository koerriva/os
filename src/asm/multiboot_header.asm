extern kernal_main

section .multiboot_header
header_start:
    dd 0xe85250d6                ; magic number (multiboot 2)
    dd 0                         ; architecture 0 (protected mode i386)
    dd header_end - header_start ; header length
    ; checksum
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

    ; insert optional multiboot tags here
    ; dw 5    ; type
    ; dw 1    ; flags
    ; dd 20    ; size
    ; dd 1024
    ; dd 768
    ; dd 32
    ; align 8
    ; required end tag
    dw 0    ; type
    dw 0    ; flags
    dd 8    ; size
header_end: