/* line.c -- low-level line manipulation functions */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "edna.h"

int
changeline (Line *l, String *s)
{
	if (l->str == NULL)
		l->str = makestring (LINESIZE);

	if (FAIL == copystring (l->str, s))
		return (FAIL);

	return (SUCC);
}

size_t
getlineno (const Line *targ)
{
	size_t lineno;

	lineno = 1;
	while ((targ = getprev(targ)))
		++lineno;

	return (lineno);
}

Line *
getnext (const Line *li)
{
	if (li == NULL)
		return (NULL);
	return (li->next);
}

Line *
getprev (const Line *li)
{
	if (li == NULL)
		return (NULL);
	return (li->prev);
}

void
freelines (Line *start, Line *stop)
{
	Line *cur, *next, *prev, *tmp;

	prev = start->prev;
	cur = start;
	next = cur->next;
	do {
		tmp = (next ? next->next : NULL);
		freestring (cur->str);
		free (cur);
		cur = next;
		next = tmp;
	} while (cur && cur != stop);

	/* line around freed region */
	linklines (prev, stop);

	return;
}

void
linklines(Line *left, Line *right)
{
	if (left)
		left->next = right;
	if (right)
		right->prev = left;
	return;
}

Line*
makeline ()
{
	Line* new;

	if (!(new = calloc (1, sizeof *new)))
		die("calloc");

	return (new);
}

Line *
walk (Line *cur, int offset)
{
	Line *li = cur;
	if (0 > offset) {
		for (; li; li = getprev(li))
			if (!++offset)
				return (li);
		return (NULL);
	} else if (0 < offset) {
		for (; li; li = getnext(li))
			if (!--offset)
				return (li);
		return (NULL);
	} else /* offset == 0 */
		return (cur);
}
