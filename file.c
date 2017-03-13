/* file.c -- file manipulation functions */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edna.h"

int
readbuf(Buffer *buf, char *errmsg)
{
	int err;
	String *s;
	Line *new;
	FILE *f;

	errno = 0;
	if (bufopen(buf, "r")) return -1;

	s = makestring();
	f = buf->file;

	errno = 0;
	while (!feof(f)) {
		err = readline(s, f);
		if (err == -1) break;
		else if (err > 0) {
			freestring(s);
			return err;
		}

		new = makeline();
		if (!new) return ENOMEM;
		changeline(new, s);
		addline(buf, new);
	}
	clearerr(f);

	freestring(s);
	return 0;
}

int
writebuf (Buffer *buf, char *errmsg)
{
	int err;
	FILE *f;
	Line *tmp;

	errno = 0;
	err = bufopen(buf, "w+");
	if (err) return err;
		
	f = buf->file;
	for (tmp = bufprobe(buf, 1); tmp; tmp = getnext(tmp))
		fputs(tmp->str->v, f);

	return 0;
}
