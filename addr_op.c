/* addr_op.h -- routines for composing line addresses */

#include "addr.h"
#include "edna.h"
#include "set.h"

Set
addr_plus (Node *left, Node *right, Buffer *buf, char *err)
{
	Set ret = NULL, pos = NULL;
	size_t off;

	if (left)
		pos = evaltree (left, buf, err);

	else	/* default to current line */
		pos = addr_dot (left, buf, err);

	if (!pos)
		goto fail;

	if (right) {

		ret = evaltree (right, buf, err);

		if (!ret)
			goto fail;

		off = setrightmost (pos, SETLEN);

		if (!setshiftleft (ret, SETLEN, off))
			goto fail;

	} else { /* default to one */

		ret = makeset (SETLEN);
		setaddmemb (ret, SETLEN, 1);

		off = setrightmost (pos, SETLEN);

		if (!setshiftleft (ret, SETLEN, off))
			goto fail;

	}


fail:
	strcpy (err, "invalid line address");
	freeset (ret);
	freeset (pos);
	return NULL;
	
}
