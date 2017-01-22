#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "str.h"

String *
chartostr (char *src)
{
	String *ret;
	if (!(ret = malloc (sizeof *ret))) die ("malloc");
	ret = makestring (strlen (src) + 1);
	strcpy (ret->v, src);
	ret->c = strlen (src);
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
		return 1;
	}
	str->v = tmp;
	str->m = len;
	return 0;
}
