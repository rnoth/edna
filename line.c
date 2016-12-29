#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "edna.h"

extern Line*	changeline	(Line *, char *, size_t);
extern Line*	freelines	(Line *, Line *);
extern Line*	linklines	(Line *, Line*);
extern Line*	makeline	();
extern size_t	readline	(char **); /* FIXME: put this in it's own file */
extern Line*	walk		(Line *, int, char *error);

Line *
changeline (Line *targ, char *buf, size_t bufsiz)
{
	if (!targ->str)
		MALLOC (targ->str, sizeof *targ->str * bufsiz);
	if (!targ->str) /* malloc failed, let's bail */
		return NULL;
	if (targ->len && targ->len < bufsiz)
		REALLOC (targ->str, sizeof *targ->str * bufsiz);
	if (!targ->str)	/* realloc failed, get out of here */
		return NULL;
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
	Line *new;

	/* only call makeline() when buffer isn't already empty */
	new = cur->str ? makeline() : cur;
	if (!changeline (new, buf, bufsiz)) {
		free (new);
		return NULL;
	}
	switch (option) {
	case 1:	/* append */
		linklines (new, cur->next);
		if (new != cur)	/* don't make a infinite loop */
			linklines (cur, new);
		break;
	case 0:	/* change */
		linklines (cur->prev, new);
		linklines (new, cur->next);
		if (new != cur) /* no use-after-free, please */
			freelines (cur, cur->next);
		break;
	case -1: /* insert */
		linklines (cur->prev, new);
		if (new != cur)
			linklines (new, cur);
		break;
	}
	return new;
}

size_t
readline (char **buf)
{
	size_t bufsiz = LINESIZE;
	MALLOC (*buf, sizeof **buf * LINESIZE);
	GETLINE (*buf, bufsiz, stdin);
	return bufsiz;
}

Line *
walk (Line *cur, int offset, char *error)
{
	Line *l = cur;
	if (0 < offset) {
		while ((l = l->next))
			if (!--offset)
				return l;
		strcpy (error, "end of file");
		return NULL;
	}
	if (0 > offset) {
		while ((l = l->prev))
			if (!++offset)
				return l;
		strcpy (error, "start of file");
		return NULL;
	}
	return cur;		
}
