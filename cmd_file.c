/* cmd_file.c -- file manipulation commands */
#include <stdio.h>
#include <string.h>

#include "edna.h"
#include "cmd.h"

extern int	edit		(State *, Buffer *, Arg *, char *);
extern int	filename	(State *, Buffer *, Arg *, char *);
extern int	quit		(State *, Buffer *, Arg *, char *);
extern int	write		(State *, Buffer *, Arg *, char *);

int
edit (State *st, Buffer *buf, Arg *arg, char *error)
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

	for (; buf->curline->next; buf->curline = buf->curline->next)
		;
	freelines (buf->curline, NULL);
	buf->curline = makeline();
	buf->lineno = 0;

	if (arg->cnt)
		strcpy (buf->filename, arg->vec[0]);

	readbuf (buf, error);
	return SUCC;
}

int
filename (State *st, Buffer *buf, Arg *arg, char *error)
{
	if (!arg->cnt) {
		if (0 > printf ("%s\n", buf->filename))
			die ("printf");
		return SUCC;
	}

	if (buf->file && fclose (buf->file) == EOF) {
		warn ("fclose");
		strcpy (error, "could not close current file");
		return FAIL;
	} else
		strcpy (buf->filename, arg->vec[0]);
	return SUCC;
}

int
quit (State *st, Buffer *buf, Arg *arg, char *error)
{
	if (arg->mode) {
		if (!strcmp (arg->mode, "force"))
			goto end;
		if (!strcmp (arg->mode, "write"))
			if (write (st, buf, arg, error) == FAIL)
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
write (State *st, Buffer *buf, Arg *arg, char *error)
{
	if (arg->cnt && arg->vec[0])
		strcpy (buf->filename, arg->vec[0]);

	if (writebuf (buf, error) == FAIL)
		return FAIL;
	buf->dirty = 0;
	return SUCC;
}
