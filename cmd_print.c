/* cmd_print.c -- printing commands */
#include <stdio.h>
#include <string.h>

#include "edna.h"
#include "cmd.h"

extern int	help	(State *, Buffer *, Arg *, char *);

int
help (State *st, Buffer *buf, Arg *arg, char *error)
{
	if (error[0])
		if (printf ("%s\n", error) < 0) die("printf");
	return SUCC;
}

