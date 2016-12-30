/* edna.c -- ed-like text editor */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <regex.h>

#include "edna.h"
#include "config.h"

int
main (int argc, char** argv)
{
	char *buf, *error, *name;
	size_t bufsiz;
	State *st;
	Arg *arg;

	/* init stuff */
	if (!(buf = malloc (LINESIZE * sizeof *buf)))
		die ("malloc");
	bufsiz = LINESIZE;

	if (!(error = malloc (LINESIZE * sizeof *error)))
		die ("malloc");
	strcpy (error, "everything is ok");

	if (!(name = calloc (LINESIZE, sizeof *name)))
		die ("calloc");

	if (!(st = calloc (1, sizeof *st)))
		die ("calloc");

	if (!(arg = calloc (1, sizeof *arg)))
		die ("calloc");

	st->curline = makeline ();
	st->lineno = 0;
	/* end init */

	/* parse argv */
	if (argc > 1)
		readfile (st, argv[1]);

	/* main execution */
	for (;;) {
		size_t cmd;

		cmd = 0;
		arg->addr = 0;
		arg->rel = 1;
		name[0] = 0;

		readline (&buf, &bufsiz, PROMPT);
		parseline (buf, name, arg);

		/* fix arg->addr, bexause parseline can't handle absolute
		 * addresses (st->lineno isn't visible outside main()
		 */
		if (!arg->rel) {
			arg->addr -= st->lineno;
			arg->rel = 1;
		}

		for (size_t j = 1; j < LEN(commands); ++j)
			if (!strcmp (name, commands[j].name)) {
				cmd = j;
				break;
			}

		if (commands[cmd].mode) {
			if (!(arg->mode = malloc (sizeof *arg->mode)))
				die("malloc");
			strcpy (arg->mode, commands[cmd].mode);
		}

		if ((*commands[cmd].func) (st, arg, error))
			if (0 > fprintf(stderr, ERROR))
				die("fprintf");

		if (!strcmp (error, "quit"))
			break;

		if (!st->curline)
			st->curline = makeline ();
		if (arg->mode) {
			free (arg->mode);
			arg->mode = NULL;
		}
	}

	for (;st->curline->prev;)
		st->curline = st->curline->prev;

	if (st->file)
		fclose (st->file);	
	freelines (st->curline, NULL);
	free (buf);
	free (error);
	free (st);
	free (name);

	return 0;
}
