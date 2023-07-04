#ifndef DEFS_H_
#define DEFS_H_

// Token structure
struct token {
	int token;
	int intvalue;
};

// Tokens
enum {
	T_PLUS, T_MINUS, T_STAR, T_SLASH, T_INTLIT
};

#endif // DEFS_H_

