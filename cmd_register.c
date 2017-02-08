/* cmd_register.c -- register manipulation commands */
#include <string.h>

#include "edna.h"
#include "buf.h"
#include "cmd.h"

int
cmd_delete (State *st, Buffer *buf, Arg *arg, char *error)
{
	Line **targ, *tmp;

	targ = arg->sel.v;

	if (!(*targ)->str) {
		strcpy (error, "empty selection");
		return FAIL;
	}

	buf->dirty = 1;

	tmp = (*targ)->next ? (*targ)->next : (*targ)->prev;
	if (!tmp)
		tmp = makeline ();

	setcurline (buf, tmp);
	if (buf->top == *targ)
		buf->top = buf->curline;

	freelines (*targ, (*targ)->next);

	return SUCC;
}

