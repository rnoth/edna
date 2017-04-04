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

//static int	runcmd	(State *, Buffer *, Command *, Arg *, char *err);

Command *
findcmd(State *st, char *name)
{
	size_t i;

	if (!name) return NULL;

	for (i = 0; i < st->cmds->c; ++i) {
		if (strlen(name) != strlen(arr(st->cmds)[i].name)) continue;
		if (!strcmp(name, st->cmds->v[i].name))
			return st->cmds->v + i;
	}
	return NULL;
}

void
freearg(Arg *arg)
{
	if (arg->param) {
		mapv(void **each, arg->param) free(*each);
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
	Arg *ret;

	ret = calloc(1, sizeof *ret);
	if (!ret) return NULL;
	make_vector(ret->param);
	if (!ret) { free(ret); return NULL; }

	return ret;
}

int
cmderror(State *st, Buffer *buf, String *s, char *err)
{
	if (!strcmp(err, "quit")) return -1;
	else if (!strcmp(err, "eof")) {
		if (printf("\r") == -1) return errno;
		if (fflush(stdout) == -1) return errno;
		if (isdirty(buf)) {
			strcpy(err, "buffer has unsaved changes");
			return 0;
		} else  {
			return -1;
		}
	}
	if (printf(ERROR) < 0) return errno;
	if (fflush(stdout) == EOF) return errno;
	return 0;
}

int
cmdeval(State *st, Buffer *buf, String *s, char *errmsg)
{
	int err = 0;
	Arg *arg = 0;
	//Command *cmd = 0;
	Expr *ex = 0;

	arg = makearg();
	if (!arg) return ENOMEM;

	ex = expr_ctor();
	if (!ex) goto finally;

	err = parseline(ex, st, s);
	if (err == EILSEQ) {
		strcpy(errmsg, "parse error");
		goto finally;
	} else if (err) goto finally;

	err = evalexpr(NULL, ex, st, buf, arg, errmsg);
	if (err) goto finally;

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

#if 0
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
		str_free(addr);
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
#endif
