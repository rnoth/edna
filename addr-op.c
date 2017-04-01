#include <stdlib.h>
#include <string.h>

#include "edna.h"

Set *
addr_plus(Node *left, Node *right, Buffer *buf, char *err)
{
	Set *ret = NULL, *inc = NULL;
	size_t off;

	if (left)
		ret = evaltree(left, buf, err);

	else	/* default to current line */
		ret = addr_dot(left, buf, err);

	if (ret == NULL)
		goto fail;

	if (right) {

		inc = evaltree(right, buf, err);

		if (inc == NULL)
			goto fail;

		off = setrightmost(inc) - 1;

		if (setshiftright(ret, off) == NULL)
			goto fail;

	} else {

		inc = makeset();

		off = 1;

		if (setshiftright(ret, off) == NULL)
			goto fail;

	}

	freeset(inc);
	return ret;

fail:
	strcpy(err, "invalid line address");
	freeset(ret);
	freeset(inc);
	return NULL;
	
}
