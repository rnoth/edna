#ifndef _edna_line_
#define _edna_line_
#include "edna.h"

struct Line {
	size_t	len;
	String*	str;
	Line*	next;
	Line*	prev;
};

/* line.c */
extern size_t	getlineno	(Line *);
extern Line*	getnext		(Line *);
extern Line*	getprev		(Line *);
extern void	freelines	(Line *, Line *);
extern void	linklines	(Line *, Line*);
extern Line*	makeline	(void);
extern int	changeline	(Line *, String *);
extern Line*	walk		(Line *, int);
#endif
