/* init.c -- initlization and cleanup routines */
#include <stdlib.h>
#include <string.h>

#include "edna.h"
#include "config.h"

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
initst(State *st)
{
	size_t i;
	/* st->buffers */
	make_vector(st->buffers);

	/* st->commands */
	make_vector(st->cmds);
	for (i = 0; commands[i].name; ++i)
		;
	vec_concat(st->cmds, commands, i);

	/* st->modes */
	make_vector(st->modes);
	for (i = 0; modes[i].name; ++i)
		;
	vec_concat(st->modes, modes, i);

	setmode(st, "command");
}

State *
makestate (void)
{
	State *st;
	if (!(st = calloc (1, sizeof *st))) die ("calloc");
	return st;
}

int
parse_argv (State *st, char **argv, char *err)
{
	Buffer *tmp;


	if (*++argv) {
		while (*argv++) {
			tmp = makebuf();
			initbuf(tmp, *argv);
			readbuf(tmp, err);
			bufclean(tmp);
			addbuf(st, tmp);
		}
	} else {
		tmp = makebuf();
		initbuf(tmp, NULL);
		addbuf(st, tmp);
	}

	return SUCC;
}

