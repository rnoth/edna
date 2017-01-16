#include <stdlib.h>

#include "edna.h"
#include "addr.h"
#include "set.h"

/*
Set
defaultarg (char *s, Buffer *buf, enum direc d, char *error)
{
	if (*s == ',') {
		if (d == LEFT)
			return (*evals[NUM_LITERAL]) ("1", buf, error);
		if (d == RIGHT)
			return (*evals[NUM_SYMBOL]) ("$", buf, error);
	} else if (d == LEFT) {
		return (*evals[NUM_SYMBOL]) (".", buf, error);
	} else if (d == RIGHT) {
		return (*evals[NUM_SYMBOL]) ("1", buf, error);
	}
	return NULL; / * not reached * /
}

Operator
getbinop (char *s)
{
	switch (*s) {
	case '+':
		return add;
	case '-':
		return sub;
	case ',':
		return range;
	case '|':
		return or;
	case '&':
		return and;
	case '^':
		return xor;
	default:
		return NULL;
	}
}
*/

Line **
resolveset (Set A, size_t len, Buffer *buf, char *error)
{
	Line *tmp, **ret = NULL;
	Set B;
	size_t height, bit, off, *t, h;

	if (!(t = malloc (len * sizeof *t))) die ("malloc");

	height = off = 0;
	for (h = 0, B = A; (unsigned)(B - A) < len; ) {
		subset b;
		b = *B & -*B;	/* isolate rightmost bit */

		while (*B) {	/* convert bitset to stack of values */
			for (bit=off; b; ++bit) /* count trailing zeros */
				b >>= 1;
			*t++ = bit;	/* push onto stack */
			++h;
			*B ^= b;
			b = *B & ~*B;
		}

		while (h && h --> 0) {	/* convert stack to stack of lines */
			tmp = walk (buf->top, *t--, error);
			if (!tmp) {
				free (t);
				return NULL;
			}
			*ret++ = tmp;	/* push tmp */
			++height;
		}
		++B;
		off += sizeof *B;
	}

	free (t);
	return ret - height;
}
