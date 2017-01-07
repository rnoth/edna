/* init.c -- initlization and cleanup routines */
#include <stdlib.h>

#include "edna.h"

extern void	freearg		(Arg *);
extern void	freestate	(State *);
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

