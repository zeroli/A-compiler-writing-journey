#include <errno.h>

#include "defs.h"
#define extern_
#include "data.h"

#include "decl.h"

static void init() {
	Line = 1;
	Putback = '\n';
}

static void usage(char* prog) {
	fprintf(stderr, "Usage: %s infile\n", prog);
	exit(1);
}

int main(int argc, char* argv[]) {
	struct ASTnode* n;

	if (argc != 2) {
		usage(argv[0]);
	}

	init();
	if ((Infile = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "Unable to open %s: %s\n", argv[1], strerror(errno));
		exit(1);
	}

	// create the output file
	if ((Outfile = fopen("out.s", "w")) == NULL) {
		fprintf(stderr, "Unable to create out.s: %s\n", strerror(errno));
		exit(1);
	}

	scan(&Token);
	n = binexpr(0);
	printf("%d\n", interpretAST(n));

	generatecode(n);
	fclose(Outfile);

	fclose(Infile);
	return 0;
}
