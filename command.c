/* command.c -- interface for executing commands */
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include "edna.h"
#include "cmd.h"
#include "addr.h"

static void	cleanup	(Buffer *, Arg *arg);
extern int	cmdchck (const void *a, const void *b);
extern int	cmdcmp (const void *a, const void *b);
extern int	evalcmd	(State *st, Buffer *, String *, char *error);

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
	free (arg);

	/* leave the structs in a sane state */
	if (!buf->curline)
		buf->curline = makeline ();
}

int
cmdchck (const void *a, const void *b)
{
	return strcmp (((char *)a), ((Command *)b)->name);
}

int
cmdcmp (const void *a, const void *b)
{
	return strcmp (((Command *)a)->name, ((Command *)b)->name);
}

int
evalcmd (State *st, Buffer *buf, String *str, char *error)
{
	int ret = FAIL;
	struct tokaddr *tok;
	Command *cmd;
	Arg *arg;

	if (!(arg = calloc (1, sizeof *arg))) die ("calloc");
	if (!(arg->name = calloc (20, sizeof *arg->name))) die ("calloc");

	if (parseline (str, buf, arg, error) == FAIL)
		goto finally;

	cmd = bsearch (arg->name, st->cmds.v, st->cmds.c,
			sizeof *st->cmds.v, &cmdchck);

	if (!cmd) {
		strcpy (error, "unknown command");
		ret = FAIL;
		goto finally;
	}

	if (cmd->mode) {
		arg->mode = malloc (strlen (cmd->mode) * sizeof *arg->mode);
		if (!arg->mode) die("malloc");
		strcpy (arg->mode, cmd->mode);
	}

	if (!*arg->sel.v) {
		tok = lexaddr (chartostr(cmd->defaddr));
		arg->sel = evaladdr (tok, buf, error);
		if (!*arg->sel.v) {
			strcpy (error, "invalid selection");
			goto finally;
		}
	}

	if ((*cmd->func) (st, buf, arg, error) == FAIL)
		goto finally;

	ret = SUCC;

finally:
	cleanup (buf, arg);
	return ret;

}
