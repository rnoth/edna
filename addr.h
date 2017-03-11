/* addr.h -- line addressing operations */
#ifndef _edna_addr_
#define _edna_addr_

#include "edna.h"

#define VALUE		BIT(0)	/* 0-ary operator */
#define OPERATOR	BIT(1)	/* binary operator */
#define NUMBER		BIT(2)	/* context-sensitive numerical value */
#define LINE		BIT(3)  /* raw line address */

enum Token {
	NUM_LITERAL,
	NUM_SYMBOL,
	OP_ARITH,
	IDENT_LEN
};

typedef unsigned int	Rule;
typedef enum Token	Token;
typedef struct Symbol	Symbol;

/* TODO: merge evaluator and operator, make it take a Set vector */
typedef Set *	(*Evaluator)	(Node *, Buffer *, char *);
typedef Node*	(*Lexer)	(String *, size_t *);
typedef Set *	(*Operator)	(Node *, Node *, Buffer *, char *);

struct Node {
	Token  tok;
	String *str;
	Evaluator ev;
	Operator op;
	Node *left;
	Node *right;
	Node *up;
};

/*
 * prototypes
 */

/* core */
extern 	Set *		evaltree	(Node *, Buffer *, char *);
extern	void*		getaddr		(String *, size_t *, Buffer *, char *);
extern	Node*		next		(String *, size_t *);
extern	Node*		parseaddr	(String *, size_t *, char *);

/* util */
extern 	Selection*	resolveset	(Set *, Buffer *, char *);

/* lex */
extern	Node*		trynum		(String *, size_t *);
extern	Node*		trysym		(String *, size_t *);
extern	Node* 		tryarith	(String *, size_t *);

/* tree */
extern int		addnode		(Node *, Node *);
extern Node*		getroot		(Node *);
extern int		extendbranch_r	(Node *, Node *);
extern void		freetree	(Node *);
extern void		freenode	(Node *);
extern Node*		makenode	(void);

/* evaluators */
extern Set *		addr_num	(Node *, Buffer *, char *);
extern Set *		addr_dot	(Node *, Buffer *, char *);
extern Set *		addr_dollar	(Node *, Buffer *, char *);

/* operators */
extern Set *		addr_plus	(Node *, Node *, Buffer *, char *);

/* tables */
extern const char*	symbols[];
extern const Evaluator	nums[];
extern const Evaluator	numsyms[];
extern const Operator	arithops[];
extern const Lexer	lexers[];
extern const Rule	ruleset[];

#endif
