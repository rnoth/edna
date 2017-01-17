#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "str.h"

String *
chartostr (char *src)
{
	String *ret;
	if (!(ret = malloc (sizeof *ret))) die ("malloc");
	*ret = makestring (strlen (src) + 1);
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
	bzero (dest->v, dest->m);
	memcpy (dest->v, src->v, src->c);
	dest->c = src->c;
	return 1;
}

void
freestring (String str)
{
	free (str.v);
}

String
makestring (size_t len)
{
	String str;

	if (!(str.v = calloc (len, sizeof *str.v)))
		die ("calloc");

	str.c = 0;
	str.m = len;

	return str;
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
