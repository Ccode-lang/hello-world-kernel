section .data

section .text

global _start

_start:
    sti
    mov ah, 0x0e
    
    mov al, ’H’
    int 0x10
    mov al, ’e’
    int 0x10
    mov al, ’l’
    int 0x10
    mov al, ’l’
    int 0x10
    mov al, ’o’
    int 0x10
    ;forever loop
    jmp $
