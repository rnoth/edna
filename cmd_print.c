/* cmd_print.c -- printing commands */
#include <stdio.h>
#include <string.h>

#include "edna.h"
#include "cmd.h"
#include "util.h"

extern int	cmd_help	(State *, Buffer *, Arg *, char *);

int
cmd_help (State *st, Buffer *buf, Arg *arg, char *error)
{
	if (error[0])
		if (printf ("%s\n", error) < 0) die("printf");
	return SUCC;
}

