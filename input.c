#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "edna.h"
#include "str.h"

int
grabline(State *st, Buffer buf, String *s, char *err)
{
	int ret = SUCC;

	errno = 0;
	if (readline(s, stdin) == FAIL) {
		if (feof(stdin))
			strcpy(err, "quit");
		else
			strcpy(err, strerror (errno));
		ret = FAIL;
		clearerr (stdin);
	}

	return ret;

}

