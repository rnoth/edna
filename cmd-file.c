#include <stdio.h>
#include <string.h>
#include "edna.h"

int
cmd_edit(State *st, Buffer *buf, Arg *arg, char *error)
{
	char *fn;

	if (arg->mode && !strcmp (arg->mode, "force"))
		goto force;

	if (isdirty(buf)) {
		strcpy (error, "buffer has unsved changes");
		return -1;
	}

force:
	if (arg->param->c) {
		fn = arg->param->v[0];
		chomp(fn);
	} else fn = bufgetname(buf);

	killbuf(buf);
	initbuf(buf, fn);
	readbuf(buf, error);
	bufclean(buf);
	setmode(st, "command");

	if (!arg->param->c) free(fn);
	return 0;
}

int
cmd_filename(State *st, Buffer *buf, Arg *arg, char *error)
{
	char *fn;
	if (!arg->param->c) {
		fn = bufgetname(buf);
		if (printf("%s\n", fn) < 0) die ("printf");
		free(fn);
		return 0;
	}

		if (bufsetname(buf, arg->param->v[0]) == FAIL) {
		strcpy(error, "invalid filename");
		return -1;
	}

	return 0;
}

int
cmd_quit (State *st, Buffer *buf, Arg *arg, char *error)
{
	if (arg->mode) {
		if (!strcmp (arg->mode, "force"))
			goto end;
		if (!strcmp (arg->mode, "write"))
			if (cmd_write (st, buf, arg, error) == FAIL)
				return FAIL;
		strcpy (error, "unknown option");
		return -1;
	}

	/* note: quit should become a mode, and handle this on its own */
	if (isdirty(buf)) {
		strcpy (error, "buffer has unsaved changes");
		return -1;
	}

end:
	strcpy (error, "quit");
	return -1;
}	

int
cmd_write (State *st, Buffer *buf, Arg *arg, char *error)
{
	int ret;

	if (arg->param->c && bufsetname(buf, arg->param->v[0])) {
		strcpy (error, "invalid filename");
		return -1;
	}

	ret = writebuf(buf, error);
	if (!ret) bufclean(buf);
	return ret;
}
