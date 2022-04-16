#include "includes/colors.h"
#include "includes/keymap.h"

// true and false
#define true 1
#define false 0
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08

// import the commands from asm
extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);
extern void kbhandler(void);

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




struct IDT_entry {
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
};


struct IDT_entry IDT[IDT_SIZE];



void idt_init(void)
{
	unsigned long keyboard_address;
	unsigned long idt_address;
	unsigned long idt_ptr[2];

	keyboard_address = (unsigned long)kbhandler;
	IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
	IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x21].zero = 0;
	IDT[0x21].type_attr = INTERRUPT_GATE;
	IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;

	write_port(0x20 , 0x11);
	write_port(0xA0 , 0x11);

	write_port(0x21 , 0x20);
	write_port(0xA1 , 0x28);

	write_port(0x21 , 0x00);
	write_port(0xA1 , 0x00);

	write_port(0x21 , 0x01);
	write_port(0xA1 , 0x01);


	write_port(0x21 , 0xff);
	write_port(0xA1 , 0xff);

	idt_address = (unsigned long)IDT ;
	idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address >> 16 ;

	load_idt(idt_ptr);
}




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

void term(void) {
	write_port(0x20, 0x20);
	// get keyboard status
	kbstatus = read_port(0x64);
	if (kbstatus & 0x01) {
		//get current keycode
		keycode = read_port(0x60);
		if (keycode == 0x1C) {
			//runs if enter is pressed
			knewline();
			counter = 0;
			command[kbcounter] = '\0';
			
			if (((char)command[0] == 'c') && ((char)command[1] == 'l') && ((char)command[2] == 'e') && ((char)command[3] == 'a') && ((char)command[4] == 'r')) {
				clear();
			} else if (((char)command[0] == 'e') && ((char)command[1] == 'x') && ((char)command[2] == 'i') && ((char)command[3] == 't')) {
				kprint("Shutting down.", VGA_COLOR_GREEN);
				return;
			} else if (((char)command[0] == 'e') && ((char)command[1] == 'c') && ((char)command[2] == 'h') && ((char)command[3] == 'o') && ((char)command[4] == ' ')) {
				counter = 5;
				while(command[counter] != '\0') {
					kprint((char *)command[counter], VGA_COLOR_GREEN);
					counter++;
				}
				knewline();
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
		} else if (keyboard_map[(unsigned char) keycode] == '\b') {
			if (kbcounter <= 0 ) {
				
			} else {
				vidptr[location - 1] = 0x07;
				vidptr[location - 2] = ' ';
				location = location - 2;
				
				command[kbcounter - 1] = ' ';
				kbcounter = kbcounter - 1;
			}
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

void kmain(void) {
	idt_init();
	// init keyboard
	write_port(0x21 , 0xFD);
	// get ready to load.
	clear();
	kprint("Hello, world!", VGA_COLOR_GREEN);
	knewline();
	kprint("Test", VGA_COLOR_GREEN);
	knewline();
	// start terminal
	kprint("Terminal -$ ", VGA_COLOR_GREEN);
	return;
}
