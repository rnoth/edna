/* cmd_print.c -- printing commands */
#include <stdio.h>
#include <string.h>

#include "edna.h"
#include "cmd.h"

extern int	print	(State *, Buffer *, Arg *, char *);
extern int	help	(State *, Buffer *, Arg *, char *);

int
help (State *st, Buffer *buf, Arg *arg, char *error)
{
	if (error[0])
		if (printf ("%s\n", error) < 0) die("printf");
	return SUCC;
}

int
print (State *st, Buffer *buf, Arg *arg, char *error)
{
	if (arg->addr && (gotol (st, buf, arg, error) == FAIL))
		return FAIL;

	if (!buf->curline->str) {
		strcpy (error, "empty buffer");
		return FAIL;
	}

	if (printf ("%ld\t%s", buf->lineno, buf->curline->str) < 0) die("printf");
	return SUCC;
}
