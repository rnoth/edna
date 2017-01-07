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
	Command *cmd;

	/* init stuff */
	if (!(line = malloc (LINESIZE * sizeof *line))) die ("malloc");
	len = LINESIZE;

	if (!(error = malloc (LINESIZE * sizeof *error))) die ("malloc");
	strcpy (error, "everything is ok");

	st = makestate();
	arg = makearg();

	qsort (commands, LEN (commands), sizeof *commands, &cmdcmp);
	/* end init */

	/* parse argv */
	if (argc > 1) {
		do {
			st->curbuf = makebuf (*(++argv));
			readbuf (st->curbuf);
			addbuf (st, st->curbuf);
			++st->bufno;
		} while (--argc > 1);
	} else {
		st->curbuf = makebuf (FILENAME);
		readbuf (st->curbuf);
		addbuf (st, st->curbuf);
	}

	/* main execution */
	for (;;) {

		cmd = NULL;
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

		cmd = bsearch (arg->name, commands, LEN (commands),
				sizeof *commands, &cmdchck);

		if (!cmd) {
			strcpy (error, "unknown command");
			if (printf(ERROR) < 0) die("printf");
			continue;
		}

		if (cmd->mode) {
			arg->mode = malloc (sizeof *arg->mode);
			if (!arg->mode) die("malloc");
			strcpy (arg->mode, cmd->mode);
		}

		if ((*cmd->func) (st, st->curbuf, arg, error))
			if (printf(ERROR) < 0) die("printf");

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

	free (error);
	free (line);
	freestate (st);
	freearg (arg);

	return 0;
}
