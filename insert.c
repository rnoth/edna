#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edna.h"

int
inserror (State *st, Buffer *buf, Arg *arg, char *error)
{
	if (feof (buf->file)) {
		clearerr (buf->file);
	}
	setmode (st, buf, "command");
	return SUCC;
}

void
inshandle (int signo)
{
	if (signo == SIGINT)
		return; /* TODO: longjmp */
}

int
insline (State *st, Buffer *buf, Arg *arg, char *error)
{
	size_t len;
	Line *tmp, *new;

	if (buf->curline == buf->top) {
		new = makeline();
		linklines (new, buf->top);
		buf->curline = new;
	} else
		buf->curline = buf->curline->prev;

	len = strtol (arg->vec[1], NULL, 10);
	if (!(tmp = putline (buf->curline, arg->vec[0], len))) {
		strcpy (error, "insertion failed");
		return FAIL;
	}

	buf->curline = tmp;

	++buf->lineno;
	return SUCC;
}

int
insparse (char *line, size_t len, Arg *arg)
{
	if (!strcmp (line, ".\n"))
		return FAIL;

	if (!(arg->vec = malloc (sizeof *arg->vec))) die ("malloc");
	if (!(arg->vec[0] = calloc (strlen (line) + 1, sizeof **arg->vec)))
		die ("calloc");
	if (!(arg->vec[1] = calloc (LINESIZE, sizeof **arg->vec)))
		die ("calloc");
	sprintf (arg->vec[1], "%ld", len);

	strcpy (arg->vec[0], line);
	return SUCC;
}
 