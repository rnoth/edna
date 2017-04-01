#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"
#include "util.h"
#include "vec.h"

Set *
cloneset(Set *src)
{
	Set *ret;

	ret = makeset();
	memcpy(ret->v, src->v, src->c);

	return ret;
}

int
expandset(Set *A)
{
	size_t c;
	void *tmp;

	c = A->c;
	tmp = realloc(A->v, A->c * 2);
	if (tmp == NULL) die("realloc");

	A->v = tmp;
	memset(A->v + c, 0, c);

	return 0;
}

void
freeset(Set *A)
{
	if (!A) return;
	free(A->v);
	free(A);
}

Set *
makeset(void)
{
#define SETSIZ 8
	Set *ret;

	ret = malloc(sizeof *ret);
	if (!ret) return NULL;
	
	ret->v = calloc(SETSIZ, sizeof *ret->v);
	if (!ret->v) { free(ret); return NULL; }
	ret->c = SETSIZ;

	return ret;
}


size_t
offset(uint32_t i)
{
	size_t ret = 0;

	if (i == 0) return 0;

	do ++ret; while (i >>= 1);

	return ret;
}

void *
set2vec(Set *A)
{
	Set *B = 0;
	uint32_t b = 0;
	size_t i = 0;
	size_t j = 0;
	size_t t[32] = {0};
	Vector(size_t) *ret = 0;

	make_vector(ret);
	if (!ret) die("make_vector");
	B = cloneset(A);

	for (i = 0; i < A->c; ++i) {
		for (j = 0; B->v[i]; ++j) {
			b = B->v[i] & -B->v[i];
			t[j] = offset (b) + i * 32;
			B->v[i] ^= b;
		}
		while (j) if (vec_append(ret, t + --j)) goto fail;
	}
	freeset(B);
	return ret;
fail:
	vec_free(ret);
	freeset(A);
	return NULL;
}

Set *
setaddmemb(Set *A, size_t memb)
{
	if (memb / 32 > A->c) if (expandset(A)) return NULL;

	A->v[memb / 32] |= bit(memb % 32);

	return A;
}

Set *
setaddrange(Set *A, size_t beg, size_t end)
{
	size_t i = 0;
	size_t j = 0;
	uint32_t k = 0;
	uint32_t n = 0;
	uint32_t premask = 0;
	uint32_t postmask = 0;

	if (beg > end) return NULL;

	if (end > A->c && expandset(A)) return NULL;

	for (i = j = 0; i < end; i += 32, ++j) {
		premask = postmask = 0;
		if (i + 31U > beg) {
			k = bit(beg - i);
			if (k > 1) premask = k - 1;
			n = end - i > 32 ? bit(31) : bit(end - i);
			if (n < bit (31)) postmask = ~(n - 1);

			A->v[j] = ~(uint32_t)0;
			A->v[j] ^= premask;
			A->v[j] ^= postmask;
		}
	}

	return A;
}

size_t
setrightmost(Set *A)
{
	Set *C = 0;
	size_t i = 0, j;

	C = A;
	for (i = 0; i < A->c; ++i) if (A->v[i]) C->v = A->v + i;

	j = offset (*C->v);

	j += (C->v - A->v) * 32;

	return j;
}

Set *
setshiftright(Set *A, size_t off)
{
	uint64_t d;
	size_t i = 0;
	size_t len = 0;
	Set *B = 0;

	if (A->c * 32 < off && expandset(A)) return NULL;

	B = makeset();

	len = A->c;
	for (i = 0; i < len; ++i) {
		d = 0;
		if (i + 1 < len) d = A->v[i+1];
		d <<= 32;
		d |= A->v[i];

		d <<= off % 32;
		B->v[i] |= d;
		if (d >> 32) {
		       	if (i == len - 1) {
				if (expandset(B)) {
					freeset(B);
					return NULL;
				}
				len += 1;
			}
			B->v[i+1] = d >> 32;
		}

	}

	memset(A->v, 0, off / 32);
	memcpy(A->v + off / 32, B->v, len - off / 32);

	freeset(B);

	return A;

}
