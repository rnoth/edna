/* init.c -- initlization and cleanup routines */
#include <stdlib.h>
#include <string.h>

#include "edna.h"
#include "cmd.h"
#include "vector.h"
#include "config.h"

extern void	freestate	(State *);
extern void	initst		(State *);
extern State*	makestate	(void);
extern int	parse_argv	(State *, char *, int, char **);

void
freestate (State *st)
{
	FREE_VECTOR (st->cmds);
	FREE_VECTOR (st->modes);
}

void
initst (State *st)
{
	/* st->buffers */
	MAKE_VECTOR (Buffer*, st->buffers, 1);

	/* st->commands */
	MAKE_VECTOR (Command, st->cmds, sizeof commands);
	if (!memcpy (st->cmds.v, commands, sizeof commands)) die ("memcpy");
	st->cmds.c = LEN (commands);
	qsort (st->cmds.v, st->cmds.c, sizeof *commands, &cmdcmp);

	/* st->modes */
	MAKE_VECTOR (Mode, st->modes, sizeof modes);
	if (!memcpy (st->modes.v, modes, sizeof modes)) die ("memcpy");
	st->modes.c = LEN (modes);
}

State *
makestate (void)
{
	State *st;
	if (!(st = calloc (1, sizeof *st)))
		die ("calloc");
	return st;
}

int
parse_argv (State *st, char *err, int argc, char **argv)
{
	Buffer *tmp;

	/* open tmpfile */
	tmp = makebuf ("/tmp/edna.hup"); /* FIXME */
	readbuf (tmp, err);
	addbuf (st, tmp);
	/* end open */

	/* parse argv */
	if (argc > 1)
		do {
			tmp = makebuf (*(++argv));
			readbuf (tmp, err);
			addbuf (st, tmp);
		} while (--argc > 1);
	/* end parse */

	return SUCC;
}

