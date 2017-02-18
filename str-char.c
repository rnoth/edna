#include <stdlib.h>
#include <string.h>

#include "str.h"
#include "util.h"

bool
appendchar(String *dest, char const src)
{
	if (dest->b + 1 >= dest->m)
		if (!expandstring (dest))
			return false;

	dest->v[dest->b - 1] = src;
	dest->v[dest->b] = 0;
	++dest->c;
	++dest->b;

	return true;
}

bool
appendchars(String *dest, char const *src)
{
	size_t len;
	len = strlen(src); 

	if (dest->m < dest->b + len)
		if (expandstring(dest))
			return false;

	strcat(dest->v, src);
	dest->b += strlen(src);
	dest->c += ustrlen(src);

	return true;
}

String *
chartostr(char const *src)
{
	String *ret;

	ret = makestring();
	appendchars(ret, src);

	return ret;
}

void
chomp(char *s)
{
	char *t, *nl;

	nl = NULL;
	for (t = s; *t; ++t) if (*t == '\n') nl = t;

	if (nl) *nl = 0;

	return;
}

String *
clonechars(char const *src)
{
	String *ret;
	ret = makestring();
	return copychars(ret, src);
}

String *
copychars(String *dest, char const *src)
{
	if (dest->m < strlen (src))
		if (expandstring (dest) == FAIL)
			return NULL;

	strcpy (dest->v, src);
	dest->b = strlen (src) + 1;
	dest->c = ustrlen (src);
	return dest;
}

char *
strtochar(String const *str)
{
	char *ret;

	if (!str) return NULL;

	ret = malloc (str->b);
	if (!ret) die("malloc");

	memcpy(ret, str->v, str->b);

	return ret;
}
