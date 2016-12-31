/* commands.c -- user interface to edna */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edna.h"

extern int	delete		(State *, Arg *, char *);
extern int	filename	(State *, Arg *, char *);
extern int	gotol		(State *, Arg *, char *);
extern int	insert		(State *, Arg *, char *);
extern int	print		(State *, Arg *, char *);
extern int	quit		(State *, Arg *, char *);
extern int	write		(State *, Arg *, char *);

int
delete (State *st, Arg *arg, char *error)
{
	Line *tmp;

	if (!st->curline->str) {
		strcpy (error, "empty buffer");
		return 1;
	}

	if (arg->addr)
		if (gotol (st, arg, error))
			return 1;

	st->dirty = 1;

	tmp = st->curline->next ? st->curline->next : st->curline->prev;
	if (!tmp)
		tmp = makeline ();

	if (!st->curline->next)
		--st->lineno;
	(void) freelines(st->curline, st->curline->next);

	st->curline = tmp;
	return 0;
}

int
filename (State *st, Arg *arg, char *error)
{
	if (st->file)
		if (fclose (st->file) == EOF) {
			warn ("fclose");
			strcpy (error, "could not close current file");
			return 1;
		}
	/* FIXME: no sanity checking on arg->vec or arg->cnt */
	if (!arg->vec) {
		if (0 > printf ("%s\n", st->filename))
			die ("printf");
		return 0;
	}
	if (!arg->vec[0]) {
		strcpy (error, "parsing error. this is not your fault");
		return 1;
	}
	strcpy (st->filename, arg->vec[0]);
	return 0;
}

int
gotol (State *st, Arg *arg, char *error)
{
	Line *l;

	if (!arg->addr)
		++arg->addr;

	l = walk (st->curline, arg->addr, error);
	if (!l)
		return 1;
	st->lineno += arg->addr;
	st->curline = l;
	return 0;
}

int
help (State *st, Arg *arg, char *error)
{
	if (0 > printf ("%s\n", error))
		die("printf");
	return 0;
}

int
insert (State *st, Arg *arg, char *error)
{
	char *buf;
	size_t bufsiz;
	int option;

	option = INSERT; /* -1 is putline's insert mode */

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

	if (arg->addr)
		if (gotol(st, arg, error))
			return 1;

	if (st->lineno == 0 || option == APPEND)
		++st->lineno;

	st->dirty = 1;

	buf = malloc (sizeof *buf * LINESIZE);
	bufsiz = LINESIZE;
	for (;readline (&buf, &bufsiz, "%ld\t", st->lineno),
	      strcmp (buf, ".\n");) {
		if(!(st->curline = putline (st->curline, buf, bufsiz, option))) {
			free (buf);
			strcpy (error, "insertion failed");
			return 1; /* error */
		}
		++st->lineno;
		option = APPEND;
	}
	--st->lineno;

	free (buf);
	return 0;
}

int
nop (State *st, Arg *arg, char *error)
{
	strcpy (error, "unknown command");
	return 1;
}

int
print (State *st, Arg *arg, char *error)
{
	if (arg->addr)
		if (gotol (st, arg, error))
			return 1;

	if (!st->curline->str) {
		strcpy (error, "empty buffer");
		return 1;
	}

	if (0 > printf ("%ld\t%s", st->lineno, st->curline->str))
		die("printf");
	return 0;
}

int
quit (State *st, Arg *arg, char *error)
{
	if (arg->mode) {
		if (!strcmp (arg->mode, "force"))
			goto end;
		if (!strcmp (arg->mode, "write"))
			write (st, arg, error);
		strcpy (error, "unknown option");
		return 1;
	}

	if (st->dirty) {
		strcpy (error, "buffer has unsaved changes");
		return 1;
	}

end:
	strcpy (error, "quit");
	return 0;
}	

int
write (State *st, Arg *arg, char *error)
{
	if (!st->file && !st->filename[0]) {
		if (!arg->vec) {
			strcpy (error, "no open file and no default filename");
			return 1;
		}
		if(filename (st, arg, error))
			return 1;
	}
	arg->addr = -st->lineno + 1; /* go to start of buffer */
	gotol (st, arg, error);
	writefile (st);
	st->dirty = 0;
	return 0;
}
