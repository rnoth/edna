/* init.c -- initlization and cleanup routines */
#include <stdlib.h>

#include "edna.h"

extern State*	makestate	(void);
extern Arg*	makearg		(void);
extern void	cleanup		(State *, Arg *);

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

void
cleanup (State *st, Arg *arg)
{
	for (; --st->buflen;)
		freebuf (st->buffers[st->buflen]);
	free (arg->name);
	free (arg);
}
