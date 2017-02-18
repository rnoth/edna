#include <stdio.h>

#include "edna.h"

static Mode *findmode(State *, char *);

int
addbuf(State *st, Buffer buf)
{
	vec_append(st->buffers, buf);
	return SUCC;
}


int
checkoutbuf(Buffer dest, State *st, size_t which)
{
	Buffer src;

	if (which >= st->buffers.c)
		return FAIL;

	src = st->buffers.v[which];
	copybuf(dest, src);

	vec_remove(st->buffers, which);

	return SUCC;
}

Mode *
findmode(State *st, char *mode)
{
	size_t i = 0;
	for (; i < st->modes.c; ++i)
		if (!strcmp(st->modes.v[i].name, mode))
			return st->modes.v + i;
	return NULL;
}

int
returnbuf(State *st, Buffer src)
{
	Buffer tmp;

	tmp = clonebuf(src);
	vec_append (st->buffers, tmp);
	
	return SUCC;
}

int
setmode(State *st, char *name)
{
	Mode *m;

	m = findmode(st, name);
	st->mode = m;
	return SUCC;
}
