#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edna.h"
#include "config.h"

int
inserror(State *st, Buffer *buf, String *s, char *err)
{
	if (feof(stdin)) clearerr(stdin);
	setmode(st, "command");
	return 0;
}


int
insline(State *st, Buffer *buf, String *str, char *err)
{
	Line *new;

	if (!strcmp(str->v, ".\n")) return -1;

	new = makeline();
	changeline(new, str);
	addline(buf, new);

	return 0;
}

int
insprompt (State *st, Buffer *buf, String *s, char *err)
{
	if (printf(INS_PROMPT) < 0) return errno;
	if (fflush(stdout) == EOF) return errno;
	return 0;
}
