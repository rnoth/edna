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
}
#endif
		
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

	ret = makestring (strlen (src) + 1);

	strcpy (ret->v, src);
	ret->b = strlen (src) + 1;
	ret->c = ustrlen (src);
	return ret;
}

String *
clonechars(char *src)
{
	String *ret;
	ret = makestring(80);
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

String *
copystring (String *dest, String *src)
{
	if (dest->m < src->c)
		if (resizestring (dest, src->m) == FAIL)
			return NULL;
	memset (dest->v, 0, dest->m);
	memcpy (dest->v, src->v, src->c);
	dest->c = src->c;
	return dest;
}

bool
eol(const String *s, size_t pos)
{
	return pos >= s->b;
}

void
freestring (String *str)
{
	if (str == NULL) return;
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
	ret->b = 1;

	return ret;
}

int
resizestring(String *str, size_t len)
{
	char *tmp;

	if (!(tmp = realloc(str->v, len))) {
		warn("realloc");
		return 0;
	}
	str->v = tmp;
	str->m = len;
	return 1;
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
