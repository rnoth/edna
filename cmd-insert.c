#include <stdlib.h>
#include <string.h>
#include "edna.h"

int
cmd_insert(State *st, Buffer *buf, Arg *arg, char *error)
{
	bufseekline(buf, arg->sel->v[0]);

	if (arg->mode) {
		if (!strcmp(arg->mode, "insert")) {
			bufseek (buf, BUF_CUR, -1);
		} else if (!strcmp(arg->mode, "append")) {
			;
		} else if (!strcmp(arg->mode, "change")) {
			if (cmd_delete(st, buf, arg, error))
				return -1;
		} else {
			strcpy(error, "unknown option");
			return 0;
		}
	}

	setmode(st, "insert");
	return 0;
}
