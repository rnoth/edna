#include <string.h>
#include "edna.h"

int
cmd_delete (State *st, Buffer *buf, Arg *arg, char *error)
{
	Line *targ;

	targ = *arg->sel->v;

	if (rmline(buf, targ)) {
		strcpy (error, "empty selection");
		return -1;
	}

	return 0;
}

