#include "str.h"

void
chompstr(String *s)
{
	if (s->v[s->b - 2] == '\n') {
		s->v[s->b - 2] = 0;

		--s->b;
		--s->c;
	}
}

bool
eol(String const *str, size_t pos)
{
	return pos >= str->b - 1;
}
