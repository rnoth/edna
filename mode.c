#include <string.h>

#include "edna.h"
#include "buf.h"
#include "mode.h"
#include "state.h"

int
setmode (State *st, Buffer *buf, char *mode)
{
	size_t i = 0;
	for (; i < st->modes.c; ++i)
		if (!strcmp (st->modes.v[i].name, mode)) {
			buf->mode = st->modes.v + i;
			return SUCC;
		}
	return FAIL;
}
