/* addr.h -- line addressing operations */
#ifndef _edna_addr_
#define _edna_addr_

#include "edna.h"
#include "set.h"

typedef unsigned char Rule;

typedef Set	(*Operator)	(Set, Set, Buffer *, char *);
typedef Set	(*Unary)	(char *, Set, Buffer *, char *);
typedef Set	(*Evaluator)	(char *, Buffer *, char *);

enum direc {
	LEFT,
	RIGHT,
};

enum ident {
	NUM_LITERAL,
	NUM_SYMBOL,
	REG_LINE,
	REG_BUF,
	UN_OP,
	BIN_OP,
	SUBSTITUTION,
	IDENT_LEN
};

struct tokaddr {
	char*		str;
	size_t		len;
	enum ident*	stack;
	size_t		height;
};

extern const Evaluator	evals[];
extern const Unary	unops[];

extern Set	defaultarg	(char *, Buffer *, enum direc, char *);
extern Operator	getbinop	(char *);
extern Line**	resolveset	(Set, size_t, Buffer *, char *);

extern Set	num	(char *, Buffer *, char *);
extern Set	symnum	(char *, Buffer *, char *);

extern Set	neg	(char *, Set, Buffer *, char *);

extern Set	add	(Set, Set, Buffer *, char *);
extern Set	sub	(Set, Set, Buffer *, char *);
extern Set	range	(Set, Set, Buffer *, char *);
extern Set	and	(Set, Set, Buffer *, char *);
extern Set	or	(Set, Set, Buffer *, char *);
extern Set	xor	(Set, Set, Buffer *, char *);

#endif
