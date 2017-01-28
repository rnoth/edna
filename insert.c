#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edna.h"

int
inserror (State *st, Buffer *buf, char *err)
{
	if (feof (buf->file))
		clearerr (buf->file);
	setmode (st, buf, "command");
	return (SUCC);
}

int
insparse (String *s, void *v, Buffer *buf, char *err)
{
	String **ret;

	ret = (String **)v;
	if (!strcmp (s->v, ".\n"))
		return (FAIL);
	*ret = s;
	return (SUCC);
}

int
insline (State *st, Buffer *buf, String *str, char *err)
{
	Line *tmp, *new;

	if (buf->curline == buf->top) {
		new = makeline();
		linklines (new, buf->top);
		buf->curline = new;
	}

	if (!(tmp = putline (buf->curline, str->v, str->c))) {
		strcpy (err, "insertion failed");
		return (FAIL);
	}

	buf->curline = tmp;

	++buf->lineno;
	++buf->len;
	return (SUCC);
}
