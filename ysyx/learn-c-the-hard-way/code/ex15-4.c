#include <stdio.h>

int main(int argc, char *argv[]) {
	char **p = argv;
	for (p = argv; p < argv + argc; p++) {
		printf("%s\n", *p);
	}
	return 0;
}
