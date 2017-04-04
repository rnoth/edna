#include "edna.h"

int
typecmp(Type const left[static 1], Type const right)
{
	if (*left == EXPR) return 0;
	return *left != right;
}

size_t
typenext(Type const left[static 1])
{
	if (left[0] == NIL) return 0;

	return left[1] != CONT ? 1 : 0;
}
