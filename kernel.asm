;nasm directive - 32 bit
bits 32
section .text
        ;multiboot spec
        align 4
        dd 0x1BADB002            ;magic
        dd 0x00                  ;flags
        dd - (0x1BADB002 + 0x00) ;checksum. m+f+c should be zero

global start
global read_port
global write_port
global load_idt

extern kmain	        ;kmain is defined in the c file
extern term


; Read and write borrowed from lisse (https://github.com/InsaneMiner/Lisse)
read_port:
	mov edx, [esp + 4]
			;al is the lower 8 bits of eax
	in al, dx	;dx is the lower 16 bits of edx
	ret
kbhandler:                 
	call    term
	iretd
write_port:
	mov   edx, [esp + 4]    
	mov   al, [esp + 4 + 4]  
	out   dx, al  
	ret

load_idt:
	mov edx, [esp + 4]
	lidt [edx]
	sti
	ret

start:
  cli 			;block interrupts
  mov esp, stack_space	;set stack pointer
  call kmain
  jmp $		 	;halt the CPU

section .bss
resb 8192
stack_space:
