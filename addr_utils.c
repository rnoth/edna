#include <stdlib.h>
#include <limits.h>

#include "edna.h"
#include "addr.h"
#include "set.h"
#include "vector.h"

Selection *
resolveset (Set A, size_t len, Buffer *buf, char *error)
{
	Selection *ret = NULL;
	Set B;
	size_t bit, off, *t, h;

	if (!(ret = calloc (1, sizeof *ret))) die ("calloc");

	if (!A)
		return ret;

	make_vector (*ret);
	if (!(t = calloc (len, sizeof *t))) die ("calloc");

	off = 0;
	for (h = 0, B = A; (unsigned)(B - A) < len; ) {
		subset b;

		/* note: not only is this incorrect, it doesn't belong here */
		while (*B) {	/* convert bitset to stack of values */
			b = *B & -*B;	/* isolate rightmost bit */

			for (bit=off; b; ++bit) /* count trailing zeros */
				b >>= 1;

			*t++ = bit;	/* push onto stack */
			++h;

			*B ^= BIT (bit - off - 1); /* remove original value of b */
		}

		while (h && h --> 0) {	/* convert stack to stack of lines */
			Line *tmp;
			tmp = walk (buf->top, *--t, error);
			if (!tmp) {
				free (t - h);
				free (ret);
				return (NULL);
			}
			vec_append (*ret, tmp);
		}
		++B;
		off += sizeof *B;
	}

	free (t);

	return ret;
}
