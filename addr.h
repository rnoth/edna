/* addr.h -- line addressing operations */
#ifndef _edna_addr_
#define _edna_addr_

#include "edna.h"
#include "set.h"

typedef unsigned int	Rule;
typedef struct Node	Node;
typedef enum Token	Token;

/* TODO: merge evaluator and operator, make it take a Set vector */
typedef Set	(*Evaluator)	(Buffer *, char *);
typedef Node*	(*Lexer)	(String *, size_t *);
typedef Set	(*Operator)	(Set, Set, Buffer *, char *);

enum Token {
	NUM_LITERAL,
	NUM_SYMBOL,
	OP_ARITH,
	IDENT_LEN,
};

struct Node {
	Token tok,
	String *str,
	Node *left,
	Node *right,
	Node *up;
};

#define _ "\0	

const char *symbols[] = {
	"1234567890"	_ _,		/* NUM_LITERAL */
	"." _ "$"	_ _,		/* NUM_SYMBOL */
	"+" _ "-"	_ _,		/* OP_ARITH */
	NULL,			/* IDENT_LEN */
};

const Lexer lexers[] = {
	trynum,
	trysym,
	tryarith,
	NULL
}

#undef _

const Evaluator evals[] = {
	num,
	symnum,
};

#define GLOB		BIT(0)	/* treat contiguous permutations as one token */
#define VALUE		BIT(1)	/* symbol to be evaluated */
#define HOMOGENOUS	BIT(2)	/* use one functions for every member */
#define OPERATOR	BIT(3)	/* binary operator */

const Rule ruleset[] = {
	GLAB | VALUE | HOMOGENOUS,
	VALUE,
	OPERATOR,
};

extern 	Selection	evaladdr (struct tokaddr *, Buffer *, char *);

extern 	Selection	resolveset	(Set, size_t, Buffer *, char *);

extern 	Set		num	(String *, Buffer *, char *);
extern 	Set		symnum	(String *, Buffer *, char *);

#endif
