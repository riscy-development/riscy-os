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

static void fdt_dump(struct fdt *fdt) {
  struct fdt_node *node = fdt_node_begin(fdt);
  int depth = 1;

  print("FDT {\n");
  while(node != NULL && depth > 0) {
    for(int i = 0; i < depth; i++) {
      putchar('\t');
    }
    print(fdt_node_name(node));
    putchar('\n');

    struct fdt_node *subnode = fdt_node_subnode_begin(fdt, node);
    if(subnode) {
      depth += 1;
      node = subnode;
      continue;
    }
    struct fdt_node *sibling_node = fdt_node_next_subnode(fdt, node);
    if(sibling_node) {
      node = sibling_node;
      continue;
    }

    node = fdt_next_node(fdt, node);
    depth -= 1;
  }
  print("}\n");
}

void kmain(uint64_t hartid, struct fdt *fdt) {
	if(fdt_verify(fdt)) {
		print("Cannot Read the FDT!\n");
	} else {
		print("Supported FDT Version\n");
	}

        fdt_dump(fdt);

	while(1) {
	}
	return;
}
