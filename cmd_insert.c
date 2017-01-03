/* cmd_insert.c -- line insertion commands */
#include <stdlib.h>
#include <string.h>

#include "edna.h"
#include "cmd.h"

extern int	insert		(State *, Buffer *, Arg *, char *);

int
insert (State *st, Buffer *buf, Arg *arg, char *error)
{
	char *line;
	size_t linelen;
	int option;

	option = INSERT;

	if (arg->mode) {
		if (!strcmp (arg->mode, "insert")) {
			; /* insert mode is redundant */
		} else if (!strcmp (arg->mode, "append")) {
			option = APPEND;
		} else if (!strcmp (arg->mode, "change")) {
			option = CHANGE;
		} else {
			strcpy (error, "unknown option");
			return 1;
		}
	}

	if (arg->addr && gotol(st, buf, arg, error))
		return 1;

	if (buf->lineno == 0 || option == APPEND)
		++buf->lineno;

	buf->dirty = 1;

	line = malloc (sizeof *line * LINESIZE);
	linelen = LINESIZE;
	for (;readline (&line, &linelen, "%ld\t", buf->lineno), strcmp (line, ".\n");) {
		if(!(buf->curline = putline (buf->curline, line, linelen, option))) {
			free (line);
			strcpy (error, "insertion failed");
			return 1; /* error */
		}
		++buf->lineno;
		option = APPEND;
	}
	--buf->lineno;

	free (line);
	return 0;
}

