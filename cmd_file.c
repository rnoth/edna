/* cmd_file.c -- file manipulation commands */
#include <stdio.h>
#include <string.h>

#include "edna.h"
#include "cmd.h"

extern int	filename	(State *, Buffer *, Arg *, char *);
extern int	quit		(State *, Buffer *, Arg *, char *);
extern int	write		(State *, Buffer *, Arg *, char *);

int
filename (State *st, Buffer *buf, Arg *arg, char *error)
{
	if (!arg->cnt) {
		if (0 > printf ("%s\n", buf->filename))
			die ("printf");
		return 0;
	}
	if (buf->file && fclose (buf->file) == EOF) {
		warn ("fclose");
		strcpy (error, "could not close current file");
		return 1;
	} else
		strcpy (buf->filename, arg->vec[0]);
	return 0;
}

int
quit (State *st, Buffer *buf, Arg *arg, char *error)
{
	if (arg->mode) {
		if (!strcmp (arg->mode, "force"))
			goto end;
		if (!strcmp (arg->mode, "write"))
			write (st, buf, arg, error);
		strcpy (error, "unknown option");
		return 1;
	}

	if (buf->dirty) {
		strcpy (error, "buffer has unsaved changes");
		return 1;
	}

end:
	strcpy (error, "quit");
	return 0;
}	

int
write (State *st, Buffer *buf, Arg *arg, char *error)
{
	if (arg->cnt && arg->vec[0])
		strcpy (buf->filename, arg->vec[0]);

	arg->addr = -buf->lineno + 1; /* go to start of buffer */
	gotol (st, buf, arg, error);
	writebuf (buf);
	buf->dirty = 0;
	return 0;
}
