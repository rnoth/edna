/* init.c -- initlization and cleanup routines */
#include <stdlib.h>
#include <string.h>

#include "edna.h"
#include "config.h"

extern void	freearg		(Arg *);
extern void	freestate	(State *);
extern void	initst		(State *);
extern Arg*	makearg		(void);
extern State*	makestate	(void);

void
freestate (State *st)
{
	for (; --st->buflen;)
		freebuf (st->buffers[st->buflen]);
}

void
freearg (Arg *arg)
{
	free (arg->name);
	free (arg);
}

void
initst (State *st)
{
	size_t i;
	Command *c;
	if (!(st->commands = calloc (1, sizeof commands)))
		die ("calloc");
	st->cmdlen = LEN (commands);

	for (i = 0, c = st->commands; i < st->cmdlen; ++i, ++c) {
		size_t tmp;
		/* copy names */
		tmp = strlen (commands[i].name) + 1;
		c->name = malloc (tmp * sizeof *c->name);
		if (!c->name)
			die ("malloc");
		strcpy (c->name, commands[i].name);
		/* copy funcs */
		c->func = commands[i].func;
		/* copy modes */
		if (commands[i].mode) {
			tmp = strlen (commands[i].mode) + 1;
			c->mode = malloc (tmp * sizeof *c->name);
			if (!c->mode)
				die ("malloc");
			strcpy (c->mode, commands[i].mode);
		}
	}
	qsort (st->commands, st->cmdlen, sizeof *commands, &cmdcmp);
}

State *
makestate (void)
{
	State *st;
	if (!(st = calloc (1, sizeof *st)))
		die ("calloc");
	return st;
}

Arg *
makearg (void)
{
	Arg *arg;
	if (!(arg = calloc (1, sizeof *arg)))
		die ("calloc");
	if (!(arg->name = malloc (LINESIZE * sizeof *arg->name)))
		die ("malloc");
	return arg;
}

