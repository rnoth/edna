#include <stdio.h>

#include "edna.h"
#include "buffer.h"
#include "state.h"
#include "vector.h"


int
addbuf (State *st, Buffer *buf)
{
	/* TODO: no error handling */
	vec_append (st->buffers, buf);
	return SUCC;
}


int
checkoutbuf (Buffer *dest, State *st, size_t which)
{
	Buffer *src;

	if (which >= st->buffers.c)
		return FAIL;

	src = st->buffers.v[which];
	memcpy (dest, src, sizeof *dest);

	free (src);
	vec_remove (st->buffers, which);

	dest->mode = st->modes.v;

	return SUCC;
}


int
returnbuf (Buffer *src, State *st)
{
	Buffer *tmp;

	tmp = makebuf (NULL);

	memcpy (tmp, src, sizeof *tmp);
	vec_append (st->buffers, tmp);
	memset (src, 0, sizeof *tmp);
	return SUCC;
}

int
rmbuf (State *st, size_t which)
{
	vec_remove (st->buffers, which);
	return SUCC;
}
