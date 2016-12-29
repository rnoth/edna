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
	char *buf, *error, *name;
	size_t bufsiz;
	Position *pos;
	Arg *arg;

	MALLOC (buf, LINESIZE * sizeof *buf);
	MALLOC (error, LINESIZE * sizeof *error);
	strcpy (error, "everything is ok");
	CALLOC (name, LINESIZE, sizeof *name);
	bufsiz = LINESIZE * sizeof *buf;
	CALLOC (pos, 1, sizeof *pos);
	MALLOC (arg, sizeof *arg);

	pos->line = makeline ();

	for(;;) {
		size_t cmd;
		char ch;

		cmd = 0;
		arg->addr = 0;
		PRINTF (PROMPT);
		GETLINE (buf, bufsiz, stdin);

		for (size_t i = 0; (ch = buf[i]) ;++i) {
			char relative = 0, tmp[64];
			int j = 0;
			switch (buf[i]) {
			case ' ':
			case '\t':
			case '\n':
				break;
			case '+':
				relative = 1;
				goto num;
				break;
			case '-':
				relative = 1;
				++i;
				goto num;
				break;
			default:
				if (ch >= '0' && ch <= '9')
					goto num;
				goto cmd;
				break;
			num:
				j = 0;
				if (relative) {
					tmp[0] = '-';
					++j;
				}
				for (; ch = buf[i], ch >= '0' && ch <= '9'; ++i, ++j)
					tmp[j] = ch;
				tmp[j] = 0;
				if (relative) {
					arg->addr = strtol (tmp, NULL, 10);
				} else
					arg->addr = strtol (tmp, NULL, 10) - pos->lineno;
				--i; /* push back last read char */
				break;
			cmd:
				for (j = 0; (ch = buf[i]) != '\n'; ++i, ++j) {
					name[j] = ch;
				}
				name[j] = 0;
				--i; /* push back last read char */
				break;
			}
		}

		for (size_t j = 1; j < LEN(commands); ++j)
			if (!strcmp (commands[j].handle, name)) {
				cmd = j;
				break;
			}

		if ((*commands[cmd].func) (pos, arg, error))
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
