/* command.c -- interface for executing commands */
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include "edna.h"

static void	cleanup	(Buffer *, Arg *arg);
extern int	evalcmd	(State *st, Buffer *, Arg *arg, char *error);

void
cleanup (Buffer *buf, Arg *arg)
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
	if (!buf->curline)
		buf->curline = makeline ();
	arg->addr = buf->lineno;
	arg->rel = 1;
	arg->cnt = 0;
	arg->vec = NULL;
	arg->mode = NULL;
}

int
cmdcmp (const void *a, const void *b)
{
	return strcmp (((Command *)a)->name, ((Command *)b)->name);
}

int
cmdchck (const void *a, const void *b)
{
	return strcmp (((char *)a), ((Command *)b)->name);
}

int
evalcmd (State *st, Buffer *buf, Arg *arg, char *error)
{
	int ret = SUCC;
	Command *cmd;

	/* fix arg->addr, bexause parseline can't handle absolute
	 * addresses (st->lineno isn't visible inside input.c )
	 */
	if (!arg->rel) {
		arg->addr -= buf->lineno;
		arg->rel = 1;
	}

	cmd = bsearch (arg->name, st->cmds.v, st->cmds.c,
			sizeof *st->cmds.v, &cmdchck);

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

	if ((*cmd->func) (st, buf, arg, error)) {
		ret = FAIL;
		goto finally;
	}

finally:
	cleanup (buf, arg);
	return ret;

}

void
sighandle (int signo)
{
	if (signo == SIGINT)
		return; /* TODO: longjmp */
}
