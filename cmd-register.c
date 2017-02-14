/* cmd_register.c -- register manipulation commands */
#include <string.h>

#include "edna.h"
#include "buffer.h"
#include "cmd.h"

int
cmd_delete (State *st, Buffer buf, Arg *arg, char *error)
{
	Line *targ;

	targ = *arg->sel.v;

	if (rmline (buf, targ) == FAIL) {
		strcpy (error, "empty selection");
		return FAIL;
	}

	return SUCC;
}

