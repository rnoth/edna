#include <limits.h>
#include <stdlib.h>

#include "edna.h"
#include "set.h"
#include "util.h"

Set
makeset (size_t len)
{
	Set A;
	A = calloc (len, sizeof *A);
	return A;
}

Set
setaddmemb (Set A, size_t len, size_t memb)
{
	size_t i = sizeof *A * CHAR_BIT;
	size_t j;

	for (j = 0; j < len && memb > 0; ++j, memb -= i)
		if (memb <= i) {
			A[j] |= BIT (memb - 1);
			return A;
		}

	return NULL;
}

Set
setcomplement (Set A, size_t len)
{
	Set B;
	size_t i;

	B = malloc (sizeof *B * len);
	if (!B) die ("malloc");

	for (i = 0; i < len; ++i)
		B[i] = ~A[i];
	return B;
}

Set
setdifference (Set A, Set B, size_t len)
{
	Set C;
	size_t i;

	C = malloc (sizeof *C * len);
	if (!C) die ("malloc");

	for (i = 0; i < len; ++i)
		C[i] = A[i] ^ B[i];
	return C;
}

Set
setintersect (Set A, Set B, size_t len)
{
	Set C;
	size_t i;

	C = malloc (sizeof *C * len);
	if (!C) die ("malloc");

	for (i = 0; i < len; ++i)
		C[i] = A[i] & B[i];

	return C;
}

int
setoffset (Set A, Set B, size_t len)
{
	Set C, tmp; /* pointer to the last elem of B */
	size_t off = 0;

	for (tmp = B; *tmp; ++tmp)
		if (*tmp)
			C = tmp;

	for (tmp = C; *tmp >>= 1; ++off)
		;
	++off;

	if (off > len * sizeof (subset))
		return FAIL;

	memmove (A, A + off, off);

	return SUCC;
}

Set
setunion (Set A, Set B, size_t len)
{
	Set C;
	size_t i;

	C = malloc (sizeof *C * len);
	if (!C) die ("malloc");

	for (i = 0; i < len; ++i)
	 	C[i] = A[i] | B[i];

	return C;
}

