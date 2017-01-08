/* edna -- ed-like text editor */
#include <regex.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edna.h"
#include "config.h"
#include "cmd.h"

int
main (int argc, char** argv)
{
	char *line, *error;
	size_t len;	/* size of s */
	State *st;
	Arg *arg;

	/* init stuff */
	if (!(line = malloc (LINESIZE * sizeof *line))) die ("malloc");
	len = LINESIZE;

	if (!(error = malloc (LINESIZE * sizeof *error))) die ("malloc");
	error[0] = 0;

	st = makestate();
	arg = makearg();

	initst (st);
	/* end init */

	/* parse argv */
	if (argc > 1) {
		do {
			st->curbuf = makebuf (*(++argv));
			readbuf (st->curbuf, error);
			addbuf (st, st->curbuf);
			++st->bufno;
		} while (--argc > 1);
	} else {
		st->curbuf = makebuf (FILENAME);
		readbuf (st->curbuf, error);
		addbuf (st, st->curbuf);
	}
	arg->addr = st->curbuf->lineno;
	/* end parse */

	/* main execution */
	for (;;) {

		if (printf (PROMPT) < 0) die ("printf");
		readline (&line, &len, stdin, error);
		parseline (line, len, arg);
		if (!evalcmd (st, arg, error))
			goto finally;

		if (0) {
		finally:
			if (!strcmp (error, "quit"))
				break;
			if (printf (ERROR) < 0) die ("printf");
		}

	}
	/* end main */

	free (error);
	free (line);
	freestate (st);
	freearg (arg);

	return 0;
}
