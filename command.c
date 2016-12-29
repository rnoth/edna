/* commands.c -- user itnerface to edna */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edna.h"

extern int	append	(Position *, Arg *, char *);
extern int	back	(Position *, Arg *, char *);
extern int	delete	(Position *, Arg *, char *);
extern int	forward	(Position *, Arg *, char *);
extern int	gotol	(Position *, Arg *, char *);
extern int	insert	(Position *, Arg *, char *);
extern int	print	(Position *, Arg *, char *);
extern int	quit	(Position *, Arg *, char *);

int
append (Position *pos, Arg *arg, char *error)
{
	char *buf;
	size_t bufsiz;
	Line *l;
	pos->lineno += arg->addr;
	++pos->lineno;
	PRINTF ("%ld\t", pos->lineno + arg->addr);
	bufsiz = readline (&buf);
	l = walk (pos->line, arg->addr, error);
	if(!(pos->line = putline (l, buf, bufsiz, 1))) {
		strcpy (error, "append failed");
		return 1; /* error */
	}
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
	Line *l;
	if (pos->lineno == 0)
		++pos->lineno;
	pos->lineno += arg->addr;
	PRINTF ("%ld\t", pos->lineno);
	bufsiz = readline (&buf);
	l = walk (pos->line, arg->addr, error);
	if(!(pos->line = putline (l, buf, bufsiz, 0))) {
		strcpy (error, "changeline failed");
		return 1; /* error */
	}
	return 0;
}

int
delete (Position *pos, Arg *arg, char *error)
{
	Line *tmp, *l;
	if (!pos->line->str) {
		strcpy (error, "empty buffer");
		return 1;
	}
	l = walk (pos->line, arg->addr, error);
	pos->lineno += arg->addr;
	tmp = l->next ? l->next : l->prev;
	if (!tmp)
		tmp = makeline ();

	freelines(l, l->next);

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
help (Position *pos, Arg *arg, char *error)
{
	PRINTF ("%s\n", error);
	return 0;
}

int
insert (Position *pos, Arg *arg, char *error)
{
	char *buf;
	size_t bufsiz;
	Line *l;
	if (pos->lineno == 0)
		++pos->lineno;
	pos->lineno += arg->addr;
	PRINTF ("%ld\t", pos->lineno);
	bufsiz = readline (&buf);
	l = walk (pos->line, arg->addr, error);
	if(!(pos->line = putline (l, buf, bufsiz, -1))) {
		strcpy (error, "insertion failed");
		return 1; /* error */
	}
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
	l = walk (pos->line, arg->addr, error);
	if (!l)
		return 1;
	if (!l->str) {
		strcpy (error, "empty buffer");
		return 1;
	}
	pos->lineno += arg->addr;
	PRINTF ("%ld\t%s", pos->lineno, l->str);
	return 0;
}

int
quit (Position *pos, Arg *arg, char *error)
{
	strcpy (error, "quit");
	return 0;
}	
