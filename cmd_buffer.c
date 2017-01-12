/* cmd_buffer.c -- buffer manipulation commands */
#include <string.h>

#include "edna.h"
#include "cmd.h"

extern int	switchbuf	(State *, Buffer *, Arg *, char *);
extern int	openbuf		(State *, Buffer *, Arg *, char *);

int
switchbuf (State *st, Buffer *buf, Arg *arg, char *error)
{
	if (!arg->mode) {
		strcpy (error, "no option specified");
		return 1;
	} else if (!strcmp (arg->mode, "next")) {
		/* TODO: this code shouldn't run
		 * into errors, but it should handle
		 * them in any case */
		returnbuf (buf, st);
		checkoutbuf (buf, st, 1);
		return 0;
	} else if (!strcmp (arg->mode, "prev")) {
		returnbuf (buf, st);
		checkoutbuf (buf, st, st->buffers.c - 1);
		return 0;
	}

	strcpy (error, "unknown option");
	return 1;
}

int
openbuf (State *st, Buffer *buf, Arg *arg, char *error)
{
	Buffer *tmp;
	size_t i = 0;

	if (!arg->cnt) {
		strcpy (error, "no filename provided");
		return 1;
	}

	do {
		tmp = makebuf (arg->vec[i]);
		readbuf (tmp, error);
		addbuf (st, tmp);
	} while (++i < arg->cnt);

	returnbuf (buf, st);
	checkoutbuf (buf, st, st->buffers.c - 1);

	return 0;
}
