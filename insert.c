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
	if (printf("\r") == -1) return errno;
	setmode(st, "command");
	return 0;
}


int
insline(State *st, Buffer *buf, String *str, char *errmsg)
{
	int err;
	Line *new;

	if (!strcmp(str->v, ".\n")) return -1;

	new = makeline();
	if (!new) return ENOMEM;

	err = changeline(new, str);
	if (err) goto fail;

	err = addline(buf, new);
	if (err) goto fail;

	return 0;
fail:
	freelines(new, NULL);
	return err;
}

int
insprompt (State *st, Buffer *buf, String *s, char *err)
{
	if (printf(INS_PROMPT) < 0) return errno;
	if (fflush(stdout) == EOF) return errno;
	return 0;
}
