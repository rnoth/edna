/* command.c -- interface for executing commands */
#define _POSIX_C_SOURCE 199309L
#include <errno.h>
#include <setjmp.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#undef _POSIX_C_SOURCE

#include "edna.h"
#include "config.h"

static int	runcmd	(State *, Buffer *, Command *, Arg *, char *err);
static Command *findcmd	(State *, char *);

Command *
findcmd(State *st, char *name)
{
	size_t i;

	if (name == NULL) return NULL;
	for (i = 0; i < st->cmds->c; ++i)
		if (!strcmp(name, st->cmds->v[i].name)) 
			return st->cmds->v + i;
	return NULL;
}

void
freearg(Arg *arg)
{
	size_t i;

	for (i = 0; i < arg->param->c; ++i)
		free(arg->param->v[i]);
	free_vector(arg->param);
	if (arg->name) free(arg->name);
	if (arg->mode) free(arg->mode);
	free_vector(arg->sel);
	free(arg);

}

Arg *
makearg(void)
{
	Arg *ret;
	ret = calloc(1, sizeof *ret);
	make_vector(ret->sel);
	return ret;
}

int
cmderror (State *st, Buffer *buf, String *s, char *err)
{
	if (!strcmp (err, "quit")) return FAIL;
	if (printf (ERROR) < 0) die ("printf");
	if (fflush (stdout) == EOF) warn ("fflush");
	return SUCC;
}

int
cmdeval(State *st, Buffer *buf, String *s, char *err)
{
	int ret;
	Arg *arg;
	Command *cmd;

	ret = FAIL;
	arg = makearg();

	if (parseline(s, buf, arg, err) == FAIL) goto finally;

	cmd = findcmd(st, arg->name);
	if (!cmd) {
		strcpy(err, "unknown command");
		goto finally;
	}


	if (runcmd(st, buf, cmd, arg, err) == SUCC) ret = SUCC;

finally:
	freearg(arg);
	return ret;

}

int
cmdprompt (State *st, Buffer *buf, String *s, char *err)
{
	if (printf (PROMPT) < 0) die ("printf");
	if (fflush (stdout) == EOF) warn ("fflush");
	return SUCC;
}

int
runcmd(State *st, Buffer *buf, Command *cmd, Arg *arg, char *err)
{
	size_t pos = 0;
	Selection *sel;
	String *addr;

	if (cmd->mode) {
		arg->mode = malloc((strlen(cmd->mode) + 1) * sizeof *arg->mode);
		if (!arg->mode) die("malloc");
		strcpy(arg->mode, cmd->mode);
	}

	if (arg->sel->c == 0 && cmd->defaddr) {
		addr = chartostr(cmd->defaddr);

		sel = getaddr(addr, &pos, buf, err);
		freestring(addr);
		if (sel == NULL) goto fail;

		free_vector(arg->sel);
		make_vector(arg->sel);
		arg->sel = vec_clone(sel);

		free_vector(sel);
		free(sel);
	}

	return cmd->func(st, buf, arg, err);

fail:
	return FAIL;
}
