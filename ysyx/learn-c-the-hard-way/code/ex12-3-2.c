#include <stdio.h>

int main(int argc, char *argv[]) {
	// go through each string in argv
	
	int i = 0;
	while (i < argc) {
		printf("arg %d: %s\n", i, argv[i]);
		i++;
	}

	// let's make our own array of strings
	char *states[] = {
		"California", "Oregon",
		"Washington", "Texas"
	};

	int num_states = 4;
	int break_num = 1;
	i = 0;	// watch for this
	while (i < num_states) {
		printf("state %d: %s\n", i, states[i]);
		if (i >= break_num) {
			break;
		}
		i++;
	}

	return 0;
}
