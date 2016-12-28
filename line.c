#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "edna.h"

extern size_t LINESIZE;

extern Line*	changeline	(Line *, char *, size_t);
extern Line*	freelines	(Line *, Line *);
extern Line*	insertline	(Line *, char *, size_t);
extern Line*	linklines	(Line *, Line*);
extern Line*	makeline	();
extern size_t	readline	(char **); /* FIXME: put this in it's own file */

Line *
changeline (Line *targ, char *buf, size_t bufsiz)
{
	if (!targ->str)
		MALLOC (targ->str, sizeof targ->str * bufsiz);
	if (targ->len && targ->len < bufsiz)
		REALLOC (targ->str, sizeof *targ->str * bufsiz);
	MEMCPY (targ->str, buf, bufsiz + 1); /* + 1 for the terminating 0 byte */
	targ->len = bufsiz;
	return targ;
}

Line *
freelines (Line *start, Line *stop)
{
	Line *cur, *next, *tmp;
	if (start->prev) /* link around the region to be freed */
		(start->prev)->next = stop;
	cur = start;
	next = cur->next;
	do {
		tmp = (next ? next->next : NULL);
		free (cur->str);
		free (cur);
		cur = next;
		next = tmp;
	} while (cur && cur != stop);
	return stop;
}

Line*
appendline (Line *cur, char* buf, size_t bufsiz)
{
	Line *new;
	new = makeline ();
	changeline (new, buf, bufsiz);
	if (cur->next)
		linklines (new, cur->next);
	linklines (cur, new);
	return new;
}

Line *
linklines(Line *left, Line *right)
{
	if (left)
		left->next = right;
	if (right)
		right->prev = left;

	return right;
}

Line*
makeline ()
{
	Line* new;
	CALLOC (new, 1, sizeof *new);
	return new;
}

size_t
readline (char **buf)
{
	size_t bufsiz = LINESIZE;
	GETLINE (*buf, bufsiz, stdin);
	return bufsiz;
}
