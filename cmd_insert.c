/* cmd_insert.c -- line insertion commands */
#include <stdlib.h>
#include <string.h>

#include "edna.h"
#include "buf.h"
#include "cmd.h"
#include "mode.h"

int
cmd_insert (State *st, Buffer *buf, Arg *arg, char *error)
{
	buf->curline = (*arg->sel.v) + arg->sel.c - 1;

	if (arg->mode) {
		if (!strcmp (arg->mode, "insert")) {
			--buf->lineno;
			buf->curline = getprev (buf->curline);
		} else if (!strcmp (arg->mode, "append")) {
			;
		} else if (!strcmp (arg->mode, "change")) {
			if (cmd_delete (st, buf, arg, error) == FAIL)
				return FAIL;
		} else {
			strcpy (error, "unknown option");
			return SUCC;
		}
	}

	setmode (st, buf, "insert");
	return SUCC;
}
