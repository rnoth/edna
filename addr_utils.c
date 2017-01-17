#include <stdlib.h>
#include <limits.h>

#include "edna.h"
#include "addr.h"
#include "set.h"
#include "vector.h"

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

Selection
resolveset (Set A, size_t len, Buffer *buf, char *error)
{
	Selection ret;
	Set B;
	size_t bit, off, *t, h;

	if (!(t = calloc (len, sizeof *t))) die ("calloc");
	MAKE_VECTOR (Line*, ret, sizeof *A * CHAR_BIT);

	/* TODO: once default addresses are implemented, come back and turn
	 * this into an error or something */
	if (!A) { /* no line address */
		free (t);
		return ret;
	} /* TODO: remove special case */

	off = 0;
	for (h = 0, B = A; (unsigned)(B - A) < len; ) {
		subset b;

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
				return (ret);
			}
			VEC_APPEND (Line*, ret, tmp);
		}
		++B;
		off += sizeof *B;
	}

	free (t);
	RESIZE_VEC (Line*, ret, ret.c * ret.z);

	return ret;
}
