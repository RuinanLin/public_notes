#include <stdio.h>

void print_using_index(int count, int *ages, char **names) {
	int i;
	for (i = 0; i < count; i++) {
		printf("%s has %d years alive.\n",
						names[i], ages[i]);
	}
}

void print_using_pointers(int count, int *cur_age, char **cur_name) {
	int i;
	for (i = 0; i < count; i++) {
		printf("%s is %d years old.\n",
						*(cur_name+i), *(cur_age+i));
	}
}

void print_using_stupid_complex_way(int count, int *ages, char **names) {
	int *cur_age;
	char **cur_name;
	for (cur_name = names, cur_age = ages;
					(cur_age - ages) < count;
					cur_name++, cur_age++) {
		printf("%s lived %d years so far.\n",
						*cur_name, *cur_age);
	}
}

int main(int argc, char *argv[]) {
	// create two arrays we care about
	int ages[] = {23, 43, 12, 89, 2};
	char *names[] = {
		"Alan", "Frank",
		"Mary", "John", "Lisa"
	};

	// safely get the size of ages
	int count = sizeof(ages) / sizeof(int);

	// first way using indexing
	print_using_index(count, ages, names);
	printf("---\n");

	// setup the pointers to the start of the arrays
	int *cur_age = ages;
	char **cur_name = names;

	// second way using pointers
	print_using_pointers(count, cur_age, cur_name);
	printf("---\n");

	// third way, pointers are just arrays
	print_using_index(count, cur_age, cur_name);
	printf("---\n");

	// fourth way with pointers in a stupid complex way
	print_using_stupid_complex_way(count, ages, names);

	return 0;
}
