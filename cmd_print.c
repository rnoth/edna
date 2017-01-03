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
	if (0 > printf ("%s\n", error))
		die("printf");
	return 0;
}

int
print (State *st, Buffer *buf, Arg *arg, char *error)
{
	if (arg->addr && gotol (st, buf, arg, error))
		return 1;

	if (!buf->curline->str) {
		strcpy (error, "empty buffer");
		return 1;
	}

	if (0 > printf ("%ld\t%s", buf->lineno, buf->curline->str))
		die("printf");
	return 0;
}
