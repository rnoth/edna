#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edna.h"
#include "buffer.h"
#include "line.h"
#include "mode.h"
#include "str.h"

int
inserror (State *st, Buffer *buf, String *s, char *err)
{
	if (feof (stdin))
		clearerr (stdin);
	setmode (st, buf, "command");
	return SUCC;
}


int
insline (State *st, Buffer *buf, String *str, char *err)
{
	Line *new;

	if (!strcmp (str->v, ".\n"))
		return FAIL;

	new = makeline ();

	if (changeline (new, str) == FAIL) {
		strcpy (err, "insline(): changeline() failed. memory errors?");
		goto fail;
	}

	if (addline (buf, new) == FAIL) {
		strcpy (err, "insline(): addline() failed. buffer inconsistency?");
		goto fail;
	}

	return SUCC;

fail:
	freelines (new, NULL);
	return FAIL;
}
