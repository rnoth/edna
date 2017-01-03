/* cmd_misc.c -- other commands */
#include <string.h>

#include "edna.h"
#include "cmd.h"

extern int	nop	 (State *st, Buffer *buf, Arg *arg, char *error);

int
nop (State *st, Buffer *buf, Arg *arg, char *error)
{
	strcpy (error, "unknown command");
	return 1;
}

