#include "defs.h"

#define extern_ extern
#include "data.h"

#define EOF (-1)

// Get the next character from the input file.
static int next(void) {
	int c;

	if (Putback) {  // Use the character put
		c = Putback;	// back if there is one
		Putback = 0;
		return c;
	}
	
	c = fgetc(Infile);  // read from the input file
	if ('\n' == c)
		Line++;		// increment line count
	return c;
}

static void putback(char c) {
	Putback = c;
}

// Skip past input that we don't need to deal with,
// i.e. whitespace, newlines. return the first
// character we do need to deal with
static int skip(void) {
	int c;

	c = next();
	while (' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c) {
		c = next();
	}
	return (c);
}

// Return the position of character c
// in string s, or -1 if c not found
static int chrpos(char* s, int c) {
	char* p;
	p = strchr(s, c);
	return (p ? p - s : -1);
}

// scan and return an integer literal
// value from the input file. Store
// the vlaue as a string in Text.
static int scanint(int c) {
	int k, val = 0;

	// convert each character into an int value
	while ((k = chrpos("0123456789", c)) >= 0) {
		val = val * 10 + k;
		c = next();
	}

	// we hit a non-integer character, put it back
	putback(c);
	return val;
}

// Scan and return the next token found in the input.
// Return 1 if token valid, 0 if no tokens left.
int scan(struct token *t) {
	int c;
	
	// Skip whitespace
	c = skip();

	// Determine the token based on
	// the input character
	switch (c) {
	case EOF:
		t->token = T_EOF;
		return (0);
	case '+':
		t->token = T_PLUS;
		break;
	case '-':
		t->token = T_MINUS;
		break;
	case '*':
		t->token = T_STAR;
		break;
	case '/':
		t->token = T_SLASH;
		break;
	default:
		// if it's a digit, scan the
		// literal integer value in
		if (isdigit(c)) {
			t->intvalue = scanint(c);
			t->token = T_INTLIT;
			break;
		}
		printf("Unrecognized character %c on line %d\n", c, Line);
		exit(1);
	}

	// we found a token
	return (1);
}


