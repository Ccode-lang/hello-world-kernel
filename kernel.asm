section .data

section .text

global _start

_start:
    mov ax, 0xb800
    mov es, ax
    mov si, 0
    
    mov es: si, ’H’
    mov si, 1
    mov es: si, ’e’
    mov si, 2
    mov es: si, ’l’
    mov si, 3
    mov es: si, ’l’
    mov si, 4
    mov es: si, ’o’
    ;forever loop
    jmp $
