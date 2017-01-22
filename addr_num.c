#include <stdlib.h>

#include "edna.h"
#include "addr.h"

Set
num (String *s, Buffer *buf, char *error)
{
	return setaddmemb (makeset (buf), buf->len, strtol (s->v, NULL, 10));
}

Set
symnum (String *s, Buffer *buf, char *error)
{
	switch (*s->v) {
	case '.':
		return setaddmemb (makeset (buf), buf->len, buf->lineno);
		break;
	case '$':
		return setaddmemb (makeset (buf), buf->len, buf->len);
		break;
	default:
		return NULL;
		break;
	}
}
