/* buffer.c -- buffer routines */
#include <stdlib.h>
#include <string.h>

#include "edna.h"

extern int	addbuf		(State *, Buffer *);
extern void	freebuf		(Buffer *);
extern Buffer*	makebuf		(char *);
extern int	rmbuf		(State *, Buffer *);

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

void
freebuf (Buffer *buf)
{
	for (; buf->curline->prev;)
		buf->curline = buf->curline->prev;
	freelines (buf->curline, NULL);
	if (buf->file)
		fclose (buf->file);
	free (buf->filename);
	free (buf);
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
	}
	
	return buf;
}

int
rmbuf (State *st, Buffer *buf)
{
	size_t i;
	for (i = 0; ++i < st->buflen && st->buffers[i] != buf;)
		;
	if (i == st->buflen)
		return -1;
	if (i == st->bufno) {
		st->curbuf = st->buffers[i - 1];
		st->bufno = i - 1;
	}
	if (i < st->bufno)
		--st->bufno;
	if (!(memmove (st->buffers[i], st->buffers[i+1], st->buflen - i)))
		die ("memmove");
	--st->buflen;
	return 0;
}
