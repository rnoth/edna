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
insline (State *st, Buffer *buf, String *str, char *err)
{
	Line *new;

	if (!strcmp (str->v, ".\n"))
		return (FAIL);

	new = makeline ();

	if (FAIL == changeline (new, str)) {
		strcpy (err, "insline(): changeline() failed. memory errors?");
		goto fail;
	}

	if (FAIL == addline (buf, new, buf->lineno)) {
		strcpy (err, "insline(): addline() failed. buffer inconsistency?");
		goto fail;
	}

	setcurline (buf, new);
	return (SUCC);

fail:
	freelines (new, NULL);
	return (FAIL);
}
