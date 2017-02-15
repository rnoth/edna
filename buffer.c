#include <stdlib.h>
#include <string.h>

#include "edna.h"
#include "buffer.h"
#include "line.h"
#include "util.h"
#include "vector.h"

typedef struct _buffer _buffer;
struct _buffer {
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

int
addline(Buffer buf, Line *new)
{
	_buffer *_buf = buf.v;
	Line *li = NULL;

	if (new == NULL)
		return FAIL;
	
	li = buftell(buf);

	linklines(new, getnext(li));
	linklines(li, new);

	if (_buf->bot == li)
		_buf->bot = new;

	++_buf->len;
	_buf->dirty = 1;

	++_buf->pos;
	_buf->cur = new;

	return SUCC;
}

void
bufclean(Buffer buf)
{
	_buffer *_buf = buf.v;

	_buf->dirty = 0;
}

char *
bufgetname(Buffer buf)
{
	_buffer *_buf = buf.v;

	return strtochar(_buf->name);
}

size_t
bufgetpos(Buffer buf)
{
	_buffer *_buf = buf.v;

	return _buf->pos;
}

size_t
bufgetlen(Buffer buf)
{
	_buffer *_buf = buf.v;

	return _buf->len;
}

int
bufname(Buffer buf, char *name)
{
	_buffer *_buf = buf.v;

	if (name == NULL)
		return FAIL;
	if (*name == '\0')
		return FAIL;

	freestring(_buf->name);
	_buf->name = chartostr(name);
	chompstr(_buf->name);

	return SUCC;
}

int
bufopen(Buffer buf, char *mode)
{
	_buffer *_buf = buf.v;
	char *fn;

	if (_buf->name == NULL) return FAIL;

	fn = strtochar(_buf->name);

	if (!fn || !*fn) {
		free(fn);
		return FAIL;
	}

	if (_buf->file) fclose(_buf->file);
	_buf->file = fopen(fn, mode);

	if (_buf->file == NULL) {
		perror("fopen");
		return FAIL;
	}

	return SUCC;
}

Line *
bufprobe(Buffer buf, size_t off)
{
	_buffer *_buf = buf.v;

	if (off > _buf->len) return NULL;

	return walk(_buf->top, off);
}

int
bufseek(Buffer buf, int whence, long off)	
{	
	int dir;
	_buffer *_buf = buf.v;
	Line *(*get)(Line *); Line *li;

	if (off > 0 && whence == BUF_END)
		return FAIL;
	else if (off < 0 && whence == BUF_SET)
		return FAIL;

	if (off == 0)
		return SUCC;

	switch (whence) {
	 case BUF_SET:
		 li = _buf->top;
		 break;
	 case BUF_CUR:
		 li = _buf->cur;
		 break;
	 case BUF_END:
		 li = _buf->bot;
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
		if (li == NULL) break;
		_buf->cur = li;
		_buf->pos += dir;
		if (--off == 0) break;
	} 

	if (off == 0) 	return SUCC;
	else 		return FAIL;
}

int
bufseekline(Buffer buf, Line *li)
{
	_buffer *_buf = buf.v;
	size_t off;
	Line *tmp;

	off = 0;
	for (tmp = _buf->top; tmp && tmp != li; tmp = getnext(tmp))
		++off;
	if (!tmp) return FAIL;

	_buf->pos = off;
	_buf->cur = tmp;

	return SUCC;
}

Line *
buftell(Buffer buf)
{
	_buffer *_buf = buf.v;
	return _buf->cur;
}

Buffer
clonebuf(Buffer src)
{
	Buffer ret;
	_buffer *_src, *_ret;

	_src = src.v;

	ret = makebuf();
	_ret = ret.v;

	memcpy(_ret, _src, sizeof *_ret);

	return ret;
}

void
copybuf(Buffer dest, Buffer src)
{
	memcpy(dest.v, src.v, sizeof(_buffer));
}

void
freebuf(Buffer buf)
{
	_buffer *_buf = buf.v;

	freelines(_buf->top, NULL);
	if (_buf->file)
		fclose(_buf->file);
	freestring(_buf->name);
	free(_buf);

	return;
}

FILE *
getfile(Buffer buf)
{
	_buffer *_buf = buf.v;
	return _buf->file;
}

int
initbuf(Buffer buf, char *fn)
{
	_buffer *_buf = buf.v;

	if (fn) setfilename (buf, fn);
	
	_buf->cur = _buf->top = _buf->bot = makeline();
	_buf->len = 1;

	return SUCC;
}

bool
isdirty(Buffer buf)
{
	_buffer *_buf = buf.v;

	return _buf->dirty;
}

void
killbuf(Buffer buf)
{
	_buffer *_buf = buf.v;

	freelines(_buf->top, NULL);
	if (_buf->file && (fclose(_buf->file) == EOF)) perror ("fclose");
	freestring(_buf->name);
	memset(_buf, 0, sizeof *_buf);

	return;
}

Buffer
makebuf(void)
{
	Buffer ret;
	_buffer *_ret;
	
	if (!(_ret = calloc(1, sizeof *_ret))) die("calloc");

	ret.v = _ret;

	return ret;
}

int
rmcurline(Buffer buf)
{
	_buffer *_buf = buf.v;
	Line *del;

	if (_buf->cur == _buf->top) return FAIL;

	del = _buf->cur;

	if (bufseek(buf, BUF_CUR, 1) == FAIL)
		bufseek(buf, BUF_CUR, 1);

	freelines(del, getnext(del));
	_buf->dirty = 1;
	--_buf->len;

	return SUCC;
}

int
rmline(Buffer buf, Line *li)
{
	_buffer *_buf = buf.v;

	if (li == NULL || li == _buf->top) return FAIL;

	bufseekline(buf, li);

	if (bufseek(buf, BUF_CUR, 1) == FAIL)
		bufseek(buf, BUF_CUR, -1);

	freelines(li, getnext(li));
	_buf->dirty = 1;
	--_buf->len;

	return SUCC;	
}

int
setfilename(Buffer buf, char *fn)
{
	_buffer *_buf = buf.v;

	if (fn == NULL) return FAIL;

	if (_buf->name)
		copychars(_buf->name, fn);
	else
		_buf->name = clonechars(fn);
	return !!_buf->name;
}
