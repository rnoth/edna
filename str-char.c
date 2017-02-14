#include <stdlib.h>
#include <string.h>

#include "str.h"
#include "util.h"

int
appendchar (String *dest, char src)
{
	if (dest->b + 1 >= dest->m)
		if (resizestring (dest, dest->m * 2) == FAIL)
			return (FAIL);
	dest->v[dest->b - 1] = src;
	dest->v[dest->b] = 0;
	++dest->c;
	++dest->b;
	return (SUCC);
}

int
appendchars (String *dest, char *src)
{
	if (dest->b + strlen (src) > dest->m)
		if (resizestring (dest, dest->m * 2))
			return (FAIL);
	strcat (dest->v, src);
	dest->b += strlen (src);
	dest->c += ustrlen (src);
	return (SUCC);
}

String *
chartostr (char *src)
{
	String *ret;

	ret = makestring ();
	appendchars(ret, src);

	return ret;
}

void
chomp(char *s)
{
	char *t, *nl;

	nl = NULL;
	for (t = s; *t; ++t)
		if (*t == '\n')
			nl = t;

	if (nl) *nl = 0;

	return;
}

String *
clonechars(char *src)
{
	String *ret;
	ret = makestring();
	return copychars(ret, src);
}

String *
copychars (String *dest, const char *src)
{
	if (dest->m < strlen (src))
		if (resizestring (dest, dest->m * 2) == FAIL)
			return NULL;

	strcpy (dest->v, src);
	dest->b = strlen (src) + 1;
	dest->c = ustrlen (src);
	return dest;
}

char *
strtochar(String *str)
{
	char *ret;

	if (str == NULL) return NULL;
	ret = malloc (str->b);
	if (ret == NULL) die("malloc");
	memcpy(ret, str->v, str->b);

	return ret;
}
