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

	if (!name) return NULL;

	for (i = 0; i < st->cmds->c; ++i)
		if (!strcmp(name, st->cmds->v[i].name)) 
			return st->cmds->v + i;
	return NULL;
}

void
freearg(Arg *arg)
{
	if (arg->param) {
		mapv(arg->param, free(*each));
		vec_free(arg->param);
	}
	if (arg->name) free(arg->name);
	if (arg->mode) free(arg->mode);
	vec_free(arg->sel);
	free(arg);
}

Arg *
makearg(void)
{
	return calloc(1, sizeof(Arg));
}

int
cmderror (State *st, Buffer *buf, String *s, char *err)
{
	if (!strcmp(err, "quit")) return -1;
	if (printf(ERROR) < 0) return errno;
	if (fflush(stdout) == EOF) return errno;
	return 0;
}

int
cmdeval(State *st, Buffer *buf, String *s, char *errmsg)
{
	int err = 0;
	Arg *arg = 0;
	Command *cmd = 0;

	arg = makearg();
	if (!arg) return ENOMEM;

	err = parseline(s, buf, arg, errmsg);
	if (err) goto finally;

	cmd = findcmd(st, arg->name);
	if (!cmd) {
		strcpy(errmsg, "unknown command");
		err = ENOENT;
		goto finally;
	}

	err = runcmd(st, buf, cmd, arg, errmsg);

finally:
	freearg(arg);
	return err;
}

int
cmdprompt (State *st, Buffer *buf, String *s, char *err)
{
	if (printf (PROMPT) < 0) return errno;
	if (fflush (stdout) == EOF) return errno;
	return 0;
}

int
runcmd(State *st, Buffer *buf, Command *cmd, Arg *arg, char *err)
{
	size_t pos = 0;
	Selection *sel = 0;
	String *addr = 0;

	if (cmd->mode) {
		arg->mode = malloc((strlen(cmd->mode) + 1) * sizeof *arg->mode);
		if (!arg->mode) return ENOMEM;
		strcpy(arg->mode, cmd->mode);
	}

	if (!arg->sel && cmd->defaddr) {
		make_vector(addr);
		if (!addr) goto nomem;
		vec_concat(addr, cmd->defaddr, strlen(cmd->defaddr));

		sel = getaddr(addr, &pos, buf, err);
		freestring(addr);
		if (sel == NULL) goto nomem;

		vec_free(arg->sel);
		arg->sel = vec_clone(sel);

		free_vector(sel);
	}

	return cmd->func(st, buf, arg, err);

nomem:
	free(arg->mode);
	return ENOMEM;
}
