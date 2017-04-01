#include <stdlib.h>
#include "edna.h"

Set *
addr_num (Node *nod, Buffer *buf, char *err)
{
	return setaddmemb (makeset(), strtol (nod->str->v, NULL, 10));
}

Set *
addr_dot (Node *nod, Buffer *buf, char *err)
{
	return setaddmemb (makeset(), bufgetpos(buf));
}

Set *
addr_dollar (Node *nod, Buffer *buf, char *err)
{
	return setaddmemb (makeset(), bufgetlen(buf));
}
