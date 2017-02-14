#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "str.h"

#if 0
int
appendstring (String *dest, String *src)
{
	int ret = 0; /* false */
	void *tmp;
	if (dest->m <= dest->b + src->b) { /* not enough space */
}
#endif
		
void
chompstr(String *s)
{
	if (s->v[s->b - 2] == '\n') {
		s->v[s->b - 2] = 0;

		--s->b;
		--s->c;
	}
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
	return pos >= s->b - 1;
}

void
freestring (String *str)
{
	if (str == NULL) return;
	free (str->v);
	free (str);
}

String *
makestring ()
{
	String *ret;

	if (!(ret = calloc (1, sizeof *ret))) die ("calloc");
	if (!(ret->v = calloc (80, 1))) die ("calloc");

	ret->m = 80;
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

