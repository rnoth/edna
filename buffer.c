/* buffer.c -- buffer routines */
#include <stdlib.h>
#include <string.h>

#include "edna.h"

extern int	addbuf		(State *, Buffer *);
extern Buffer*	makebuf		(char *);

int
addbuf (State *st, Buffer *buf)
{
	Buffer **tmp;
	++st->buflen;
	tmp = realloc (st->buffers, st->buflen * sizeof *st->buffers);
	if (!tmp) {
		warn ("realloc");
		--st->buflen;
		return -1;
	}
	st->buffers = tmp;
	st->buffers[st->buflen - 1] = buf;
	return 0;
}

Buffer *
makebuf (char *filename)
{
	Buffer *buf;
	
	if (!(buf = calloc (1, sizeof *buf)))
		die ("calloc");
	if (!(buf->filename = malloc (LINESIZE * sizeof *buf->filename)))
		die ("malloc");
	buf->curline = makeline();

	if (filename) {
		strcpy (buf->filename, filename);
		readbuf (buf);
	}
	
	return buf;
}
