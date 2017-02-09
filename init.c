/* init.c -- initlization and cleanup routines */
#include <stdlib.h>
#include <string.h>

#include "edna.h"
#include "buffer.h"
#include "cmd.h"
#include "vector.h"
#include "util.h"

#include "config.h"

extern void	freestate	(State *);
extern void	initst		(State *);
extern State*	makestate	(void);
extern int	parse_argv	(State *, char *, int, char **);

void
freestate (State *st)
{
	size_t i;

	for (i = 0; i < st->buffers.c; ++i)
		freebuf (st->buffers.v[i]);
	free_vector (st->buffers);
	free_vector (st->cmds);
	free_vector (st->modes);
	free (st);
}

void
initst (State *st)
{
	/* st->buffers */
	make_vector (st->buffers);

	/* st->commands */
	make_vector (st->cmds);
	vec_concat (st->cmds, commands, LEN (commands));
	qsort (st->cmds.v, st->cmds.c, sizeof *commands, &cmdcmp);

	/* st->modes */
	make_vector (st->modes);
	vec_concat (st->modes, modes, LEN (modes));
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


	if (argc > 1) {
		do {
			tmp = makebuf ();
			initbuf (tmp, *(++argv));
			readbuf (tmp, err);
			addbuf (st, tmp);
		} while (--argc > 1);
	} else {
		tmp = makebuf (); /* FIXME */
		initbuf (tmp, NULL);
		addbuf (st, tmp);
	}

	return SUCC;
}

