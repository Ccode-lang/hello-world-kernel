section .data

section .text

global _start

_start:
    mov ax, 0xb800
    mov es, ax
    
    mov es:0, ’H’
    mov es:1, ’e’
    mov es:2, ’l’
    mov es:3, ’l’
    mov es:4, ’o’
    ;forever loop
    jmp $
