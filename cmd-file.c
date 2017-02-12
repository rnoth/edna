/* cmd_file.c -- file manipulation commands */
#include <stdio.h>
#include <string.h>

#include "edna.h"
#include "buffer.h"
#include "cmd.h"
#include "mode.h"
#include "util.h"

int
cmd_edit(State *st, Buffer *buf, Arg *arg, char *error)
{
	char *fn;
	if (arg->mode && !strcmp (arg->mode, "force"))
		goto force;
	if (buf->dirty) {
		strcpy (error, "buffer has unsved changes");
		return FAIL;
	}

force:
	if (arg->param.c) fn = arg->param.v[0];
	else	fn = strtochar(buf->name);

	killbuf(buf);
	initbuf(buf, fn);
	readbuf(buf, error);
	setmode(st, buf, "command");
	return SUCC;
}

int
cmd_filename(State *st, Buffer *buf, Arg *arg, char *error)
{
	char *fn;
	if (!arg->param.c) {
		fn = strtochar(buf->name);
		if (printf("%s\n", fn) < 0) die ("printf");
		return SUCC;
	}

	if (bufname(buf, arg->param.v[0]) == FAIL) {
		strcpy(error, "invalid filename");
		return FAIL;
	}

	return SUCC;
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
		return FAIL;
	}

	if (buf->dirty) {
		strcpy (error, "buffer has unsaved changes");
		return FAIL;
	}

end:
	strcpy (error, "quit");
	return FAIL;
}	

int
cmd_write (State *st, Buffer *buf, Arg *arg, char *error)
{
	if (arg->param.c) {
		if (bufname(buf, arg->param.v[0]) == FAIL) {
			strcpy (error, "invalid filename");
			return FAIL;
		}
	}

	if (writebuf (buf, error) == FAIL)
		return FAIL;
	buf->dirty = 0;
	return SUCC;
}
