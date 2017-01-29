/* vector.c -- generic vector routines */
#include <stdlib.h>
#include <string.h>

#include "vector.h"

/*
 * TODO: _vec_truncate(), _vec_clone()
 */
static int _expand_vec (Vector *);

int
_vec_append (Vector *inst, const void *data)
{
	return _vec_insert (inst, inst->c, data);
}

int
_vec_concat (Vector *inst, const void *data, size_t len)
{
	if ((inst->c * inst->z + len) > inst->m * inst->z)
		if (!_expand_vec (inst))
			return (FAIL);

	memmove (inst->v, data, len);
	inst->c += len / inst->z;

	return (SUCC);
}

int
_vec_copy (Vector *dest, const Vector *src)
{
	if (dest->m < src->c)
		if (!_expand_vec (dest))
			return (FAIL);

	memcpy ((Vector *)dest->v,
		(Vector *)src->v,
		src->z * src->c);

	dest->c = src->c;

	return (SUCC);
}

int
_expand_vec (Vector *inst)
{
	void *tmp;

	tmp = realloc (inst->v, inst->m * inst->z * 2);

	if (tmp) {
		inst->v = tmp;
		inst->m *= 2;
		return (SUCC);
	} else {
		perror ("realloc");
		return (FAIL);
	}
}

int
_vec_insert (Vector *inst, size_t loc, const void *data)
{
	if (inst->c >= inst->m)
		if (!_expand_vec (inst))
			return (FAIL);

	if (loc > inst->c)
		loc = inst->c;

	else if (loc < inst->c)
		memmove ((char *)inst->v + (loc + 1) * inst->z,
			 (char *)inst->v + loc * inst->z,
			 (inst->c - loc) * inst->z);

	memcpy ((char *)inst->v + loc * inst->z, data, inst->z);
	++inst->c;
	return (SUCC);

}

int
_vec_remove (Vector *inst, size_t loc)
{
	if (loc > inst->c)
		return (SUCC); /* should this be an error? */

	memset ((char *)inst->v + loc, 0, inst->z);
	--inst->c;

	if (loc < inst->c)
		memmove ((char *)inst->v + (loc - 1) * inst->z,
			 (char *)inst->v + loc * inst->z,
			 (inst->c - loc) * inst->z);

	return (SUCC);

}
