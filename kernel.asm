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
extern kmain	        ;kmain is defined in the c file

read_port:
  mov edx, [esp + 4]
  in al, dx
  ret

write_port:
  mov   edx, [esp + 4]    
  mov   al, [esp + 4 + 4]  
  out   dx, al  
  ret

start:
  cli 			;block interrupts
  mov esp, stack_space	;set stack pointer
  call kmain
  jmp $		 	;halt the CPU

section .bss
resb 8192
stack_space:
