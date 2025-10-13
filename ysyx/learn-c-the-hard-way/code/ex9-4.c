#include <stdio.h>

int main(int argc, char *argv[]) {
	char name[4] = {'a', 'b', 'c', 'd'};
	int* p = (int *)name;

	// read
	int read = *p;
	printf("Read out as an int: %d\n", read);
	printf("Turn it into hex: 0x%x\n", read);

	// write
	*p = 0x30313233;
	printf("Read out the written stuff: %c %c %c %c\n", name[0], name[1], name[2], name[3]);

	return 0;
}
