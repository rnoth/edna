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
	killbuf(buf);
	free(buf);
	return;
}

int
initbuf(Buffer *buf, char *fn)
{
	int err;
	if (fn) {
		err = bufsetname(buf, fn);
		if (err) return err;
	}
	
	buf->cur = buf->top = buf->bot = makeline();
	buf->len = 1;

	return 0;
}


void
killbuf(Buffer *buf)
{
	freelines(buf->top, NULL);
	if (buf->file) {
		if (fflush(buf->file) == -1) abort();
		if (fclose(buf->file) == -1) abort();
	}
	edna_str_free(buf->name);
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
