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
	str_free(buf->name);
	free(buf);

	return;
}

int
initbuf(Buffer *buf, char *fn)
{
	if (fn) bufsetname (buf, fn);
	
	buf->cur = buf->top = buf->bot = makeline();
	buf->len = 1;

	return 0;
}


void
killbuf(Buffer *buf)
{
	memset(buf, 0, sizeof *buf);
	freebuf(buf);
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
