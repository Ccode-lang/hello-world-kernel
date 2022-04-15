# hello-world-kernel
A simple kernel for i386 CPUs that has a terminal.

# Running wh.iso from blbuild.sh file
 * Run with `qemu-system-i386 -cdrom wh.iso`
This puts grub bootloader in the iso to make it easier to boot.
# Running kernel.iso from build.sh file
 * Run with `qemu-system-i386 -kernel kernel.iso`
This uses the bootloader that comes with qemu.
# Grub config
First rename the kernel.iso file you get from build.sh to kernel-1 and add it to /boot directory.
```
title helloWorld
	root (hd0,0)
	kernel /boot/kernel-1 ro
```
# Features
 * idt
 * can write to basic vga video memory.
 * Keyboard support.
 * Has 2 commands.

![](2022-04-14-105913_1920x1080_scrot.png)
