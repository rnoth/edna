/* addr.h -- line addressing operations */
#ifndef _edna_addr_
#define _edna_addr_

#include "edna.h"

typedef unsigned long Set;
typedef unsigned char Rule;

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

extern size_t forwl	(Set *, size_t, char *, State *);
extern size_t backl	(Set *, size_t, char *, State *);
extern size_t forwb	(Set *, size_t, char *, State *);
extern size_t backb	(Set *, size_t, char *, State *);
extern size_t neg	(Set *, size_t, char *, State *);
extern size_t add	(Set *, size_t, char *, State *);
extern size_t sub	(Set *, size_t, char *, State *);
extern size_t rep	(Set *, size_t, char *, State *);
extern size_t range	(Set *, size_t, char *, State *);
extern size_t expmac	(Set *, size_t, char *, State *);
extern size_t expmrk	(Set *, size_t, char *, State *);
extern size_t empreg	(Set *, size_t, char *, State *);

#endif
