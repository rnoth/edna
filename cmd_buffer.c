/* cmd_buffer.c -- buffer manipulation commands */
#include <string.h>

#include "edna.h"
#include "cmd.h"

int
cmd_switchbuf (State *st, Buffer *buf, Arg *arg, char *error)
{
	if (!arg->mode) {
		strcpy (error, "no option specified");
		return (FAIL);

	} else if (!strcmp (arg->mode, "next")) {
		returnbuf (buf, st);
		checkoutbuf (buf, st, 0);

		return (SUCC);

	} else if (!strcmp (arg->mode, "prev")) {
		returnbuf (buf, st);
		checkoutbuf (buf, st, st->buffers.c - 1);

		return (SUCC);
	}

	strcpy (error, "unknown option");
	return FAIL;
}

int
cmd_openbuf (State *st, Buffer *buf, Arg *arg, char *error)
{
	Buffer *tmp;
	size_t i;

	if (!arg->param.c) {
		strcpy (error, "no filenames provided");
		return FAIL;
	}

	i = 0;
	do {
		tmp = makebuf (arg->param.v[i]);
		readbuf (tmp, error);
		addbuf (st, tmp);
	} while (++i < arg->param.c);

	return SUCC;
}
