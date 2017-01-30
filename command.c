/* command.c -- interface for executing commands */
#define _POSIX_C_SOURCE 199309L
#include <setjmp.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#undef _POSIX_C_SOURCE

#include "edna.h"
#include "cmd.h"
#include "addr.h"

static void	cleanup	(Buffer *, Arg *);
static int	runcmd (State *, Buffer *, Command *, Arg *, char *err);
static void	sighandle (int);

static sigjmp_buf jbuf;
static struct sigaction act, old;

void
cleanup (Buffer *buf, Arg *arg)
{
	size_t i;
	/* reset signal handlers */
	if (sigaction (SIGINT, &old, NULL) == -1)
		perror ("sigaction");
	/* free resources */
	if (arg->name)
		free (arg->name);
	if (arg->mode)
		free (arg->mode);
	if (arg->param.c) {
		for (i = 0;i < arg->param.c; ++i)
			free (arg->param.v[arg->cnt]);
		free_vector (arg->param);
	}
	free_vector (arg->sel);
	free (arg);

	/* leave the structs in a sane state */
	if (!buf->curline)
		buf->curline = makeline ();
}

int
runcmd (State *st, Buffer *buf, Command *cmd, Arg *arg, char *err)
{
	size_t pos = 0;
	Selection *sel;
	String *addr;
	if (!cmd) {
		strcpy (err, "unknown command");
		goto fail;
	}

	if (cmd->mode) {
		arg->mode = malloc (strlen (cmd->mode) + 1 * sizeof *arg->mode);
		if (!arg->mode) die("malloc");
		strcpy (arg->mode, cmd->mode);
	}

	if (arg->sel.c == 0 && cmd->defaddr) {
		addr = chartostr (cmd->defaddr);

		sel = getaddr (addr, &pos, buf, err);
		if (sel == NULL || sel == ERR)
			goto fail;

		make_vector (arg->sel);
		vec_copy (arg->sel, *sel);

		free_vector (*sel);
		free (sel);
	}

	return (cmd->func (st, buf, arg, err));

fail:
	return (FAIL);
}

void
sighandle (int signo) {
	switch (signo) {
	case SIGINT:
		siglongjmp (jbuf, 1);
		break;
	}
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

#if 0
int
cmdinit (State *st, Buffer *buf)
{
	act.sa_handler = sighandle;

	if (sigaction (SIGINT, &act, &old) == -1) {
		perror ("sigaction");
		return FAIL;
	}
	return (SUCC);
}
#endif

int
cmdparse (String *str, void *v, Buffer *buf, char *err)
{
	Arg **arg;

	arg = (Arg **)v;
	*arg = calloc (1, sizeof **arg);
	make_vector ((*arg)->sel);

	if (parseline (str, buf, *arg, err) == FAIL)
		goto fail;

	return (SUCC);

fail:
	free_vector ((*arg)->sel);
	free (*arg);
	return (FAIL);

}

int
cmdeval (State *st, Buffer *buf, Arg *arg, char *err)
{
	int ret = FAIL;
	Command *cmd;

	act.sa_handler = sighandle;

	if (sigaction (SIGINT, &act, &old) == -1) {
		perror ("sigaction");
		return FAIL;
	}
	if (sigsetjmp (jbuf, 1))
		goto finally;

	cmd = bsearch (arg->name, st->cmds.v, st->cmds.c,
			sizeof *st->cmds.v, &cmdchck);

	if (runcmd (st, buf, cmd, arg, err) == SUCC)
		ret = SUCC;

finally:
	cleanup (buf, arg);
	return (ret);

}

