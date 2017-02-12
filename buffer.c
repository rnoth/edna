#include <stdlib.h>
#include <string.h>

#include "edna.h"
#include "buffer.h"
#include "line.h"
#include "util.h"
#include "vector.h"

int
addline(Buffer *buf, Line *new)
{
	Line *li = NULL;

	if (new == NULL)
		return FAIL;
	
	li = buftell(buf);

	linklines(new, getnext(li));
	linklines(li, new);

	if (buf->bot == li)
		buf->bot = new;

	++buf->len;
	buf->dirty = 1;

	++buf->pos;
	buf->cur = new;

	return SUCC;
}

int
bufopen(Buffer *buf, char *mode)
{
	char *fn;

	if (buf->name == NULL)
		return FAIL;

	fn = strtochar(buf->name);
	if (buf->file)
		buf->file = freopen(fn, mode, buf->file);
	else
		buf->file = fopen(fn, mode);
	if (buf->file == NULL) {
		perror("freopen");
		return FAIL;
	}

	return SUCC;
}

int
bufname(Buffer *buf, char *name)
{
	if (name == NULL)
		return FAIL;
	if (*name == '\0')
		return FAIL;
	freestring(buf->name);
	buf->name = chartostr(name);
	return SUCC;
}

Line *
buftell(Buffer *buf)
{
	return buf->cur;
}

int
bufseek(Buffer *buf, int whence, long off)	
{	
	int dir;
	Line *(*get)(Line *); Line *li;

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
		li = get(li);
		if (li == NULL || li == buf->top)
			break;
		buf->cur = li;
		buf->pos += dir;
		if (--off == 0)
			break;
	} 

	if (off == 0) 	return SUCC;
	else 		return FAIL;
}

void
freebuf(Buffer *buf)
{
	freelines(buf->top, NULL);
	if (buf->file)
		fclose(buf->file);
	freestring(buf->name);
	free(buf);
}

int
initbuf(Buffer *buf, char *fn)
{
	if (fn)
		setfilename (buf, fn);
	
	buf->cur = buf->top = buf->bot = makeline();

	return SUCC;
}

void
killbuf(Buffer *buf)
{
	freelines(buf->top, NULL);
	if (buf->file && (fclose(buf->file) == EOF)) perror ("fclose");
	freestring(buf->name);
	memset(buf, 0, sizeof *buf);
	return;
}

Buffer *
makebuf(void)
{
	Buffer *buf;
	
	if (!(buf = calloc(1, sizeof *buf)))
		die("calloc");
	return buf;
}

int
rmcurline(Buffer *buf)
{
	Line *del;

	if (buf->cur == buf->top)
		return FAIL;

	del = buf->cur;

	if (bufseek(buf, BUF_CUR, 1) == FAIL)
		bufseek(buf, BUF_CUR, 1);

	freelines(del, getnext(del));
	buf->dirty = 1;

	return SUCC;
}

int
rmline(Buffer *buf, Line *li)
{
	if (li == NULL || li == buf->top)
		return FAIL;
	buf->cur = li;
	buf->pos = getlineno(li);

	if (bufseek(buf, BUF_CUR, 1) == FAIL)
		bufseek(buf, BUF_CUR, -1);

	freelines(li, getnext(li));
	buf->dirty = 1;

	return SUCC;	
}

int
setfilename(Buffer *buf, char *fn)
{
	if (fn == NULL)
		return FAIL;
	if (buf->name)
		copychars(buf->name, fn);
	else
		buf->name = clonechars(fn);
	return !!buf->name;
}
