#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "edna.h"

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
	Line *cur, *next, *prev, *tmp;

	prev = start->prev;
	cur = start;
	next = cur->next;
	do {
		tmp = (next ? next->next : NULL);
		free (cur->str);
		free (cur);
		cur = next;
		next = tmp;
	} while (cur && cur != stop);

	/* line around freed region */
	linklines (prev, stop);

	return stop;
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

Line*
putline (Line *cur, char* buf, size_t bufsiz, int option)
{
	Line *new, *targ;

	new = makeline ();
	changeline (new, buf, bufsiz);
	switch (option) {
	case 1:	/* append */
		targ = cur;
		break;
	case 0:	/* change */
		linklines (cur->prev, new);
		linklines (new, cur->next);
		freelines (cur, cur->next);
		return new;
	case -1: /* insert */
		targ = cur->prev;
		break;
	}
	if (targ->next)
		linklines (new, targ->next);
	if (targ)
		linklines (targ, new);
	return new;
}

size_t
readline (char **buf)
{
	size_t bufsiz = LINESIZE;
	GETLINE (*buf, bufsiz, stdin);
	return bufsiz;
}
