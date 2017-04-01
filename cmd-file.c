#include <stdio.h>
#include <string.h>
#include "edna.h"

int
cmd_edit(State *st, Buffer *buf, Arg *arg, char *error)
{
	char *fn;
	int err;

	if (arg->mode && !strcmp(arg->mode, "force"))
		goto force;

	if (isdirty(buf)) {
		strcpy(error, "buffer has unsaved changes");
		return -1;
	}

force:
	if (arg->param && len(arg->param)) {
		fn = arr(arg->param)[0];
		chomp(fn);
	} else fn = bufgetname(buf);

	killbuf(buf);
	err = initbuf(buf, fn);
	if (err) return err;
	err = readbuf(buf, error);
	if (err) return err;
	bufclean(buf);
	setmode(st, "command");

	return 0;
}

int
cmd_filename(State *st, Buffer *buf, Arg *arg, char *err)
{
	char *fn;
	if (len(arg->param)) {
		chomp(arr(arg->param)[0]);
		if (!bufsetname(buf, arr(arg->param)[0]))
			return 0;
		strcpy(err, "invalid filename");
		return -1;
	} else if (!(fn = bufgetname(buf))) {
		strcpy(err, "no filename");
		return -1;
	} else if (printf("%s\n", fn) < 0) die("printf");

	return 0;
}

int
cmd_quit (State *st, Buffer *buf, Arg *arg, char *error)
{
	int err;
	if (arg->mode) {
		if (!strcmp (arg->mode, "force"))
			goto end;
		if (!strcmp (arg->mode, "write")) {
			err = cmd_write (st, buf, arg, error);
			if (err) return err;
		}
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
