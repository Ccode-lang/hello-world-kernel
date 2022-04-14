nasm -f elf32 kernel.asm -o kasm.o
gcc -m32 -c kernel.c -o kc.o
ld -m elf_i386 -fstack-protector -shared -T link.ld -o kernel.iso kasm.o kc.o
