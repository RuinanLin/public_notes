#include <stdio.h>

int main(int argc, char *argv[]) {
	int a = 40;
	int *p = &a;
	printf("p = %p, a = %d\n", p, *p);
	return 0;
}
