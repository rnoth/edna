/* commands.c -- user interface to edna */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edna.h"
#include "cmd.h"

extern int	delete		(State *, Buffer *, Arg *, char *);
extern int	filename	(State *, Buffer *, Arg *, char *);
extern int	gotol		(State *, Buffer *, Arg *, char *);
extern int	insert		(State *, Buffer *, Arg *, char *);
extern int	print		(State *, Buffer *, Arg *, char *);
extern int	quit		(State *, Buffer *, Arg *, char *);
extern int	write		(State *, Buffer *, Arg *, char *);

int
delete (State *st, Buffer *buf, Arg *arg, char *error)
{
	Line *tmp;

	if (!buf->curline->str) {
		strcpy (error, "empty buffer");
		return 1;
	}

	if (arg->addr)
		if (gotol (st, buf, arg, error))
			return 1;

	buf->dirty = 1;

	tmp = buf->curline->next ? buf->curline->next : buf->curline->prev;
	if (!tmp)
		tmp = makeline ();

	if (!buf->curline->next)
		--buf->lineno;
	freelines(buf->curline, buf->curline->next);

	buf->curline = tmp;
	return 0;
}

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
gotol (State *st, Buffer *buf, Arg *arg, char *error)
{
	Line *l;

	if (!arg->addr)
		++arg->addr;

	l = walk (buf->curline, arg->addr, error);
	if (!l)
		return 1;
	buf->lineno += arg->addr;
	buf->curline = l;
	return 0;
}

int
help (State *st, Buffer *buf, Arg *arg, char *error)
{
	if (0 > printf ("%s\n", error))
		die("printf");
	return 0;
}

int
insert (State *st, Buffer *buf, Arg *arg, char *error)
{
	char *line;
	size_t linelen;
	int option;

	option = INSERT;

	if (arg->mode) {
		if (!strcmp (arg->mode, "insert")) {
			; /* insert mode is redundant */
		} else if (!strcmp (arg->mode, "append")) {
			option = APPEND;
		} else if (!strcmp (arg->mode, "change")) {
			option = CHANGE;
		} else {
			strcpy (error, "unknown option");
			return 1;
		}
	}

	if (arg->addr && gotol(st, buf, arg, error))
		return 1;

	if (buf->lineno == 0 || option == APPEND)
		++buf->lineno;

	buf->dirty = 1;

	line = malloc (sizeof *line * LINESIZE);
	linelen = LINESIZE;
	for (;readline (&line, &linelen, "%ld\t", buf->lineno), strcmp (line, ".\n");) {
		if(!(buf->curline = putline (buf->curline, line, linelen, option))) {
			free (line);
			strcpy (error, "insertion failed");
			return 1; /* error */
		}
		++buf->lineno;
		option = APPEND;
	}
	--buf->lineno;

	free (line);
	return 0;
}

int
nop (State *st, Buffer *buf, Arg *arg, char *error)
{
	strcpy (error, "unknown command");
	return 1;
}

int
print (State *st, Buffer *buf, Arg *arg, char *error)
{
	if (arg->addr && gotol (st, buf, arg, error))
		return 1;

	if (!buf->curline->str) {
		strcpy (error, "empty buffer");
		return 1;
	}

	if (0 > printf ("%ld\t%s", buf->lineno, buf->curline->str))
		die("printf");
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
