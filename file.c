#include <stdio.h>
#include <stdlib.h>

#include "edna.h"

void
readfile (State *st, char *filename)
{
	char *buf;
	size_t bufsiz;
	
	st->file = fopen(filename, "r");
	if (!st->file)
		die();

	buf = malloc (sizeof *buf * LINESIZE);
	bufsiz = LINESIZE;
	while (!feof (st->file)) {
		if (-1 == getline (&buf, &bufsiz, st->file))
			continue;
		st->pos->line = putline (st->pos->line, buf, bufsiz, 1);
		++st->pos->lineno;
	}
	free (buf);

	return;
}
	