#include "edna.h"

int
addline(Buffer *buf, Line *new)
{
	Line *li = NULL;

	assert(buf != NULL);
	assert(new != NULL);
	
	li = buftell(buf);

	linklines(new, getnext(li));
	linklines(li, new);

	if (buf->bot == li) buf->bot = new;

	++buf->len;
	buf->dirty = 1;

	++buf->pos;
	buf->cur = new;

	return 0;
}


int
rmcurline(Buffer *buf)
{
	Line *del;

	assert (buf != NULL);

	del = buf->cur;

	if (bufseek(buf, BUF_CUR, 1) == FAIL)
		if (bufseek(buf, BUF_CUR, 1) == FAIL)
			return -1;

	freelines(del, getnext(del));
	buf->dirty = 1;
	--buf->len;

	return 0;
}

int
rmline(Buffer *buf, Line *li)
{
	assert(buf != NULL);
	assert(li != NULL);

	bufseekline(buf, li);

	if (bufseek(buf, BUF_CUR, 1) == FAIL)
		if (bufseek(buf, BUF_CUR, -1) == FAIL)
			return -1;

	freelines(li, getnext(li));
	buf->dirty = 1;
	--buf->len;

	return 0;
}
