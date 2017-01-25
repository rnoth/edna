/* buffer.c -- buffer routines */
#include <stdlib.h>
#include <string.h>

#include "edna.h"
#include "vector.h"

extern int	addbuf		(State *, Buffer *);
extern int	checkoutbuf	(Buffer *, State *, size_t);
extern void	freebuf		(Buffer *);
extern Buffer*	makebuf		(char *);
extern int	rmbuf		(State *, size_t);
extern int	returnbuf	(Buffer *, State *);

int
addbuf (State *st, Buffer *buf)
{
	/* TODO: no error handling */
	vec_append (st->buffers, buf);
	return SUCC;
}

int
checkoutbuf (Buffer *dest, State *st, size_t which)
{
	Buffer *src;

	if (which >= st->buffers.c)
		return FAIL;

	src = st->buffers.v[which];
	memcpy (dest, src, sizeof *dest);
	vec_remove (st->buffers, which);

	dest->mode = st->modes.v;

	return SUCC;
}

void
freebuf (Buffer *buf)
{
	freelines (buf->top, NULL);
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
	if (!(buf->filename = calloc (LINESIZE, sizeof *buf->filename)))
		die ("calloc");
	buf->curline = buf->top = buf->bot = makeline();

	if (filename) {
		strcpy (buf->filename, filename);
	}
	
	return buf;
}

int
returnbuf (Buffer *src, State *st)
{
	vec_insert (st->buffers, 0, src);
	return SUCC;
}

int
rmbuf (State *st, size_t which)
{
	vec_remove (st->buffers, which);
	return SUCC;
}
