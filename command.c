/* commands.c -- user itnerface to edna */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edna.h"

extern int	append	(Position *, Arg *, char *);
extern int	back	(Position *, Arg *, char *);
extern int	delete	(Position *, Arg *, char *);
extern int	forward	(Position *, Arg *, char *);
extern int	insert	(Position *, Arg *, char *)
extern int	print	(Position *, Arg *, char *);
extern int	quit	(Position *, Arg *, char *);

int
append (Position *pos, Arg *arg, char *error)
{
	char *buf;
	size_t bufsiz;
	MALLOC (buf, LINESIZE * sizeof *buf);
	PRINTF ("%ld\t", pos->lineno + 1);
	bufsiz = readline (&buf);
	if(!(pos->line = putline (pos->line, buf, bufsiz, 1))) {
		strcpy (error, "append failed");
		return 1; /* error */
	}
	++pos->lineno;
	return 0;
}

int
back (Position *pos, Arg *arg, char *error)
{
	if (!pos->line->prev) {
		strcpy (error, "begining of file");
		return 1;
	}
	pos->line = pos->line->prev;
	--pos->lineno;
	return 0;
}

int
change (Position *pos, Arg *arg, char *error)
{
	char *buf;
	size_t bufsiz;
	MALLOC (buf, LINESIZE * sizeof *buf);
	PRINTF ("%ld\t", pos->lineno);
	bufsiz = readline (&buf);
	if(!(pos->line = putline (pos->line, buf, bufsiz, 0))) {
		strcpy (error, "changeline failed");
		return 1; /* error */
	}
	return 0;
}

int
delete (Position *pos, Arg *arg, char *error)
{
	Line *tmp;
	if (!pos->line) {
		strcpy (error, "empty buffer");
		return 1;
	}
	tmp = pos->line->next ? pos->line->next : pos->line->prev;
	if (!tmp)
		tmp = makeline ();

	freelines(pos->line, pos->line->next);

	pos->line = tmp;
	if (!pos->line->next)	/* lineno only decreases at the buffer end */
		--pos->lineno;
	return 0;
}

int
forward (Position *pos, Arg *arg, char *error)
{
	if (!pos->line->next) {
		strcpy (error, "end of file");
		return 1;
	}
	pos->line = pos->line->next;
	++pos->lineno;
	return 0;
}

int
insert (Position *pos, Arg *arg, char *error)
{
	char *buf;
	size_t bufsiz;
	MALLOC (buf, LINESIZE * sizeof *buf);
	PRINTF ("%ld\t", pos->lineno);
	bufsiz = readline (&buf);
	if(!(pos->line = putline (pos->line, buf, bufsiz, -1))) {
		strcpy (error, "insertion failed");
		return 1; /* error */
	}
	return 0;
}

int
help (Position *pos, Arg *arg, char *error)
{
	PRINTF ("%s\n", error);
	return 0;
}

int
nop (Position *pos, Arg *arg, char *error)
{
	strcpy (error, "unknown command");
	return 1;
}

int
print (Position *pos, Arg *arg, char *error)
{
	Line *l;
	if (!pos->line) {
		strcpy (error, "empty buffer");
		return 1;
	}
	l = walk (pos->line, arg->addr, error);
	if (!l)
		return 1;
	if (!l->str) {
		strcpy (error, "null string");
		return 1;
	}
	PRINTF ("%ld\t%s", pos->lineno + arg->addr, l->str);
	return 0;
}

int
quit (Position *pos, Arg *arg, char *error)
{
	strcpy (error, "quit");
	return 0;
}	
