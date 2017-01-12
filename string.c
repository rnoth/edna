#include <stdlib.h>

#include "util.h"
#include "str.h"

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
