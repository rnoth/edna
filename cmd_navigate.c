/* cmd_navigate.c -- navigation commands */
#include <string.h>

#include "edna.h"
#include "cmd.h"

extern int	gotol	(State *, Buffer *, Arg *, char *);

int
gotol (State *st, Buffer *buf, Arg *arg, char *error)
{
	Line *l;

	if (!arg->addr)
		++arg->addr;

	l = walk (buf->curline, arg->addr, error);
	if (!l)
		return FAIL;
	buf->lineno += arg->addr;
	buf->curline = l;
	return SUCC;
}

