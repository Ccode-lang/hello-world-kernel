section .data

section .text

global _start

_start:
    mov 0xb8000, ’H’
    mov 0xb8001, ’e’
    mov 0xb8002, ’l’
    mov 0xb8003, ’l’
    mov 0xb8004, ’o’
    ;forever loop
    jmp $
