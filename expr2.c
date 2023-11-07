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

static struct ASTnode* multiplicative_expr()
{
    struct ASTnode* left, *right;
    int tokentype;

    left = primary();

    tokentype = Token.token;
    if (tokentype == T_EOF)
        return left;

    while ((tokentype == T_STAR) || (tokentype == T_SLASH)) {
        scan(&Token);
        right = primary();

        left = mkastnode(arithop(tokentype), left, right, 0);
        tokentype = Token.token;
        if (tokentype == T_EOF)
            break;
    }

    return left;
}

static struct ASTnode* additive_expr()
{
    struct ASTnode* left, *right;
    int tokentype;

    left = multiplicative_expr();
    tokentype = Token.token;
    if (tokentype == T_EOF)
        return left;

    while ((tokentype == T_PLUS) || (tokentype == T_MINUS)) {
        scan(&Token);
        right = multiplicative_expr();
        left = mkastnode(arithop(tokentype), left, right, 0);

        tokentype = Token.token;
        if (tokentype == T_EOF)
            break;
    }
    return left;
}

// Return the AST tree whose root is a binary operator
struct ASTnode* binexpr(int ptp) {
    return additive_expr();
}
