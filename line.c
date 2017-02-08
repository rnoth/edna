#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edna.h"
#include "line.h"
#include "str.h"
#include "util.h"

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
getlineno (Line *targ)
{
	size_t lineno;

	lineno = 0;
	while ((targ = getprev(targ)))
		++lineno;

	return (lineno);
}

Line *
getnext (Line *li)
{
	if (li == NULL)
		return (NULL);
	return (li->next);
}

Line *
getprev (Line *li)
{
	if (li == NULL)
		return (NULL);
	return (li->prev);
}

void
freelines (Line *start, Line *stop)
{
	Line *cur, *next, *prev, *tmp;

	prev = getprev (start);
	cur = start;
	next = getnext (cur);
	while (cur && cur != stop) {
		tmp = getnext (next);
		freestring (cur->str);
		free (cur);
		cur = next;
		next = tmp;
	}

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
