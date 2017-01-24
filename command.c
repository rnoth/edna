/* command.c -- interface for executing commands */
#include <setjmp.h>
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
static void	sighandle (int);

static sigjmp_buf	jbuf;
static struct sigaction act, old;

void
cleanup (Buffer *buf, Arg *arg)
{
	/* reset signal handlers */
	if (sigaction (SIGINT, &old, NULL) == -1)
		perror ("sigaction");
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
/* TODO: simplify */
int
evalcmd (State *st, Buffer *buf, String *str, char *error)
{
	int ret = FAIL;
	void *tmp = NULL;	/* to hold the Selection* returned by getaddr() */
	size_t pos;
	String *s;		/* to hold the converted default address */
	Command *cmd;
	Arg *arg;

	if (!(arg = calloc (1, sizeof *arg))) die ("calloc");
	if (!(arg->name = calloc (20, sizeof *arg->name))) die ("calloc");

	{
		act.sa_handler = sighandle;

		if (sigaction (SIGINT, &act, &old) == -1) {
			perror ("sigaction");
			return (FAIL);
		}

		if (sigsetjmp (jbuf, 1))
			goto finally;
	}

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

	/* TODO: this could be it's own function? */
	if (!arg->sel.v) {
		s = chartostr (cmd->defaddr);
		pos = 0;
		tmp = getaddr (s, &pos, buf, error);
		if (!tmp)
			goto finally;

		arg->sel = *(Selection *)tmp;
	}

	if ((*cmd->func) (st, buf, arg, error) == FAIL)
		goto finally;

	ret = SUCC;

finally:
	cleanup (buf, arg);
	if (tmp)
		free (tmp);
	return ret;

}

void
sighandle (int signo) {
	switch (signo) {
	case SIGINT:
		siglongjmp (jbuf, 1);
		break;
	}
}
