#ifndef _edna_buf_
#define _edna_buf_

#include <stdio.h>
#include <stdlib.h>

#include "edna.h"
#include "state.h"
#include "str.h"

enum {
	BUF_SET,
	BUF_CUR,
	BUF_END,
};

struct Buffer {
	/* file info */
	int	dirty;
	FILE	*file;
	String	*name;

	/* line info */
	size_t	len;
	size_t	pos;
	Line	*top;
	Line	*bot;
	Line	*cur;

	/* misc. info */
	Mode	*mode;
};

/* buffer.c */
extern int	addline		(Buffer *, Line *);	
extern int	bufname		(Buffer *, char *);
extern int	bufopen		(Buffer *, char *);
extern int	bufseek		(Buffer *, int, long);
extern Line*	buftell		(Buffer *);
extern Buffer*	makebuf		(void);
extern void	freebuf		(Buffer *);
extern int	initbuf		(Buffer *, char *);
extern void	killbuf		(Buffer *);
extern int	rmcurline	(Buffer *);
extern int	rmline		(Buffer *, Line *);
extern int	setfilename	(Buffer *, char *fn);

/* file.c */
extern int	readbuf		(Buffer *, char *);
extern int	writebuf	(Buffer *, char *);

#endif
