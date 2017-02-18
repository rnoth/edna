#include <stdlib.h>
#include <string.h>

#include "str.h"
#include "util.h"

bool
expandstring(String *str)
{
	char *tmp;

	tmp = realloc(str->v, str->m * 2);
	if (!tmp) {
		warn("realloc");
		return false;
	}
	str->v = tmp;
	str->m *= 2;
	return true;
}

bool
copystring(String *dest, String const *src)
{
	if (src->b > dest->m)
		if (!expandstring(dest)) return false;
	memset(dest->v, 0, dest->m);
	memcpy(dest->v, src->v, src->c);
	dest->c = src->c;
	return true;
}

void
freestring(String *str)
{
	if (str == NULL) return;
	free(str->v);
	memset(str, 0, sizeof *str);
	free(str);
}

String *
makestring(void)
{
	String *ret;

	ret = calloc(1, sizeof *ret);
	if (!ret) die("calloc");

	ret->v = calloc(80, 1);
	if (!ret->v) die("calloc");

	ret->m = 80;
	ret->b = 1;

	return ret;
}
