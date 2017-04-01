/* init.c -- initlization and cleanup routines */
#include <stdlib.h>
#include <string.h>

#include "edna.h"
#include "config.h"

void
freestate(State *st)
{
	mapv(st->buffers, freebuf(*each));
	vec_free(st->buffers);
	vec_free(st->cmds);
	vec_free(st->modes);
	free(st);
}

int
initst(State *st)
{
	size_t len;

	make_vector(st->buffers);
	if (!st->buffers) return ENOMEM;

	make_vector(st->cmds);
	if (!st->cmds) goto nomem;
	len = sizeof_commands();
	vec_concat(st->cmds, commands, len);

	make_vector(st->modes);
	if (!st->modes) goto nomem;
	len = sizeof_modes();
	vec_concat(st->modes, modes, len);

	setmode(st, "command");

	st->running = 1;

	return 0;

nomem:
	vec_free(st->buffers);
	vec_free(st->cmds);
	vec_free(st->modes);
	return ENOMEM;
}

State *
makestate(void)
{
	State *st = 0;

	st = calloc(1, sizeof *st);

	return st;
}

int
parse_argv(State *st, char **argv, char *errmsg)
{
	int err = 0;
	Buffer *tmp = 0;

	if (argv[1]) while (*++argv) {
		tmp = makebuf();
		if (!tmp) return ENOMEM;

		err = initbuf(tmp, *argv);
		if (err) return err;
		
		err = readbuf(tmp, errmsg);
		if (err) return err;

		bufclean(tmp);

		err = addbuf(st, tmp);
		if (err) return err;

	} else {
		tmp = makebuf();
		if (!tmp) return ENOMEM;

		err = initbuf(tmp, NULL);
		if (err) return err;

		err = addbuf(st, tmp);
		if (err) return err;
	}

	return 0;
}

int
quit(State *st)
{
	st->running = 0;
	return 0;
}
