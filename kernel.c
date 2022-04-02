char *vidptr = (char*)0xb8000; 	//video mem begins here.
unsigned int i = 0;
unsigned int j = 0;


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

void kprint(const char *str) {
  j = 0;
	while(str[j] != '\0') {
		/* the character's ascii */
		vidptr[i] = str[j];
		/* attribute-byte: give character black bg and light grey fg */
		vidptr[i+1] = 0x07;
		++j;
		i = i + 2;
	}
}

void kmain(void) {
  clear();
  kprint("Hello world!");
	return;
}
