git clone https://github.com/N00byEdge/bootelf
cd bootelf
git checkout 36692a211a24bd941d6178ba5460cbbd9974be0e
nasm bootelf.asm -f bin -o bootsect.bin
cd ..
gcc -S kernel.c
nasm -f elf64 kernel.s
ld kernel.o -o kernel.elf
touch os.iso
cat bootsect.bin >> os.iso
cat kernel.elf >> os.iso
