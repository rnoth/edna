#include <stdlib.h>
#include <string.h>

#include "edna.h"
#include "buffer.h"
#include "line.h"
#include "util.h"
#include "vector.h"

int
addline (Buffer *buf, Line *new)
{
	Line *li = NULL;

	if (new == NULL)
		return FAIL;
	
	li = buftell (buf);

	linklines (new, getnext (li));
	linklines (li, new);

	if (buf->bot == li)
		buf->bot = new;

	++buf->len;
	buf->dirty = 1;

	return SUCC;
}

int
rmcurline (Buffer *buf)
{
	Line *del;

	if (buf->cur == buf->top)
		return FAIL;

	del = buf->cur;

	if (bufseek (buf, BUF_CUR, 1) == FAIL)
		bufseek (buf, BUF_CUR, 1);

	freelines (del, getnext (del));
	buf->dirty = 1;

	return SUCC;
}

int
rmline (Buffer *buf, Line *li)
{
	if (li == NULL || li == buf->top)
		return FAIL;
	buf->cur = li;
	buf->pos = getlineno (li);

	if (bufseek (buf, BUF_CUR, 1) == FAIL)
		bufseek (buf, BUF_CUR, -1);

	freelines (li, getnext (li));
	buf->dirty = 1;

	return SUCC;	
}

Line *
buftell (Buffer *buf)
{
	return buf->cur;
}

int
bufseek (Buffer *buf, int whence, long off)	
{	
	int dir;
	Line *(*get) (Line *); Line *li;

	if (off > 0 && whence == BUF_END)
		return FAIL;
	else if (off < 0 && whence == BUF_SET)
		return FAIL;

	if (off == 0)
		return SUCC;

	switch (whence) {
	 case BUF_SET:
		 li = buf->top;
		 break;
	 case BUF_CUR:
		 li = buf->cur;
		 break;
	 case BUF_END:
		 li = buf->bot;
		 break;
	 default:
		 return FAIL;
	}

	if (off > 0) {
		get = getnext;
		dir = 1;
	} else {
		get = getprev;
		dir = -1;
	}


	for (;;) {
		li = get (li);
		if (li == NULL || li == buf->top)
			break;
		buf->cur = li;
		buf->pos += dir;
		if (--off == 0)
			break;
	} 

	if (off == 0) return SUCC;
	else return FAIL;
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
makebuf (void)
{
	Buffer *buf;
	
	if (!(buf = calloc (1, sizeof *buf)))
		die ("calloc");
	return (buf);
}

int
initbuf (Buffer *buf, char *filename)
{
	if (filename) {
		if (!(buf->filename = calloc (LINESIZE, sizeof *buf->filename)))
			die ("calloc");

		strcpy (buf->filename, filename);
	}
	
	buf->cur = buf->top = buf->bot = makeline();

	return SUCC;
}
