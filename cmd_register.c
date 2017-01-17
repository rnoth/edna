/* cmd_register.c -- register manipulation commands */
#include <string.h>

#include "edna.h"
#include "cmd.h"

extern int	delete		(State *, Buffer *, Arg *, char *);

int
delete (State *st, Buffer *buf, Arg *arg, char *error)
{
	Line **targ, *tmp;

	targ = arg->sel.v;
	//tlen = arg->sel.c;

	if (!(*targ)->str) {
		strcpy (error, "empty selection");
		return FAIL;
	}

	buf->dirty = 1;

	tmp = (*targ)->next ? (*targ)->next : (*targ)->prev;
	if (!tmp)
		tmp = makeline ();

	freelines(*targ, (*targ)->next);

	buf->curline = tmp;
	buf->lineno  = getlineno (tmp);
	return SUCC;
}

