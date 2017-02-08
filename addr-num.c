#include <stdlib.h>

#include "edna.h"
#include "addr.h"
#include "buffer.h"

Set
addr_num (Node *nod, Buffer *buf, char *err)
{
	return setaddmemb (makeset (SETLEN), SETLEN, strtol (nod->str->v, NULL, 10));
}

Set
addr_dot (Node *nod, Buffer *buf, char *err)
{
	return setaddmemb (makeset (SETLEN), SETLEN, buf->pos);
}

Set
addr_dollar (Node *nod, Buffer *buf, char *err)
{
	return setaddmemb (makeset (SETLEN), SETLEN, buf->len);
}
