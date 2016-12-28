/* commands.c -- user itnerface to edna */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edna.h"

extern int LINESIZE;

extern int	append	(Position *pos, char *);
extern int	back	(Position *pos, char *);
extern int	delete	(Position *pos, char *);
extern int	forward	(Position *pos, char *);
extern int	print	(Position *pos, char *);
extern int	quit	(Position *pos, char *);

int
append (Position *pos, char *error)
{
	char *buf;
	size_t bufsiz;
	MALLOC (buf, LINESIZE * sizeof *buf);
	bufsiz = LINESIZE;
	bufsiz = readline (&buf);
	if(!(pos->line = putline (pos->line, buf, bufsiz, 1))) {
		strcpy (error, "insertion failed");
		return 1; /* error */
	}
	++pos->lineno;
	return 0;
}

int
back (Position *pos, char *error)
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
delete (Position *pos, char *error)
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
forward (Position *pos, char *error)
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
help (Position *pos, char *error)
{
	PRINTF ("%s\n", error);
	return 0;
}

int
nop (Position *pos, char *error)
{
	strcpy (error, "unknown command");
	return 1;
}

int
print (Position *pos, char *error)
{
	if (!pos->line) {
		strcpy (error, "empty buffer");
		return 1;
	}
	if (!pos->line->str) {
		strcpy (error, "null string");
		return 1;
	}
	PRINTF ("%s", pos->line->str);
	return 0;
}

int
quit (Position *pos, char *error)
{
	strcpy (error, "quit");
	return 0;
}	
