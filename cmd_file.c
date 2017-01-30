/* cmd_file.c -- file manipulation commands */
#include <stdio.h>
#include <string.h>

#include "edna.h"
#include "cmd.h"

int
cmd_edit (State *st, Buffer *buf, Arg *arg, char *error)
{
	if (arg->mode && !strcmp (arg->mode, "force"))
		goto end;
	if (buf->dirty) {
		strcpy (error, "buffer has unsved changes");
		return FAIL;
	}

end:
	if (EOF == fclose (buf->file)) {
		warn ("fclose");
		return FAIL;
	}

	freelines (buf->top, NULL);
	buf->top = buf->bot = buf->curline = NULL;
	buf->len = buf->lineno = 0;

	if (arg->param.c)
		strcpy (buf->filename, arg->param.v[0]);

	readbuf (buf, error);
	return (SUCC);
}

int
cmd_filename (State *st, Buffer *buf, Arg *arg, char *error)
{
	if (!arg->param.c) {
		if (0 > printf ("%s\n", buf->filename)) die ("printf");
		return (SUCC);
	}

	if (buf->file && fclose (buf->file) == EOF) {
		warn ("fclose");
		strcpy (error, "could not close current file");
		return (FAIL);
	} else if (arg->param.v[0][0] == '\0') {
		strcpy (error, "invalid filename");
		return (FAIL);
	}

	strcpy (buf->filename, arg->param.v[0]);

	return (SUCC);
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
		return (FAIL);
	}

	if (buf->dirty) {
		strcpy (error, "buffer has unsaved changes");
		return (FAIL);
	}

end:
	strcpy (error, "quit");
	return (FAIL);
}	

int
cmd_write (State *st, Buffer *buf, Arg *arg, char *error)
{
	if (arg->param.c) {
		if (arg->param.v[0][0])
			strcpy (buf->filename, arg->vec[0]);
		else {
			strcpy (error, "invalid filename");
			return (FAIL);
		}
	}

	if (writebuf (buf, error) == FAIL)
		return (FAIL);
	buf->dirty = 0;
	return (SUCC);
}
