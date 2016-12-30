/* edna.c -- ed-like text editor */
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "edna.h"
#include "config.h"

int
main (int argc, char** argv)
{
	char *buf, *error, *name;
	size_t bufsiz;
	Position *pos;
	State *st;
	Arg *arg;

	/* init stuff */

	MALLOC (buf, LINESIZE * sizeof *buf);
	bufsiz = LINESIZE * sizeof *buf;

	MALLOC (error, LINESIZE * sizeof *error);
	strcpy (error, "everything is ok");
	CALLOC (name, LINESIZE, sizeof *name);

	MALLOC (pos, sizeof *pos);
	CALLOC (st, 1, sizeof *st);
	MALLOC (arg, sizeof *arg);

	pos->line = makeline ();
	pos->lineno = 0;
	st->pos = pos;

	/* parse argv */
	if (argc > 1)
		readfile (st, argv[1]);

	for (;;) {
		size_t cmd;

		cmd = 0;
		arg->addr = 0;
		arg->rel = 1;
		name[0] = 0;

		readline (&buf, &bufsiz, PROMPT);
		parseline (buf, name, arg);

		for (size_t j = 1; j < LEN(commands); ++j)
			if (!strcmp (name, commands[j].name)) {
				cmd = j;
				break;
			}

		if ((*commands[cmd].func) (pos, arg, error))
			FPRINTF(stderr, ERROR);

		if (!strcmp (error, "quit"))
			break;

		if (!pos->line)
			pos->line = makeline ();
	}

	PRINTF ("quitting\n");

	for (; pos->line->prev; pos->line = pos->line->prev)
		; /* nop */

	if (st->file)
		fclose (st->file);	
	freelines (pos->line, NULL);
	free (buf);
	free (error);

	return 0;
}
