#include <stdio.h>
#include <stdlib.h>

#include "edna.h"

extern void readfile	(State *, char *);
extern void writefile	(State *);

void
readfile (State *st, char *filename)
{
	char *buf;
	size_t bufsiz;

	st->file = fopen(filename, "r+");
	if (!st->file)
		die("fopen");

	buf = malloc (sizeof *buf * LINESIZE);
	bufsiz = LINESIZE;
	while (!feof (st->file)) {
		if (-1 == getline (&buf, &bufsiz, st->file))
			continue;
		st->curline = putline (st->curline, buf, bufsiz, 1);
		++st->lineno;
	}
	free (buf);

	return;
}

void
writefile (State *st)
{
	rewind (st->file);
	do {
		fputs (st->curline->str, st->file);
		st->curline = st->curline->next;
	} while (st->curline);
	return;
}
