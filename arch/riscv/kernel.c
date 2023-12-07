#include<kernel/stdint.h>
#include<kernel/string.h>

unsigned char * uart = (unsigned char *)0x10000000; 
void putchar(char c) {
	*uart = c;
	return;
}

void print(const char * str) {
	while(*str != '\0') {
		putchar(*str);
		str++;
	}
	return;
}

__attribute__ ((constructor)) void foo(void)
{
	print("foo is running and print is available at this point\n");
}

void kmain(void) {
	print("Hello world!\r\n");
	while(1) {
		// Read input from the UART
		putchar(*uart);
	}
	return;
}
