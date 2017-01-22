#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edna.h"


extern int inserror (State *st, Buffer *buf, char *error);
extern int insline (State *st, Buffer *buf, String *str, char *error);

int
inserror (State *st, Buffer *buf, char *error)
{
	if (feof (buf->file))
		clearerr (buf->file);
	setmode (st, buf, "command");
	return SUCC;
}

int
insline (State *st, Buffer *buf, String *str, char *error)
{
	Line *tmp, *new;

	if (!strcmp (str->v, ".\n"))
		return FAIL;

	if (buf->curline == buf->top) {
		new = makeline();
		linklines (new, buf->top);
		buf->curline = new;
	}

	if (!(tmp = putline (buf->curline, str->v, str->c))) {
		strcpy (error, "insertion failed");
		return FAIL;
	}

	buf->curline = tmp;

	++buf->lineno;
	++buf->len;
	return SUCC;
}
