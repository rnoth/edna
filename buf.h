#ifndef _edna_buf_
#define _edna_buf_

#include <stdio.h>

#include "edna.h"
#include "state.h"

struct Buffer {
	/* file info */
	int	dirty;
	FILE*	file;
	char*	filename;

	/* line info */
	size_t	len;
	Line*	top;
	Line*	bot;
	Line*	curline;
	size_t	lineno;

	/* misc. info */
	Mode*	mode;
};

/* buffer.c */
extern int	addbuf		(State *, Buffer *);
extern int	addline		(Buffer *, Line *, size_t);
extern int	checkoutbuf	(Buffer *, State *, size_t);
extern Buffer*	makebuf		(char *);
extern void	freebuf		(Buffer *);
extern void	setcurline	(Buffer *, Line *);
extern int	rmbuf		(State *, size_t);
extern int	returnbuf	(Buffer *, State *);

/* file.c */
extern int	readbuf		(Buffer *, char *);
extern int	writebuf	(Buffer *, char *);

#endif
