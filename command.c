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

static void	cleanup	(Buffer *, Arg *arg);
static void	sighandle (int);

static sigjmp_buf jbuf;
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
	free_vector (arg->sel);
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

void
sighandle (int signo) {
	switch (signo) {
	case SIGINT:
		siglongjmp (jbuf, 1);
		break;
	}
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
	size_t pos = 0;
	Command *cmd;
	Selection *sel;
	String *addr;

	act.sa_handler = sighandle;

	if (sigaction (SIGINT, &act, &old) == -1) {
		perror ("sigaction");
		return FAIL;
	}
	if (sigsetjmp (jbuf, 1))
		goto finally;

	cmd = bsearch (arg->name, st->cmds.v, st->cmds.c,
			sizeof *st->cmds.v, &cmdchck);

	if (!cmd) {
		strcpy (err, "unknown command");
		ret = FAIL;
		goto finally;
	}

	if (cmd->mode) {
		arg->mode = malloc (strlen (cmd->mode) * sizeof *arg->mode);
		if (!arg->mode) die("malloc");
		strcpy (arg->mode, cmd->mode);
	}
	if (!arg->sel.v) {
		addr = chartostr (cmd->defaddr);
		sel = getaddr (addr, &pos, buf, err);
		if (sel == NULL || sel == ERR)
			goto finally;

		vec_copy (arg->sel, *sel);
		free_vector (*sel);
		free (sel);
	}


	if (cmd->func (st, buf, arg, err) == FAIL)
		goto finally;

	ret = SUCC;

finally:
	cleanup (buf, arg);
	return ret;

}
