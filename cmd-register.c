#include <string.h>
#include "edna.h"

int
cmd_delete (State *st, Buffer *buf, Arg *arg, char *error)
{
	Line *targ;

	targ = *arg->sel.v;

	if (rmline (buf, targ) == FAIL) {
		strcpy (error, "empty selection");
		return FAIL;
	}

	return SUCC;
}

