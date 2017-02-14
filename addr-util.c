#include <stdlib.h>
#include <limits.h>

#include "edna.h"
#include "addr.h"
#include "buffer.h"
#include "line.h"
#include "set.h"
#include "util.h"
#include "vector.h"

Selection *
resolveset (Set *A, Buffer *buf, char *error)
{
	void *tmp;
	VECTOR (size_t, *stack);
	Selection *ret;
	size_t i;

	if (!A) return (NULL);

	ret = calloc (1, sizeof *ret);
	if (!ret) die ("calloc");

	make_vector (*ret);

	stack = set2vec (A);
	if (!stack->c)
		goto invalid;

	for (i = 0; i < stack->c; ++i) {
		tmp = walk (buf->top, stack->v[i]);
		if (!tmp)
			goto invalid;
		vec_append (*ret, tmp);
	}

	free_vector (*stack);
	free (stack);

	return (ret);

invalid:
	strcpy (error, "invalid line address");
	free_vector (*ret);
	free (ret);
	free_vector (*stack);
	free (stack);

	return (NULL);
}
