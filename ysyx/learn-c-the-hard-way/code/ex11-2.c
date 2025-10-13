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
	i = 0;	// watch for this
	while (i < num_states) {
		printf("state %d: %s\n", i, states[i]);
		i++;
	}

	// copy from argv to states
	int num_copied_elems = (argc < num_states) ? argc : num_states;
	i = 0;
	while (i < num_copied_elems) {
		states[i] = argv[i];
		i++;
	}

	// display copied results
	printf("-------------------------\n");
	printf("states[] after copying from argv[]\n");
	i = 0;
	while (i < num_states) {
		printf("state %d: %s\n", i, states[i]);
		i++;
	}

	return 0;
}
