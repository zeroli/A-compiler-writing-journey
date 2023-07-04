#include <stdio.h>
#include <stdlib.h>

#define extern_ 
#include "data.h"

#include "defs.h"

extern int scan(struct token* t);

// List of printable tokens
char* tokstr[] = { "+", "-", "*", "/", "intlit" };

// loop scanning in all the tokens in the input file
// print out details of each token found.
static void scanfile() {
	struct token T;
	
	while (scan(&T)) {
		printf("Token %s", tokstr[T.token]);
		if (T.token == T_INTLIT)
			printf(", value %d", T.intvalue);
		printf("\n");
	}
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("usage: %s <input>\n", argv[0]);
		exit(1);
	}
	
	//init();
	Infile = fopen(argv[1], "r");

	scanfile();

	fclose(Infile);
	return 0;
}
