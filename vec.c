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
vec_alloc(void *_vec, size_t size)
{
	Vector(char) vec;

	memcpy(&vec, _vec, sizeof vec);

	vec.c = 0;
	vec.m = VECSIZ;
	vec.v = calloc(VECSIZ, size);
	if (!vec.v) return ENOMEM;

	memcpy(_vec, &vec, sizeof vec);

	return 0;
}

int
_vec_append(void *_vec, void const *data, size_t size)
{
	Vector(char) vec;
	
	memcpy(&vec, _vec, sizeof vec);

	return _vec_insert(_vec, data, vec.c, size);
}

void *
_vec_clone(void const *_vec, size_t size)
{
	Vector(char) vec, *ret;

	memcpy(&vec, _vec, sizeof vec);
	
	ret = malloc(sizeof *ret);
	if (!ret) return NULL;
	memcpy(ret, _vec, sizeof *ret);
	ret->v = malloc(vec.m * size);
	if (!ret->v) { free(ret); return NULL; }
	memcpy(ret->v, vec.v, vec.m);

	return ret;
}

int
_vec_concat(void *_vec, void const *data, size_t nmemb, size_t size)
{
	size_t len;
	Vector(char) vec;

	assert(_vec);
	assert(data);

	memcpy(&vec, _vec, sizeof vec);
	len = nmemb * size;

	while ((vec.c + nmemb) * size >= vec.m) if (vec_expand(&vec)) return ENOMEM;
	
	memcpy(vec.v + vec.c * size, data, len);

	vec.c += nmemb;
	memcpy(_vec, &vec, sizeof vec);

	return 0;
}

void
_vec_delete(void *_vec, size_t which, size_t size)
{
	Vector(char) vec;

	memcpy(&vec, _vec, sizeof vec);
	if (which > vec.c) return;

	memmove(vec.v + which * size, vec.v + (which + 1) * size, (vec.c - which) * size);

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
_vec_insert(void *_vec, void const *data, size_t pos, size_t size)
{
	Vector(char) vec;

	assert(_vec);
	assert(data);

	memcpy(&vec, _vec, sizeof vec);

	assert(pos <= vec.c);

	if ((vec.c + 1) * size >= vec.m && vec_expand(&vec)) return ENOMEM;

	memmove(vec.v + (pos + 1) * size, vec.v + pos * size, (vec.c - pos) * size);
	memcpy(vec.v + pos * size, data, size);

	++vec.c;

	memcpy(_vec, &vec, sizeof vec);

	return 0;
}

int
_vec_join(void *_dest, void const *_src, size_t size)
{
	Vector(char) src;

	memcpy(&src, _src, sizeof src);

	return _vec_concat(_dest, src.v, src.c, size);
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
_vec_prepend(void *vec, void const *data, size_t size)
{
	return _vec_insert(vec, data, 0, size);
}

void
_vec_shift(void *_vec, size_t off, size_t size)
{
	Vector(char) vec;

	memcpy(&vec, _vec, sizeof vec);
	_vec_slice(_vec, off, vec.c - off, size);

	return;
}

void
_vec_slice(void *_vec, size_t beg, size_t ext, size_t size)
{
	size_t min;
	Vector(char) vec;

	memcpy(&vec, _vec, sizeof vec);
	if (beg >= vec.c) {
		_vec_truncate(_vec, 0, size);
		return;
	}

	min = MIN(ext, vec.c - beg);

	memmove(vec.v, vec.v + beg * size, min * size);
	memset(vec.v + (beg + min) * size, 0, (vec.c-min-beg) * size);

	vec.c = min;
	memcpy(_vec, &vec, sizeof vec);
}

void
_vec_truncate(void *_vec, size_t off, size_t size)
{
	Vector(char) vec;

	assert(_vec);

	memcpy(&vec, _vec, sizeof vec);

	memset(vec.v + off * size, 0, (vec.c - off) * size);
	vec.c = off;

	memcpy(_vec, &vec, sizeof vec);
}
