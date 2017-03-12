#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "vec.h"

#define VECSIZ 16
#define GROWTH 2
//#define SHRINK .3

static int vec_expand(void *);

int
vec_alloc(void *_vec)
{
	Vector(char) vec;

	memcpy(&vec, _vec, sizeof vec);

	vec.c = 0;
	vec.m = VECSIZ;
	vec.v = calloc(VECSIZ, vec.z);
	if (!vec.v) return ENOMEM;

	memcpy(_vec, &vec, sizeof vec);

	return 0;
}

int
vec_append(void *_vec, void const *data)
{
	Vector(char) vec;
	
	memcpy(&vec, _vec, sizeof vec);

	return vec_insert(_vec, data, vec.c);
}

void *
vec_clone(void const *_vec)
{
	Vector(char) vec, *ret;

	memcpy(&vec, _vec, sizeof vec);
	
	ret = malloc(sizeof *ret);
	if (!ret) return NULL;
	memcpy(ret, _vec, sizeof *ret);
	ret->v = malloc(vec.m * vec.z);
	if (!ret->v) { free(ret); return NULL; }
	memcpy(ret->v, vec.v, vec.m);

	return ret;
}

int
vec_concat(void *_vec, void const *data, size_t nmemb)
{
	size_t len;
	Vector(char) vec;

	assert(_vec);
	assert(data);

	memcpy(&vec, _vec, sizeof vec);
	len = nmemb * vec.z;

	while ((vec.c + nmemb) * vec.z >= vec.m) if (vec_expand(&vec)) return ENOMEM;
	
	memcpy(vec.v + vec.c, data, len);

	vec.c += nmemb;
	memcpy(_vec, &vec, sizeof vec);

	return 0;
}

void
vec_delete(void *_vec, size_t which)
{
	Vector(char) vec;

	memcpy(&vec, _vec, sizeof vec);
	if (which > vec.c) return;

	memmove(vec.v + which * vec.z, vec.v + (which + 1) * vec.z, (vec.c - which) * vec.z);

	--vec.c;

	memcpy(_vec, &vec, sizeof vec);
}

int
vec_expand(void *_vec)
{
	Vector(char) vec;
	void *tmp;

	assert(_vec);

	memcpy(&vec, _vec, sizeof vec);
	assert(vec.m);

	tmp = realloc(vec.v, vec.m * GROWTH);
	if (!tmp) return ENOMEM;

	vec.v = tmp;
	vec.m *= GROWTH;

	memcpy(_vec, &vec, sizeof vec);
	return 0;
}

int
vec_insert(void *_vec, void const *data, size_t pos)
{
	Vector(char) vec;

	assert(_vec);
	assert(data);

	memcpy(&vec, _vec, sizeof vec);

	assert(pos <= vec.c);

	if ((vec.c + 1) * vec.z >= vec.m && vec_expand(&vec)) return ENOMEM;

	memmove(vec.v + (pos + 1) * vec.z, vec.v + pos * vec.z, (vec.c - pos) * vec.z);
	memcpy(vec.v + pos * vec.z, data, vec.z);

	++vec.c;

	memcpy(_vec, &vec, sizeof vec);

	return 0;
}

int
vec_join(void *_dest, void const *_src)
{
	Vector(char) src;

	memcpy(&src, _src, sizeof src);

	return vec_concat(_dest, src.v, src.c);
}

void
vec_free(void *_vec)
{
	Vector(char) vec;

	if (!_vec) return;

	memcpy(&vec, _vec, sizeof vec);
	free(vec.v);
	memset(_vec, 0, sizeof vec);
	free(_vec);
}


int
vec_prepend(void *vec, void const *data)
{
	return vec_insert(vec, data, 0);
}

void
vec_shift(void *_vec, size_t off)
{
	Vector(char) vec;
	memcpy(&vec, _vec, sizeof vec);
	vec_slice(_vec, off, vec.c - off);
	return;
}

void
vec_slice(void *_vec, size_t beg, size_t ext)
{
	size_t min;
	Vector(char) vec;

	memcpy(&vec, _vec, sizeof vec);
	if (beg >= vec.c) {
		vec_truncate(_vec, 0);
		return;
	}

	min = MIN(ext, vec.c - beg);

	memmove(vec.v, vec.v + beg * vec.z, min * vec.z);
	memset(vec.v + (beg + min) * vec.z, 0, (vec.c - min) * vec.z);

	vec.c = min;
	memcpy(_vec, &vec, sizeof vec);
}

void
vec_truncate(void *_vec, size_t off)
{
	Vector(char) vec;

	assert(_vec);

	memcpy(&vec, _vec, sizeof vec);

	memset(vec.v + off * vec.z, 0, (vec.c - off) * vec.z);
	vec.c = off;

	memcpy(_vec, &vec, sizeof vec);
}
