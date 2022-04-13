#include "includes/colors.h"
#include "includes/keymap.h"

// true and false
#define true 1
#define false 0

// import the commands from asm
extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);

char *vidptr = (char*)0xb8000; 	//video mem begins here.
// location on screen in bytes
unsigned int location = 0;
// a counter used for everything
unsigned int counter = 0;
// keyboard status variable
unsigned char kbstatus;
// keyboard keycode variable
char keycode;
//right after video memory. used for storing the terminal command
char *command = (char*)0xb8fa1;
// a counter used for the keyboard
unsigned int kbcounter = 0;

void clear() {
	counter = 0;
	location = 0;
	while(counter < 80 * 25 * 2) {
		// fill screen with blanks
		vidptr[counter] = ' ';
		// makes characters grey.
		vidptr[counter+1] = 0x07; 		
		counter = counter + 2;
	}
}

void kprint(const char *str, int color) {
	counter = 0;
	//run until end of string is found
	while(str[counter] != '\0') {
		// ascii of character written to the memory
		vidptr[location] = str[counter];
		// gives color to letter
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
	while(true) {
		// get keyboard status
		kbstatus = read_port(0x64);
		if (kbstatus & 0x01) {
			//get current keycode
			keycode = read_port(0x60);
			if (keycode == 0x1C) {
				//runs if enter is pressed
				knewline();
				counter = 0;
				
				if (((char)command[0] == 'c') && ((char)command[1] == 'l') && ((char)command[2] == 'e') && ((char)command[3] == 'a') && ((char)command[4] == 'r')) {
					clear();
				} else if (((char)command[0] == 'e') && ((char)command[1] == 'x') && ((char)command[2] == 'i') && ((char)command[3] == 't')) {
					kprint("Shutting down.", VGA_COLOR_GREEN);
					return;
				} else {
					kprint("Invalid command.", VGA_COLOR_GREEN);
					knewline();
				}
				
				counter = 0;
				// clear terminal input memory
				while (counter != 200) {
					command[counter] = ' ';
					counter++;
				}
				// show new prompt
				kbcounter = 0;
				kprint("Terminal -$ ", VGA_COLOR_GREEN);
			} else if (!(keycode < 0)) {
				// print character on screen
				vidptr[location++] = keyboard_map[(unsigned char) keycode];
				vidptr[location++] = VGA_COLOR_GREEN;
				
				// add keyboard output to the input
				command[kbcounter] = keyboard_map[(unsigned char) keycode];
				kbcounter++;
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
	// init keyboard
	write_port(0x21 , 0xFD);
	write_port(0x20, 0x20);
	// get ready to load.
	clear();
	kprint("Hello, world!", VGA_COLOR_GREEN);
	knewline();
	kprint("Test", VGA_COLOR_GREEN);
	knewline();
	// start terminal
	kprint("Terminal -$ ", VGA_COLOR_GREEN);
	term();
	return;
}
