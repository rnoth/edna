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
main (/*int argc, char** argv*/)
{
	char *buf, *error;
	size_t bufsiz;
	Position *pos;

	MALLOC (buf, LINESIZE * sizeof *buf);
	bufsiz = LINESIZE * sizeof *buf;
	MALLOC (error, LINESIZE * sizeof *error);
	CALLOC (pos, 1, sizeof *pos);

	pos->line = makeline ();

	for(;;) {
		size_t i, cmd;
		char ch;

		cmd = 0;
		PRINTF (PROMPT);
		GETLINE (buf, bufsiz, stdin);

		for (i = 0; ch = buf[i], ch == ' ' || ch == '\t';  ++i)
			; /* nop */
		chomp (buf + i, bufsiz - i);

		for (size_t j = 1; j < LEN(commands); ++j)
			if (!strcmp (commands[j].handle, buf + i)) {
				cmd = j;
				break;
			}

		if ((*commands[cmd].func) (pos, error))
			FPRINTF(stderr, "?\n");

		if (!strcmp (error, "quit"))
			break;

		if (!pos->line)
			pos->line = makeline ();
	}

	PRINTF ("quitting\n");
	for (; pos->line->prev; pos->line = pos->line->prev)
		; /* nop */
	freelines (pos->line, NULL);
	free (buf);
	free (error);

	return 0;
}
