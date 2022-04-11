#include "includes/colors.h"
#include "includes/keymap.h"

#define true 1
#define false 0

extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);

char *vidptr = (char*)0xb8000; 	//video mem begins here.
unsigned int location = 0;
unsigned int counter = 0;
unsigned char kbstatus;
char keycode;

void clear() {
	counter = 0;
	location = 0;
	while(counter < 80 * 25 * 2) {
		/* blank character */
		vidptr[counter] = ' ';
		/* attribute-byte - light grey on black screen */
		vidptr[counter+1] = 0x07; 		
		counter = counter + 2;
	}
}

void kprint(const char *str, int color) {
	counter = 0;
	while(str[counter] != '\0') {
		/* the character's ascii */
		vidptr[location] = str[counter];
		/* attribute-byte: give character black bg and light grey fg */
		vidptr[location+1] = color;
		++counter;
		location = location + 2;
	}
}

void knewline() {
	// number of bytes in a line
	unsigned int line_size = 160;
	location = location + (line_size - location % (line_size));
}

void term() {
	while(1) {
		kbstatus = read_port(0x64);
		if (kbstatus & 0x01) {
			keycode = read_port(0x60);
			if (keycode == 0x1C) {
				knewline();
				kprint("Terminal -$ ", VGA_COLOR_GREEN);
			} else if (!(keycode < 0)) {
				vidptr[location++] = keyboard_map[(unsigned char) keycode];
				vidptr[location++] = VGA_COLOR_GREEN;
			} else {
				;
			}
		}
		else {
			;
		}
	}
}

void kmain(void) {
	write_port(0x21 , 0xFD);
	write_port(0x20, 0x20);
	clear();
	kprint("Hello, world!", VGA_COLOR_GREEN);
	knewline();
	kprint("Test", VGA_COLOR_GREEN);
	knewline();
	kprint("Terminal -$ ", VGA_COLOR_GREEN);
	term();
	return;
}
