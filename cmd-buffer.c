/* cmd_buffer.c -- buffer manipulation commands */
#include <string.h>

#include "edna.h"
#include "buffer.h"
#include "cmd.h"
#include "state.h"

int
cmd_openbuf(State *st, Buffer buf, Arg *arg, char *error)
{
	Buffer tmp;
	size_t i;

	if (!arg->param.c) {
		strcpy(error, "no filenames provided");
		return FAIL;
	}

	i = 0;
	do {
		tmp = makebuf();
		initbuf(tmp, arg->param.v[i]);
		readbuf(tmp, error);
		bufclean(buf);
		addbuf(st, tmp);
	} while (++i < arg->param.c);

	return SUCC;
}

int
cmd_switchbuf(State *st, Buffer buf, Arg *arg, char *error)
{
	if (!arg->mode) {
		strcpy(error, "no option specified");
		return FAIL;

	} else if (!strcmp(arg->mode, "next")) {
		returnbuf(st, buf);
		checkoutbuf(buf, st, 0);

		return SUCC;

	} else if (!strcmp(arg->mode, "prev")) {
		returnbuf(st, buf);
		checkoutbuf(buf, st, (st->buffers.c != 1) ? st->buffers.c - 2 : 0 );

		return SUCC;
	}

	strcpy (error, "unknown option");
	return FAIL;
}
