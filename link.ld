OUTPUT_FORMAT(elf32-i386)
ENTRY(start)
SECTIONS
 {
   . = 0x100000;
   .text : { *(.text) }
   . += CONSTANT(MAXPAGESIZE);
   .data : { *(.data) }
   . += CONSTANT(MAXPAGESIZE);
   .bss  : { *(.bss)  }
 }
