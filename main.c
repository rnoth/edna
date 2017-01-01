/* edna -- ed-like text editor */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <regex.h>

#include "edna.h"
#include "cmd.h"
#include "config.h"

int
main (int argc, char** argv)
{
	char *buf, *error;
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

	if (!(st = calloc (1, sizeof *st)))
		die ("calloc");
	if (!(st->filename = malloc(sizeof *st->filename)))
		die ("malloc");
	strcpy (st->filename, FILENAME);

	if (!(arg = calloc (1, sizeof *arg)))
		die ("calloc");
	if (!(arg->name = calloc (LINESIZE, sizeof *arg->name)))
		die ("calloc");

	st->curline = makeline ();
	st->lineno = 0;
	/* end init */

	/* parse argv */
	if (argc > 1) {
		strcpy (st->filename, argv[1]);
		readfile (st);
	}

	/* main execution */
	for (;;) {
		size_t cmd;

		cmd = 0;
		arg->addr = st->lineno;
		arg->rel = 0;
		arg->cnt = 0;
		arg->vec = NULL;
		arg->mode = NULL;

		readline (&buf, &bufsiz, PROMPT);
		parseline (buf, bufsiz, arg);

		/* fix arg->addr, bexause parseline can't handle absolute
		 * addresses (st->lineno isn't visible outside main() )
		 */
		if (!arg->rel) {
			arg->addr -= st->lineno;
			arg->rel = 1;
		}

		for (size_t j = 1; j < LEN(commands); ++j)
			if (!strcmp (arg->name, commands[j].name)) {
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
		if (arg->mode)
			free (arg->mode);
		if (arg->cnt) {
			for (;arg->cnt--;)
				free (arg->vec[arg->cnt]);
			free (arg->vec);
		}
	}

	for (;st->curline->prev;)
		st->curline = st->curline->prev;

	if (st->file)
		fclose (st->file);	
	freelines (st->curline, NULL);
	free (buf);
	free (error);
	free (st->filename);
	free (st);
	free (arg);
	free (arg->name);
	if (arg->mode)
		free (arg->mode);
	if (arg->cnt) {
		for (;arg->cnt;--arg->cnt)
			free (arg->vec[arg->cnt]);
		free (arg->vec);
	}

	return 0;
}
