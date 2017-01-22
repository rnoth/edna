/* addr.h -- line addressing operations */
#ifndef _edna_addr_
#define _edna_addr_

#include "edna.h"
#include "set.h"

typedef unsigned char	Rule;

typedef Set		(*Evaluator)	(String *, Buffer *, char *);

enum direc {
	LEFT,
	RIGHT,
};

enum ident {
	NUM_LITERAL,
	NUM_SYMBOL,
	IDENT_LEN,
};

struct tokaddr {
	String 		*str;
	enum ident	*stack;
	size_t		 height;
};

extern	struct tokaddr*	lexaddr		(String *);
extern 	Selection	evaladdr	(struct tokaddr *, Buffer *, char *);

extern 	Selection	resolveset	(Set, size_t, Buffer *, char *);

extern 	Set		num		(String *, Buffer *, char *);
extern 	Set		symnum		(String *, Buffer *, char *);

#endif
