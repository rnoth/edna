#include <stdlib.h>

#include "edna.h"
#include "addr.h"

Set
addr_num (Node *nod, Buffer *buf, char *err)
{
	return setaddmemb (makeset (SETLEN), buf->len, strtol (nod->str->v, NULL, 10));
}

Set
addr_dot (Node *nod, Buffer *buf, char *err)
{
	return setaddmemb (makeset (SETLEN), buf->len, buf->lineno);
}

Set
addr_dollar (Node *nod, Buffer *buf, char *err)
{
	return setaddmemb (makeset (SETLEN), buf->len, buf->len);
}
