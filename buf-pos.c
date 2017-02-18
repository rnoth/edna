#include "edna.h"

size_t
bufgetpos(Buffer *buf)
{
	assert(buf != NULL);

	return buf->pos;
}

size_t
bufgetlen(Buffer *buf)
{
	assert(buf != NULL);

	return buf->len;
}

Line *
bufprobe(Buffer *buf, size_t off)
{
	assert(buf != NULL);

	if (off > buf->len) return NULL;

	return walk(buf->top, off);
}

int
bufseek(Buffer *buf, int whence, long off)	
{	
	int dir;
	Line *(*get)(Line *); Line *li;

	assert(buf != NULL);
	assert(off > 0 || whence != BUF_SET);
	assert(off < 0 || whence != BUF_END);

	if (off == 0) return SUCC;

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
		if (li == NULL) break;
		buf->cur = li;
		buf->pos += dir;
		if (--off == 0) break;
	} 

	if (off == 0) 	return SUCC;
	else 		return FAIL;
}

int
bufseekline(Buffer *buf, Line *li)
{
	size_t off;
	Line *tmp;

	off = 0;
	for (tmp = buf->top; tmp && tmp != li; tmp = getnext(tmp))
		++off;
	if (!tmp) return FAIL;

	buf->pos = off;
	buf->cur = tmp;

	return SUCC;
}

Line *
buftell(Buffer *buf)
{
	return buf->cur;
}
