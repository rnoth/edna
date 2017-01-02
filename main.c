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
	/* FIXME: line isn't a Line, len isn't descriptive */
	char *line, *error;
	size_t len;
	State *st;
	Arg *arg;

	/* init stuff */
	if (!(line = malloc (LINESIZE * sizeof *line)))
		die ("malloc");
	len = LINESIZE;

	if (!(error = malloc (LINESIZE * sizeof *error)))
		die ("malloc");
	strcpy (error, "everything is ok");

	if (!(st = calloc (1, sizeof *st)))
		die ("calloc");
	if (!(st->buffers = malloc (sizeof *st->buffers)))
		die ("malloc");
	if (!(st->curbuf = malloc (sizeof *st->curbuf)))
		die ("malloc");
	st->buffers[0] = st->curbuf;
	if (!(st->curbuf->filename = malloc(LINESIZE * sizeof *st->curbuf->filename)))
		die ("malloc");
	strcpy (st->curbuf->filename, FILENAME);

	if (!(arg = calloc (1, sizeof *arg)))
		die ("calloc");
	if (!(arg->name = calloc (LINESIZE, sizeof *arg->name)))
		die ("calloc");

	st->curbuf->curline = makeline ();
	st->curbuf->lineno = 0;
	/* end init */

	/* parse argv */
	if (argc > 1) {
		strcpy (st->curbuf->filename, argv[1]);
		readbuf (st->curbuf);
	}

	/* main execution */
	for (;;) {
		size_t cmd;

		cmd = 0;
		arg->addr = st->curbuf->lineno;
		arg->rel = 0;
		arg->cnt = 0;
		arg->vec = NULL;
		arg->mode = NULL;

		readline (&line, &len, PROMPT);
		parseline (line, len, arg);

		/* fix arg->addr, bexause parseline can't handle absolute
		 * addresses (st->lineno isn't visible outside main() )
		 */
		if (!arg->rel) {
			arg->addr -= st->curbuf->lineno;
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

		if ((*commands[cmd].func) (st, st->curbuf, arg, error))
			if (0 > fprintf(stderr, ERROR))
				die("fprintf");

		if (!strcmp (error, "quit"))
			break;

		if (!st->curbuf->curline)
			st->curbuf->curline = makeline ();
		if (arg->mode)
			free (arg->mode);
		if (arg->cnt) {
			for (;arg->cnt--;)
				free (arg->vec[arg->cnt]);
			free (arg->vec);
		}
	}

	/* TODO: this is getting ridiculous. put in in it's own function */
	for (;st->curbuf->curline->prev;)
		st->curbuf->curline = st->curbuf->curline->prev;

	/* FIXME: doesn't free all buffers if there's more */
	if (st->curbuf->file)
		fclose (st->curbuf->file);	
	freelines (st->curbuf->curline, NULL);
	free (st->curbuf->filename);
	free (st->curbuf);
	free (st->buffers);
	free (st);

	free (line);
	free (error);

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
