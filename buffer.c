#include <stdlib.h>
#include <string.h>

#include "edna.h"
#include "buf.h"
#include "line.h"
#include "util.h"
#include "vector.h"

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
	if (filename) {
		if (!(buf->filename = calloc (LINESIZE, sizeof *buf->filename)))
			die ("calloc");

		strcpy (buf->filename, filename);
	}
	
	return (buf);
}

void
setcurline (Buffer *buf, Line *li)
{
	buf->curline = li;
	buf->lineno = getlineno (li);
}
