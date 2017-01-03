/* cmd_buffer.c -- buffer manipulation commands */
#include <string.h>

#include "edna.h"
#include "cmd.h"

extern int	switchbuf	(State *, Buffer *, Arg *, char *);

int
switchbuf (State *st, Buffer *buf, Arg *arg, char *error)
{
	if (!arg->mode) {
		strcpy (error, "no option specified");
		return 1;
	} else if (!strcmp (arg->mode, "next")) {
		if (st->buffers[st->buflen - 1] == buf) {
			strcpy (error, "at last buffer");
			return 1;
		}
		st->curbuf = st->buffers[st->bufno + 1];
		++st->bufno;
		return 0;
	} else if (!strcmp (arg->mode, "prev")) {
		if (st->buffers[0] == buf) {
			strcpy (error, "at first buffer");
			return 1;
		}
		st->curbuf = st->buffers[st->bufno - 1];
		--st->bufno;
		return 0;
	}

	strcpy (error, "unknown option");
	return 1;
}