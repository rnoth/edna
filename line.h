#ifndef _edna_line_
#define _edna_line_
#include "edna.h"
#include "str.h"
#include "vector.h"

struct Line {
	size_t	len;
	String*	str;
	Line*	next;
	Line*	prev;
};

VECTOR_TAG (Line *, Selection);

/* line.c */
extern size_t	getlineno	(const Line *);
extern Line*	getnext		(const Line *);
extern Line*	getprev		(const Line *);
extern void	freelines	(Line *, Line *);
extern void	linklines	(Line *, Line*);
extern Line*	makeline	(void);
extern int	changeline	(Line *, String *);
extern Line*	walk		(Line *, int);
#endif
