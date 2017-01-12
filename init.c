/* init.c -- initlization and cleanup routines */
#include <stdlib.h>
#include <string.h>

#include "edna.h"
#include "vector.h"
#include "config.h"

extern void	freearg		(Arg *);
extern void	freestate	(State *);
extern void	init		(State *);
extern Arg*	makearg		(void);
extern State*	makestate	(void);

void
freestate (State *st)
{
	FREE_VECTOR (st->cmds);
	FREE_VECTOR (st->modes);
}

void
freearg (Arg *arg)
{
	free (arg->name);
	free (arg);
}

void
init (State *st)
{
	/* st->commands */
	MAKE_VECTOR (Command, st->cmds, sizeof commands);
	if (!memcpy (st->cmds.v, commands, sizeof commands)) die ("memcpy");
	st->cmds.c = LEN (commands);
	qsort (st->cmds.v, st->cmds.c, sizeof *commands, &cmdcmp);
	/* end st->commands */

	/* st->modes */
	MAKE_VECTOR (Mode, st->modes, sizeof modes);
	if (!memcpy (st->modes.v, modes, sizeof modes)) die ("memcpy");
	st->modes.c = LEN (modes);
	/* end st->modes */

	return;
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

