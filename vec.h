#ifndef _vector_
#define _vector_
#include <stddef.h>

#define VECSIZ 16

#define arr(vec) ((vec)->v)
#define len(vec) ((vec)->c)
#define mem(vec) ((vec)->m)

#define Vector(Type)		\
	struct {		\
		size_t c;	\
		size_t m;	\
		Type *v;	\
	}

/* deprecated */
#define free_vector(INST) do {		\
	Vector(void) *inst;		\
	inst = (void *) INST;		\
	if (inst) free(inst->v);	\
	memset(inst, 0, sizeof *inst);	\
	free(inst);			\
} while (0)

#define make_vector(INST) do {				\
	Vector(void) *inst;				\
							\
	inst = malloc(sizeof *inst);			\
	if (inst) {					\
		if (vec_alloc(inst,sizeof *arr(INST))) {\
			free(inst);			\
			inst = NULL;			\
		}					\
	}						\
	(INST) = (void *)inst;				\
} while (0)

#define mapv_step(n) (_j += (n))
#define mapv(var, VEC)						\
	for (Vector_t *_vec = (void *){0} = (VEC), *_p=(void*)1;\
			_vec && _p;				\
			_p = 0)					\
	for (size_t _i=0,_j=0, siz = sizeof *arr(VEC);		\
			_i < len(_vec);				\
			++_i, _j = _i)				\
	for (char _q = 1; _q;)					\
	for (var = (void*)(arr(_vec) + _i*siz);			\
			_i = _q ? len(_vec) : _j, _q;		\
			_q = 0)

#define tovec(arr, len) { .c = len, .v = arr, .m = len }

#define vec_append(vec, el) _vec_append(vec, el, sizeof *arr(vec))
#define vec_clone(vec) _vec_clone(vec, sizeof *arr(vec))
#define vec_concat(vec, a, len) _vec_concat(vec, a, len, sizeof *arr(vec))
#define vec_delete(vec, id) _vec_delete(vec, id, sizeof *arr(vec))
#define vec_insert(vec, el, wh) _vec_insert(vec, el, wh, sizeof *arr(vec))
#define vec_join(dest, src) _vec_join(dest, src, sizeof *arr(dest))
#define vec_prepend(vec, el) _vec_prepend(vec, el, sizeof *arr(vec))
#define vec_shift(vec, off) _vec_shift(vec, off, sizeof *arr(vec))
#define vec_slice(vec, beg, end) _vec_slice(vec, beg, end, sizeof *arr(vec))
#define vec_truncate(vec, ext) _vec_truncate(vec, ext, sizeof *arr(vec))

typedef Vector(char) Vector_t;

int vec_alloc(void *, size_t);
int _vec_append(void *, void const *, size_t);
void *_vec_clone(void const *, size_t);
int _vec_concat(void *, void const *, size_t, size_t);
void _vec_delete(void *, size_t, size_t);
int _vec_insert(void *, void const *, size_t, size_t);
int _vec_join(void *, void const *, size_t);
void vec_free(void *);
int _vec_prepend(void *, void const *, size_t);
void _vec_shift(void *, size_t, size_t);
void _vec_slice(void *, size_t, size_t, size_t);
void _vec_truncate(void *, size_t, size_t);

#endif
