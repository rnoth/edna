/* cmd_insert.c -- line insertion commands */
#include <stdlib.h>
#include <string.h>

#include "edna.h"
#include "cmd.h"

extern int	insert		(State *, Buffer *, Arg *, char *);

int
insert (State *st, Buffer *buf, Arg *arg, char *error)
{
	Line *new;

	if (arg->addr && gotol(st, buf, arg, error))
		return 1;
	arg->addr = 0;

	if (arg->mode) {
		if (!strcmp (arg->mode, "insert")) {
			;
		} else if (!strcmp (arg->mode, "append")) {
			if (!buf->curline->next) {
				new = makeline();
				linklines (buf->curline, new);
			}
			++buf->lineno;
			buf->curline = buf->curline->next;
		} else if (!strcmp (arg->mode, "change")) {
			if (!delete (st, buf, arg, error))
				return 1;
		} else {
			strcpy (error, "unknown option");
			return 1;
		}
	}

	setmode (st, buf, "insert");
	return 0;
}

