#include <limits.h>
#include <stdlib.h>
#include <string.h>

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
setrightmost (Set A, size_t len)
{
	Set C = NULL;
	subset c;
	size_t i, j;

	for (i = 0; i < len; ++i)
		if (A[i])
			C = A + i;

	if (!C)
		return (0);

	c = *C;
	j = 0;
	do {
		++j;
	} while (c >>= 1);

	j += --i * 32;

	return (j);
}

Set
setshift (Set A, size_t len, size_t off, int left)
{
	doubleset d;
	Set B;
	size_t i;

	if (len * 32 < off)
		return (NULL);

	B = makeset (len);

	d = 0;
	for (i = len; i --> 0; d = 0){
		if (left) {
			if (i != len - 1)
				d = A[i+1];
			d <<= 32;
			d |= A[i];

			d <<= off % 32;
			B[i] = d;
		} else {
			d = A[i];
			d <<= 32;

			if (i)
				d |= A[i - 1];

			d >>= off % 32;
			d >>= 32;
			B[i] = d;
		}

	}

	memset (A, 0, len);
	memcpy (A + off / 32, B, len - off / 32);

	freeset (B);

	return (A);

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

