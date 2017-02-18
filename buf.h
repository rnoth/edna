#ifndef _edna_buf_
#define _edna_buf_
#include "edna.h"

#define isdirty(b) ((b)->dirty)

enum {
	BUF_SET,
	BUF_CUR,
	BUF_END,
};

struct Buffer {
	/* file info */
	bool	dirty;
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
extern Buffer *	clonebuf	(Buffer *);
extern void	copybuf		(Buffer *, Buffer *);
extern void	freebuf		(Buffer *);
extern int	initbuf		(Buffer *, char *);
extern void	killbuf		(Buffer *);
extern Buffer *	makebuf		(void);

/* buf-file.c */
extern void	bufclean	(Buffer *);
extern char *	bufgetname	(Buffer *);
extern int	bufsetname 	(Buffer *, char *);
extern int	bufopen		(Buffer *, char *);
extern int	setfilename	(Buffer *, char *fn);

/* buf-line.c */
extern int	addline		(Buffer *, Line *);	
extern int	rmcurline	(Buffer *);
extern int	rmline		(Buffer *, Line *);

/* buf-pos.c */
extern size_t	bufgetlen	(Buffer *);
extern size_t	bufgetpos	(Buffer *);
extern Line *	bufprobe	(Buffer *, size_t off);
extern int	bufseek		(Buffer *, int, long);
extern int	bufseekline	(Buffer *, Line *);
extern Line *	buftell		(Buffer *);

/* file.c */
extern int	readbuf		(Buffer *, char *);
extern int	writebuf	(Buffer *, char *);

#endif
