/* buffer.c -- buffer routines */
#include <stdlib.h>
#include <string.h>

#include "edna.h"
#include "vector.h"

int
addbuf (State *st, Buffer *buf)
{
	/* TODO: no error handling */
	vec_append (st->buffers, buf);
	return SUCC;
}

int
addline (Buffer *buf, Line *new, size_t whence)
{
	Line *li = NULL;

	if (new == NULL)
		return (FAIL);

	if (whence > buf->len)
		return (FAIL);

	if (whence) {
		li = walk (buf->top, whence);
		linklines (new, getnext (li));
		linklines (li, new);
	} else
		linklines (new, buf->top);

	if (buf->top == NULL)
		buf->top = new;
	else if (getprev(buf->top))
		buf->top = getprev (buf->top);
	if (buf->bot == li)
		buf->bot = new;
	if (buf->curline == NULL)
		setcurline (buf, new);

	++buf->len;

	return (SUCC);
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

	if (filename) {
		strcpy (buf->filename, filename);
	}
	
	return buf;
}

void
setcurline (Buffer *buf, Line *li)
{
	buf->curline = li;
	buf->lineno = getlineno (li);
}

int
returnbuf (Buffer *src, State *st)
{
	Buffer *tmp;

	tmp = makebuf (NULL);

	memcpy (tmp, src, sizeof *tmp);
	vec_append (st->buffers, tmp);
	memset (src, 0, sizeof *tmp);
	return SUCC;
}

int
rmbuf (State *st, size_t which)
{
	vec_remove (st->buffers, which);
	return SUCC;
}
