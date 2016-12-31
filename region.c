/* region.h -- functions for line ranges */
#include <stdlib.h>
#include <stdio.h>

#include "edna.h"

extern void freelines	(Line *, Line *);
extern void linklines	(Line *, Line *);

void
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

