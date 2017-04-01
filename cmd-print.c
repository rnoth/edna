#include <stdio.h>
#include <string.h>

#include "edna.h"

#include "config.h"

int
cmd_help(State *st, Buffer *buf, Arg *arg, char *error)
{
	if (error[0]) {
		if (printf("%s\n", error) < 0) die("printf");
		if (fflush(stdout) == EOF) die("fflush");
	}
	return 0;
}

int
cmd_print(State *st, Buffer *buf, Arg *arg, char *error)
{
	if (!arg->sel->v[0] || !arg->sel->v[0]->str) {
		strcpy(error, "empty selection");
		return -1;
	}

	bufseekline(buf, *arg->sel->v);
	if (printf(PRINT_FMT) < 0) die("printf");
	if (fflush(stdout) == EOF) die("fflush");
	return 0;
}
