/* cmd_register.c -- register manipulation commands */
#include <string.h>

#include "edna.h"
#include "cmd.h"

extern int	delete		(State *, Buffer *, Arg *, char *);

int
delete (State *st, Buffer *buf, Arg *arg, char *error)
{
	Line *tmp;

	if (!buf->curline->str) {
		strcpy (error, "empty buffer");
		return 1;
	}

	if (arg->addr)
		if (gotol (st, buf, arg, error))
			return 1;

	buf->dirty = 1;

	tmp = buf->curline->next ? buf->curline->next : buf->curline->prev;
	if (!tmp)
		tmp = makeline ();

	if (!buf->curline->next)
		--buf->lineno;
	freelines(buf->curline, buf->curline->next);

	buf->curline = tmp;
	return 0;
}

