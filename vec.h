#ifndef _vector_
#define _vector_
#include <stddef.h>

#define VECSIZ 16

#define arr(vec) ((vec)->v)
#define len(vec) ((vec)->c)
#define siz(vec) ((vec)->z)
#define mem(vec) ((vec)->m)

#define Vector(Type)		\
	struct {		\
		size_t c;	\
		size_t m;	\
		size_t z;	\
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

#define make_vector(INST) do {			\
	Vector(void) *inst;			\
						\
	inst = malloc(sizeof *inst);		\
	if (inst) {				\
		inst->z = sizeof *(INST)->v;	\
		if (vec_alloc(inst)) {		\
			free(inst);		\
			inst = NULL;		\
		}				\
	}					\
	(INST) = (void *)inst;			\
} while (0)

/* e.g. mapv(vec, sqrt(each)) */
#define mapv(VEC, expr) do {			\
	size_t _i;				\
	void **each;				\
	Vector(void *) *_vec;			\
						\
	_vec = (void *)VEC;			\
	for (_i = 0; _i < len(_vec); ++_i) {	\
		each = arr(_vec) + _i;		\
		expr;				\
	}					\
} while (0)

#define tovec(arr, len) { .c = len, .v = arr, .z = sizeof *arr, .m = len }

int vec_alloc(void *);
int vec_append(void *, void const *);
void *vec_clone(void const *);
int vec_concat(void *, void const *, size_t);
void vec_delete(void *, size_t);
int vec_insert(void *, void const *, size_t);
int vec_join(void *, void const *);
void vec_free(void *);
int vec_prepend(void *, void const *);
void vec_shift(void *, size_t);
void vec_slice(void *, size_t, size_t);
void vec_truncate(void *, size_t);

#endif
