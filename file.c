/* file.c -- file manipulation functions */
#include <stdio.h>
#include <stdlib.h>

#include "edna.h"

extern void readbuf	(Buffer *);
extern void writebuf	(Buffer *);

void
readbuf (Buffer *buf)
{
	char *s;
	size_t i;

	buf->file = fopen (buf->filename, "r+");
	if (!buf->file) {
		warn ("fopen");
		return;
	}

	if (!(s = malloc (sizeof *s * LINESIZE)))
		die ("malloc");
	i = LINESIZE;

	while (!feof (buf->file)) {
		if (-1 == getline (&s, &i, buf->file))
			continue; /* usually means eof, go check */
		buf->curline = putline (buf->curline, s, i, 1);
		++buf->lineno;
	}

	free (s);
	return;
}

void
writebuf (Buffer *buf)
{
	if (!buf->file) {
	/* FIXME: nested ifs tend to imply bad design. Can this be rework? */
		if (!buf->filename[0])
			return;
		if (!(buf->file = fopen (buf->filename, "w+")))
			warn ("fopen");
	} else if (!buf->curline->str)
		return;
	rewind (buf->file);
	do {
		fputs (buf->curline->str, buf->file);
		buf->curline = buf->curline->next;
	} while (buf->curline);
	return;
}
