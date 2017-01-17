/* file.c -- file manipulation functions */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edna.h"

extern int readbuf	(Buffer *, char *);
extern int writebuf	(Buffer *, char *);

int
readbuf (Buffer *buf, char *error)
{
	String s;

	buf->file = fopen (buf->filename, "r+");
	if (!buf->file ) {
		warn ("fopen");
		return FAIL;
	}

	s = makestring (LINESIZE);

	/* TODO: no actual error handling */
	while (!feof (buf->file)) {
		if (!readline (&s, buf->file, error))
			continue; /* usually means eof, go check */
		buf->curline = putline (buf->curline, s.v, s.c);
		++buf->lineno;
		++buf->len;
	}
	clearerr (buf->file);

	freestring (s);
	return SUCC;
}

int
writebuf (Buffer *buf, char *error)
{
	Line *tmp;

	if (!buf->filename[0]) {
		strcpy (error, "invalid filename");
		return FAIL;
	}
	if (!buf->curline->str) {
		strcpy (error, "empty buffer");
		return FAIL;
	}
	if (!(buf->file = freopen (buf->filename, "w+", buf->file))) {
		strcpy (error, "fopen: ");
		strcpy (error + strlen (error), strerror (errno));
		return FAIL;
	}

	for (tmp = buf->top; tmp; tmp = tmp->next)
		fputs (tmp->str, buf->file);

	return (SUCC);
}
