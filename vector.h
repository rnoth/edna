/* vector.h -- ugly macro-based vector implementation */
#ifndef _vector_
#define _vector_

#include <stdlib.h>
#include <string.h>

#ifndef SUCC
#	define SUCC (1)
#	define FAIL (0)
#endif

#define VECSIZ 16

#define vec_append(inst, item)		_vec_append ((Vector *)&(inst), &(item))
#define vec_concat(inst, data, len)	_vec_concat ((Vector *)&(inst), data, len)
#define vec_copy(dest, src)		_vec_copy   ((Vector *)&(dest), (Vector *)&(src))
#define vec_insert(inst, loc, item)	_vec_insert ((Vector *)&(inst), loc, &(item))
#define vec_remove(inst, loc)		_vec_remove ((Vector *)&(inst), loc)

typedef struct Vector Vector;

extern int _vec_append (Vector *, const void *);
extern int _vec_concat (Vector *, const void *, size_t);
extern int _vec_copy (Vector *, const Vector *);
extern int _vec_insert (Vector *, size_t, const void *);
extern int _vec_remove (Vector *, size_t);

struct Vector {
	void *v;
	size_t c;
	size_t m;
	size_t z;
};

#define VECTOR(TYPE, INST)				\
	struct {					\
		TYPE *v;  /* vector (data) */		\
		size_t c; /* count (length) */		\
		size_t m; /* memory allocated */	\
		size_t z; /* size of unit */		\
	} INST

#define VECTOR_TAG(TYPE, TAG)				\
	struct TAG {					\
		TYPE	*v;				\
		size_t	 c;				\
		size_t	 m;				\
		size_t	 z;				\
	}

#define make_vector(INST) do { 				\
	Vector *inst;					\
	inst = (Vector *) &(INST);			\
	inst->c = 0;					\
	inst->z = sizeof *((INST).v);			\
	inst->v = calloc(VECSIZ, sizeof *((INST).v));	\
	if (!inst->v) die("malloc");			\
	inst->m = VECSIZ;				\
} while (0)

#define free_vector(INST) do {		\
	Vector *inst;			\
	inst = (Vector *)&(INST);	\
	free(inst->v);			\
	memset(inst, 0, sizeof *inst);	\
} while (0)

#endif
