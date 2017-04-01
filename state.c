#include <string.h>
#include <stdio.h>

#include "edna.h"

static Mode *findmode(State *, char *);

int
addbuf(State *st, Buffer *buf)
{
	return vec_append(st->buffers, &buf);
}


int
checkoutbuf(Buffer *dest, State *st, size_t which)
{
	Buffer *src;

	if (which >= len(st->buffers)) return EINVAL;

	src = arr(st->buffers)[which];
	memcpy(dest, src, sizeof *dest);

	free(arr(st->buffers)[which]);
	vec_delete(st->buffers, which);

	return 0;
}

Mode *
findmode(State *st, char *mode)
{
	size_t i = 0;
	for (; i < st->modes->c; ++i)
		if (!strcmp(st->modes->v[i].name, mode))
			return st->modes->v + i;
	return NULL;
}

int
returnbuf(State *st, Buffer *src)
{
	Buffer *tmp;

	tmp = clonebuf(src);
	return vec_append(st->buffers, &tmp);
}

int
setmode(State *st, char *name)
{
	Mode *m;

	m = findmode(st, name);
	if (!m) return -1;
	st->mode = m;
	return 0;
}
