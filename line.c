#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edna.h"

int
changeline(Line *l, String *s)
{
	if (!l->str) l->str = makestring();

	vec_truncate(l->str, 0);
	return vec_join(l->str, s);
}

size_t
getlineno(Line *targ)
{
	size_t lineno;

	lineno = 0;
	while ((targ = getprev(targ))) ++lineno;

	return lineno;
}

Line *
getnext(Line *li)
{
	if (!li) return 0;
	return li->next;
}

Line *
getprev(Line *li)
{
	if (!li) return 0;
	return li->prev;
}

void
freelines(Line *start, Line *stop)
{
	Line *cur, *next, *prev, *tmp;

	prev = getprev(start);
	cur = start;
	next = getnext(cur);
	while (cur && cur != stop) {
		tmp = getnext(next);
		freestring(cur->str);
		free(cur);
		cur = next;
		next = tmp;
	}

	linklines(prev, stop);
}

void
linklines(Line *left, Line *right)
{
	if (left) left->next = right;
	if (right) right->prev = left;
}

Line*
makeline(void)
{
	return calloc(1, sizeof(Line));
}

Line *
walk(Line *cur, int offset)
{
	Line *li = cur;
	if (offset < 0) {
		while ((li = getprev(li))) if (!++offset) return (li);
		return NULL;
	} else if (offset > 0) {
		while ((li = getnext(li))) if (!--offset) return (li);
		return NULL;
	} else return cur;
}
