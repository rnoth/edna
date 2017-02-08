#ifndef _edna_buf_
#define _edna_buf_

#include <stdio.h>
#include <stdlib.h>

#include "edna.h"
#include "state.h"

enum {
	BUF_SET,
	BUF_CUR,
	BUF_END,
};

struct Buffer {
	/* file info */
	int	dirty;
	FILE*	file;
	char*	filename;

	/* line info */
	size_t	len;
	Line*	top;
	Line*	bot;
	Line*	cur;
	size_t	pos;

	/* misc. info */
	Mode*	mode;
};

/* buffer.c */
extern int	addline		(Buffer *, Line *);	
extern Line*	buftell		(Buffer *);
extern int	bufseek		(Buffer *, int, long);
extern Buffer*	makebuf		(char *);
extern void	freebuf		(Buffer *);
extern int	rmcurline	(Buffer *);
extern int	rmline		(Buffer *, Line *);

/* file.c */
extern int	readbuf		(Buffer *, char *);
extern int	writebuf	(Buffer *, char *);

#endif
