#include <stdlib.h>
#include <string.h>

#include "edna.h"

Buffer *
clonebuf(Buffer *src)
{
	Buffer *ret;

	ret = makebuf();
	memcpy(ret, src, sizeof *ret);

	return ret;
}

void
freebuf(Buffer *buf)
{
	freelines(buf->top, NULL);
	if (buf->file) fclose(buf->file);
	freestring(buf->name);
	free(buf);

	return;
}

int
initbuf(Buffer *buf, char *fn)
{
	if (fn) bufsetname (buf, fn);
	
	buf->cur = buf->top = buf->bot = makeline();
	buf->len = 1;

	return SUCC;
}


void
killbuf(Buffer *buf)
{
	freebuf(buf);
	memset(buf, 0, sizeof *buf);
	return;
}

Buffer *
makebuf(void)
{
	Buffer *ret;
	
	ret = calloc(1, sizeof *ret);
	if (!ret) die("calloc");

	return ret;
}
