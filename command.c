/* command.c -- interface for executing commands */
#include <stdlib.h>
#include <string.h>

#include "edna.h"

extern int	evalcmd	(State *st, Arg *arg, char *error);
static void	cleanup	(State *st, Arg *arg);

int
evalcmd (State *st, Arg *arg, char *error)
{
	int ret = SUCC;
	Command *cmd;

	/* fix arg->addr, bexause parseline can't handle absolute
	 * addresses (st->lineno isn't visible inside input.c )
	 */
	if (!arg->rel) {
		arg->addr -= st->curbuf->lineno;
		arg->rel = 1;
	}

	cmd = bsearch (arg->name, st->commands, st->cmdlen,
			sizeof *st->commands, &cmdchck);

	if (!cmd) {
		strcpy (error, "unknown command");
		ret = FAIL;
		goto finally;
	}

	if (cmd->mode) {
		arg->mode = malloc (sizeof *arg->mode);
		if (!arg->mode) die("malloc");
		strcpy (arg->mode, cmd->mode);
	}

	if ((*cmd->func) (st, st->curbuf, arg, error)) {
		ret = FAIL;
		goto finally;
	}

finally:
	cleanup (st, arg);
	return ret;

}

void
cleanup (State *st, Arg *arg)
{
	/* free resources */
	if (arg->mode)
		free (arg->mode);
	if (arg->cnt) {
		for (;arg->cnt--;)
			free (arg->vec[arg->cnt]);
		free (arg->vec);
	}

	/* leave the structs in a sane state */
	if (!st->curbuf->curline)
		st->curbuf->curline = makeline ();
	arg->addr = st->curbuf->lineno;
	arg->rel = 0;
	arg->cnt = 0;
	arg->vec = NULL;
	arg->mode = NULL;
}
