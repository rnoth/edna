#include <stdlib.h>
#include <string.h>

#include "str.h"
#include "util.h"

void
chomp(char *s)
{
	char *nl;
	do if (*s == '\n') nl = s; while (*s++);
	*nl = 0;
}

void
freestring(String *str)
{
	vec_free(str);
}

String *
makestring(void)
{
	String *ret;
	make_vector(ret);
	return ret;
}
