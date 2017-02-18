#include <stdlib.h>
#include "edna.h"

Set *
addr_num (Node *nod, Buffer *buf, char *err)
{
	return setaddmemb (make_set(), strtol (nod->str->v, NULL, 10));
}

Set *
addr_dot (Node *nod, Buffer *buf, char *err)
{
	return setaddmemb (make_set(), bufgetpos(buf));
}

Set *
addr_dollar (Node *nod, Buffer *buf, char *err)
{
	return setaddmemb (make_set(), bufgetlen(buf));
}
