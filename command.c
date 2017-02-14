/* command.c -- interface for executing commands */
#define _POSIX_C_SOURCE 199309L
#include <errno.h>
#include <setjmp.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#undef _POSIX_C_SOURCE

#include "edna.h"
#include "addr.h"
#include "cmd.h"
#include "state.h"
#include "util.h"

static int	runcmd (State *, Buffer *, Command *, Arg *, char *err);

int
grabline(State *st, Buffer *buf, String *s, char *err)
{
	int ret = SUCC;

	errno = 0;
	if (readline(s, stdin) == FAIL) {
		if (feof(stdin))
			strcpy(err, "quit");
		else
			strcpy(err, strerror (errno));
		ret = FAIL;
		clearerr (stdin);
	}

	return ret;

}

void
freearg (Arg *arg)
{
	size_t i;

	for (i = 0; i < arg->param.c; ++i)
		free(arg->param.v[arg->cnt]);
	free_vector(arg->param);
	if (arg->name) free(arg->name);
	if (arg->mode) free(arg->mode);
	free_vector(arg->sel);
	free(arg);

}

Arg *
makearg (void)
{
	Arg *ret;
	ret = calloc(1, sizeof *ret);
	make_vector(ret->sel);
	return ret;
}


int
runcmd (State *st, Buffer *buf, Command *cmd, Arg *arg, char *err)
{
	size_t pos = 0;
	Selection *sel;
	String *addr;

	if (!cmd) {
		strcpy(err, "unknown command");
		goto fail;
	}

	if (cmd->mode) {
		arg->mode = malloc(strlen(cmd->mode) + 1 * sizeof *arg->mode);
		if (!arg->mode) die("malloc");
		strcpy(arg->mode, cmd->mode);
	}

	if (arg->sel.c == 0 && cmd->defaddr) {
		addr = chartostr(cmd->defaddr);

		sel = getaddr(addr, &pos, buf, err);
		freestring(addr);
		if (sel == NULL)
			goto fail;

		free_vector(arg->sel);
		make_vector(arg->sel);
		vec_copy(arg->sel, *sel);

		free_vector(*sel);
		free(sel);
	}

	return cmd->func(st, buf, arg, err);

fail:
	return FAIL;
}

int
cmdchck (const void *a, const void *b)
{
	return strcmp(((char *)a), ((Command *)b)->name);
}

int
cmdcmp (const void *a, const void *b)
{
	return strcmp (((Command *)a)->name, ((Command *)b)->name);
}

int
cmdeval (State *st, Buffer *buf, String *s, char *err)
{
	int ret;
	Arg *arg;
	Command *cmd;

	ret = FAIL;
	arg = makearg();

	if (parseline(s, buf, arg, err) == FAIL) goto finally;

	cmd = bsearch(arg->name, st->cmds.v, st->cmds.c,
			sizeof *st->cmds.v, &cmdchck);

	if (runcmd(st, buf, cmd, arg, err) == SUCC) ret = SUCC;

finally:
	freearg(arg);
	return ret;

}

