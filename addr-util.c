#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "edna.h"

Selection *
resolveset(Set *A, Buffer *buf, char *error)
{
	size_t i;
	Line *tmp;
	Selection *ret;
	Vector(size_t) *stack;

	if (!A) return NULL;

	make_vector(ret);
	if (!ret) die("make_vector");

	stack = set2vec(A);
	if (!stack->c) goto invalid;

	for (i = 0; i < stack->c; ++i) {
		tmp = bufprobe(buf, stack->v[i] - 1);
		if (!tmp) goto invalid;
		vec_append(ret, &tmp);
	}

	vec_free(stack);

	return ret;

invalid:
	strcpy(error, "invalid line address");
	vec_free(ret);
	vec_free(stack);

	return NULL;
}
