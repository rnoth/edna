/* line.c -- low-level line manipulation functions */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "edna.h"

extern size_t	getlineno	(Line *);
extern Line*	makeline	();
extern Line*	putline		(Line *, char *, size_t);
extern Line*	walk		(Line *, int, char *error);

static Line*	changeline	(Line *, char *, size_t);

Line *
changeline (Line *l, char *line, size_t len)
{
	if (!l->str)
		if (!(l->str = malloc (sizeof *l->str * len + 1)))
			die("malloc");
	if (l->len && l->len < len)
		if (!(l->str = realloc (l->str, sizeof *l->str * len + 1)))
			die("realloc");
	memcpy (l->str, line, len + 1); /* + 1 for the terminating 0 byte */
	l->len = len;
	return l;
}

size_t
getlineno (Line *targ)
{
	size_t lineno;

	lineno = 1;
	while ((targ = targ->prev))
		++lineno;

	return (lineno);
}
Line*
makeline ()
{
	Line* new;
	if (!(new = calloc (1, sizeof *new)))
		die("calloc");
	return new;
}

Line*
putline (Line *cur, char *line, size_t len)
{
	Line *new;

	/* only call makeline() when buffer isn't already empty */
	new = cur->str ? makeline() : cur;

	if (!changeline (new, line, len)) {
		free (new);
		return NULL;
	}

	if (new != cur)	{	/* don't make a infinite loop */
		linklines (new, cur->next);
		linklines (cur, new);
	}
	return new;
}

Line *
walk (Line *cur, int offset, char *error)
{
	Line *li = cur;
	if (0 > offset) {
		for (; li; li = li->prev)
			if (!++offset)
				return li;
		strcpy (error, "start of file");
		return (NULL);
	} else if (0 < offset) {
		for (; li; li = li->next)
			if (!--offset)
				return li;
		strcpy (error, "end of file");
		return (NULL);
	}
	return (cur); /* offset == 0 */
}
