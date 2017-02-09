#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"
#include "util.h"
#include "vector.h"

Set *
cloneset (Set *dest)
{
	Set *ret;

	ret = make_set ();
	if (vec_copy (*ret, *dest) == FAIL) {
		free_set (ret);
		return NULL;
	}

	return ret;
}

void
free_set (Set *A)
{
	free_vector (*A);
	free (A);
}

Set *
make_set (void)
{
	Set *ret;

	ret = malloc (sizeof *ret);
	if (!ret) die ("malloc");
	
	make_vector (*ret);

	return ret;
}


size_t
offset (uint32_t i)
{
	size_t ret = 0;

	if (i == 0)
		return (0);

	do {
		++ret;
	} while (i >>= 1);

	return ret;
}

void *
set2vec (Set *A)
{
	Set *B;
	subset b;
	size_t i, j, t[32];
	VECTOR (size_t, *ret);

	ret = calloc (1, sizeof *ret);
	if (!ret) die ("calloc");
	make_vector (*ret);
	B = cloneset (A);

	for (i = 0; i < A->c; ++i) {
		for (j = 0; B->v[i]; ++j) {
			b = B->v[i] & -B->v[i];
			t[j] = offset (b);
			B->v[i] ^= b;
		}
		while (j)
			vec_append (*ret, t[--j]);
	}
	free_set (B);
	return ret;
}

Set *
setaddmemb (Set *A, size_t memb)
{
	const size_t i = A->z * CHAR_BIT;
	size_t j;

	for (j = 0; j < A->c; ++j, memb -= i)
		if (memb <= i) {
			A->v[j] |= BIT (memb);
			return A;
		}

	return A;
}

Set *
setaddrange (Set *A, size_t beg, size_t end)
{
	size_t i, j;
	uint32_t k, n, premask, postmask;

	if (beg > end)
		return NULL;

	if (end > A->c)
		return NULL;

	for (i = j = 0; i < end; i += 32, ++j) {
		premask = postmask = 0;
		if (i + 31U > beg) {
			k = BIT (beg - i);
			if ( k > 1 )
				premask = k - 1;
			n = end - i > 32 ? BIT (31) : BIT (end - i);
			if ( n < BIT (31))
				postmask = ~(n - 1);

			A->v[j] = ~(uint32_t)0;
			A->v[j] ^= premask;
			A->v[j] ^= postmask;
		}
	}

	return A;
}

Set *
setcomplement (Set *A)
{
	Set *B;
	size_t i;

	B = make_set ();

	for (i = 0; i < A->c; ++i)
		B->v[i] = ~A->v[i];

	return B;
}

Set *
setdifference (Set *A, Set *B)
{
	Set *C;
	size_t i, len;

	C = make_set ();
	len = MIN (A->c, B->c);

	for (i = 0; i < len; ++i)
		C->v[i] = A->v[i] ^ B->v[i];

	return C;
}

Set *
setintersect (Set *A, Set *B)
{
	Set *C;
	size_t i, len;

	C = make_set ();
	len = MIN (A->c, B->c);

	for (i = 0; i < len; ++i)
		C->v[i] = A->v[i] & B->v[i];

	return C;
}

size_t
setrightmost (Set *A)
{
	Set *C;
	size_t i, j;

	C = A;
	for (i = 0; i < A->c; ++i)
		if (A->v[i])
			C->v = A->v + i;

	j = offset (*C->v);

	j += (C->v - A->v) * 32;

	return (j);
}

Set *
setshift (Set *A, size_t off, int left)
{
	doubleset d;
	Set *B;
	size_t i, len;

	if (A->c * 32 < off)
		return (NULL);

	B = make_set ();

	d = 0;
	len = A->c;
	for (i = len; i --> 0; d = 0){
		if (left) {
			if (i != len - 1)
				d = A->v[i+1];
			d <<= 32;
			d |= A->v[i];

			d <<= off % 32;
			B->v[i] = d;
		} else {
			d = A->v[i];
			d <<= 32;

			if (i)
				d |= A->v[i - 1];

			d >>= off % 32;
			d >>= 32;
			B->v[i] = d;
		}

	}

	memset (A, 0, len);
	memcpy (A + off / 32, B, len - off / 32);

	free_set (B);

	return (A);

}

Set *
setunion (Set *A, Set *B)
{
	Set *C;
	size_t i, len;

	C = make_set();
	len = MIN (A->c, B->c);

	for (i = 0; i < len; ++i)
	 	C->v[i] = A->v[i] | B->v[i];

	return C;
}

