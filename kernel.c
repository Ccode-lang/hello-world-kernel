#include "includes/colors.h"
#include "includes/map.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define ENTER_KEY_CODE 0x1C

extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);

char *vidptr = (char*)0xb8000; 	//video mem begins here.
unsigned int i = 0;
unsigned int j = 0;

void kb_init(void) {
	write_port(0x21 , 0xFD);
}

void clear() {
  j = 0;
  i = 0;
	while(j < 80 * 25 * 2) {
		/* blank character */
		vidptr[j] = ' ';
		/* attribute-byte - light grey on black screen */
		vidptr[j+1] = 0x07; 		
		j = j + 2;
	}
}

void kprint(const char *str, int color) {
  j = 0;
	while(str[j] != '\0') {
		/* the character's ascii */
		vidptr[i] = str[j];
		/* attribute-byte: give character black bg and light grey fg */
		vidptr[i+1] = color;
		++j;
		i = i + 2;
	}
}

void kmain(void) {
  clear();
  kb_init()
  kprint("Hello, world!", VGA_COLOR_GREEN);
  return;
}
