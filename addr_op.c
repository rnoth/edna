/* addr_op.h -- routines for composing line addresses */
#include <stdlib.h>
#include <string.h>

#include "addr.h"
#include "buf.h"
#include "edna.h"
#include "set.h"

Set
addr_plus (Node *left, Node *right, Buffer *buf, char *err)
{
	Set ret = NULL, inc = NULL;
	size_t off;

	if (left)
		ret = evaltree (left, buf, err);

	else	/* default to current line */
		ret = addr_dot (left, buf, err);

	if (!ret)
		goto fail;

	if (right) {

		inc = evaltree (right, buf, err);

		if (!inc)
			goto fail;

		off = setrightmost (inc, SETLEN);

		if (!setshiftleft (ret, SETLEN, off))
			goto fail;

	} else { /* default to one */

		inc = makeset (SETLEN);

		off = 1;

		if (!setshiftleft (ret, SETLEN, off))
			goto fail;

	}

	return (ret);

fail:
	strcpy (err, "invalid line address");
	freeset (ret);
	freeset (inc);
	return NULL;
	
}
