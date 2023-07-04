#include "defs.h"

#define extern_ extern
#include "data.h"

#include "decl.h"

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
struct ASTnode* binexpr(void) {
	struct ASTnode* n, *left, *right;
	int nodetype;

	// Get the integer literal on the left
	// fetch the next token at the same time
	left = primary();

	// if no tokens left, return just the left node
	if (Token.token == T_EOF)
		return (left);
	
	// convert the token into a node type
	nodetype = arithop(Token.token);

	// get the next token in
	scan(&Token);

	// recursively get the right-hand tree
	right = binexpr();

	// Now build a tree with both sub-trees
	n = mkastnode(nodetype, left, right, 0);
	return n;
}

