#include<kernel/stdint.h>
#include<kernel/string.h>
#include<kernel/of/fdt.h>

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

void kmain(uint64_t hartid, struct fdt *fdt) {
	if(verify_fdt(fdt)) {
		print("Supported FDT Version\n");
	} else {
		print("Cannot Read the FDT (Unsupported Version!)\n");
	}
	while(1) {
	}
	return;
}
