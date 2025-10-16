#include <stdio.h>

int main(int argc, char *argv[]) {
	char *str_list[] = {
		"abc", "def", "ghi", "jkl", "mno"
	};

	char **p = str_list;
	int num = 5;
	for (p = str_list; p < str_list + num; p++) {
		printf("%s\n", *p);
	}

	return 0;
}
