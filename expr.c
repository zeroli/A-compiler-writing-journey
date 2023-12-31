#include "defs.h"

#define extern_ extern
#include "data.h"

#include "decl.h"

static int OpPrep[] = {
	0,  // EOF
	10,  // +
	10,  // -
	20,  // *
	20,  // /
	0  // INTLIT
};

static int op_precedence(int tokentype) {
	int prec = OpPrep[tokentype];
	if (prec == 0) {
		fprintf(stderr, "syntax error on line %d, token %d\n",
			Line, tokentype);
		exit(1);
	}
	return prec;
}

// convert a token into an AST operation.
int arithop(int tok) {
	switch (tok) {
		case T_PLUS:
			return (A_ADD);
		case T_MINUS:
			return (A_SUBTRACT);
		case T_STAR:
			return (A_MULTIPLY);
		case T_SLASH:
			return (A_DIVIDE);
		default:
			fprintf(stderr, "unknown token in arithop() on line %d\n", Line);
			exit(1);
	}
}

// parse a primary factor and return an
// AST node representing it
static struct ASTnode* primary(void) {
	struct ASTnode* n;

	// for an INTLIT token, make a leaf AST node for it
	// and scan in the next token. Otherwise, a syntax error
	// for any other token type
	switch (Token.token) {
	case T_INTLIT:
		n = mkastleaf(A_INTLIT, Token.intvalue);
		scan(&Token);
		return n;
	default:
		fprintf(stderr, "syntax error on line %d\n", Line);
		exit(1);
	}
}

// Return the AST tree whose root is a binary operator
struct ASTnode* binexpr(int ptp) {
	struct ASTnode* n, *left, *right;
	int tokentype;

	// Get the integer literal on the left
	// fetch the next token at the same time
	left = primary();

	// if no tokens left, return just the left node
	tokentype = Token.token;
	if (tokentype == T_EOF)
		return (left);

	// while the precedence of this token is
	// more than that of the previous token precedence
	while (op_precedence(tokentype) > ptp) {
		// fetch in the next integer literal
		scan(&Token);

		// recursively call binexpr() with the
		// precedence of our token to build a sub-tree
		right = binexpr(OpPrep[tokentype]);

		// join the sub-tree with ours. convert the token
		// into an AST operation at the same time
		left = mkastnode(arithop(tokentype), left, right, 0);

		// update the details of the current token.
		// if no tokens left, return just the left node
		tokentype = Token.token;
		if (tokentype == T_EOF)
			return left;
	}

	// return the tree we have when the precedence
	// is the same or lower
	return left;
}
