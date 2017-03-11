/* init.c -- initlization and cleanup routines */
#include <stdlib.h>
#include <string.h>

#include "edna.h"
#include "config.h"

void
freestate (State *st)
{
	mapv(st->buffers, freebuf(each));
	vec_free(st->buffers);
	vec_free(st->cmds);
	vec_free(st->modes);
	free(st);
}

void
initst(State *st)
{
	size_t i;

	make_vector(st->buffers);
	if (!st->buffers) die("make_vector");

	make_vector(st->cmds);
	if (!st->cmds) die("make_vector");
	for (i = 0; commands[i].name; ++i)
		;
	vec_concat(st->cmds, commands, i * sizeof *commands);

	make_vector(st->modes);
	if (!st->modes) die("make_vector");
	for (i = 0; modes[i].name; ++i)
		;
	vec_concat(st->modes, modes, i * sizeof *modes);

	setmode(st, "command");
}

State *
makestate (void)
{
	State *st;
	st = calloc(1, sizeof *st);
	if (!st) die("calloc");
	return st;
}

int
parse_argv (State *st, char **argv, char *err)
{
	Buffer *tmp;

	if (argv[1]) while (*++argv) {
		tmp = makebuf();
		initbuf(tmp, *argv);
		readbuf(tmp, err);
		bufclean(tmp);
		addbuf(st, tmp);
	} else {
		tmp = makebuf();
		initbuf(tmp, NULL);
		addbuf(st, tmp);
	}

	return SUCC;
}

