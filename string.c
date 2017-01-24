#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "str.h"

/* TODO: use FAIL and SUCC instead of 0 and 1 */

#if 0
int
appendstring (String *dest, String *src)
{
	int ret = 0; /* false */
	void *tmp;
	if (dest->m <= dest->b + src->b) { /* not enough space */
		tmp = realloc (dest->v, dest->b + src->b);
		if (!tmp) [
			ret = 1; /* error */
			goto finally;
		}
		dest->v = tmp;
		dest->m = dest->b + src->b;
}
#endif
		
int
appendchar (String *dest, char src)
{
	/* TODO: this probably isn't the right behavior */
	if (!isascii (src)) /* don't make an invalid utf-8 sequence */
		return 0;
	if (dest->b == dest->m && !resizestring (dest, dest->m * 2))
		return 0;
	dest->v[dest->b++] = src;
	dest->v[dest->b++] = 0;
	++dest->c;
	return 1;
}

String *
chartostr (char *src)
{
	String *ret;

	ret = makestring (strlen (src) + 1);

	strcpy (ret->v, src);
	ret->b = strlen (src);
	ret->c = strlen (src); /* TODO: not utf-8 aware */
	return ret;
}

int
copystring (String *dest, String *src)
{
	/* TODO: recover from errors instead of just returing failure */
	if (!dest || !dest->v)
		return 0; /* error */
	if (dest->m < src->c)
		return 0;
	memset (dest->v, 0, dest->m);
	memcpy (dest->v, src->v, src->c);
	dest->c = src->c;
	return 1;
}

void
freestring (String *str)
{
	free (str->v);
	free (str);
}

String *
makestring (size_t len)
{
	String *ret;

	if (!(ret = calloc (1, sizeof *ret)))
		die ("calloc");
	if (!(ret->v = calloc (len, sizeof *ret->v)))
		die ("calloc");

	ret->m = len;

	return ret;
}

int
resizestring (String *str, size_t len)
{
	char *tmp;

	if (!(tmp = realloc (str->v, len))) {
		warn ("realloc");
		return 0;
	}
	str->v = tmp;
	str->m = len;
	return 1;
}
