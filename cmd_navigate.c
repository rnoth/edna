/* cmd_navigate.c -- navigation commands */
#include <string.h>

#include "edna.h"
#include "cmd.h"

extern int	gotol	(State *, Buffer *, Arg *, char *);

int
gotol (State *st, Buffer *buf, Arg *arg, char *error)
{
	buf->curline = (*arg->sel.v) + arg->sel.c - 1;
	buf->lineno = getlineno (buf->curline);
	return SUCC;
}

