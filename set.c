#include <limits.h>
#include <stdlib.h>

#include "edna.h"
#include "set.h"

Set
makeset (Buffer *buf)
{
	Set A;
	A = calloc (buf->len % (sizeof *A * CHAR_BIT), sizeof *A);
	return A;
}

Set
setaddmemb (Set A, size_t len, size_t memb)
{
	size_t i = sizeof *A * CHAR_BIT;
	for (size_t j=0; j < len && memb > 0; ++j, memb -= i)
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
	B = malloc (sizeof *B * len);
	if (!B) die ("malloc");
	for (size_t i = 0; i < len; ++i)
		B[i] = ~A[i];
	return B;
}

Set
setdifference (Set A, Set B, size_t len)
{
	Set C;
	C = malloc (sizeof *C * len);
	if (!C) die ("malloc");
	for (size_t i = 0; i < len; ++i)
		C[i] = A[i] ^ B[i];
	return C;
}

Set
setintersect (Set A, Set B, size_t len)
{
	Set C;
	C = malloc (sizeof *C * len);
	if (!C) die ("malloc");
	for (size_t i = 0; i < len; ++i)
		C[i] = A[i] & B[i];
	return C;
}

Set
setunion (Set A, Set B, size_t len)
{
	Set C;
	C = malloc (sizeof *C * len);
	if (!C) die ("malloc");
	for (size_t i = 0; i < len; ++i)
		C[i] = A[i] | B[i];
	return C;
}

