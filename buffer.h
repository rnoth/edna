#ifndef _edna_buf_
#define _edna_buf_

#include <stdbool.h>
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

/* buffer.c */
extern int	addline		(Buffer, Line *);	
extern Buffer	clonebuf	(Buffer);
extern void	copybuf		(Buffer, Buffer);
extern void	bufclean	(Buffer);
extern size_t	bufgetlen	(Buffer);
extern char*	bufgetname	(Buffer);
extern size_t	bufgetpos	(Buffer);
extern int	bufname		(Buffer, char *);
extern int	bufopen		(Buffer, char *);
extern Line*	bufprobe	(Buffer, size_t off);
extern int	bufseek		(Buffer, int, long);
extern int	bufseekline	(Buffer, Line *);
extern Line*	buftell		(Buffer);
extern void	freebuf		(Buffer);
extern FILE*	getfile		(Buffer);
extern bool	isdirty		(Buffer);
extern int	initbuf		(Buffer, char *);
extern void	killbuf		(Buffer);
extern Buffer	makebuf		(void);
extern int	rmcurline	(Buffer);
extern int	rmline		(Buffer, Line *);
extern int	setfilename	(Buffer, char *fn);

/* file.c */
extern int	readbuf		(Buffer, char *);
extern int	writebuf	(Buffer, char *);

#endif
