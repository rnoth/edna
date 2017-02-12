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
#define vec_concat(inst, data, len)	_vec_concat ((Vector *)&(inst), data, len * sizeof *data)
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

/* note: the side effects con be removed if we're willing to add TYPE as arg */
#define make_vector(INST) { 				\
	(INST).c = 0;					\
	(INST).z = sizeof *((INST).v);			\
	(INST).v = calloc (VECSIZ, sizeof *((INST).v));	\
	if (!(INST).v) die ("malloc");			\
	(INST).m = VECSIZ;				\
}

#define free_vector(INST) {				\
	free ((INST).v);				\
	(INST).v = NULL;				\
	(INST).m = 0;					\
}

/* example: mapf (dest, src, sqrt(src.c[i])) */
#define mapf(dest, src, fun) 			\
	for (size_t i = 0; i < src.c; ++i)	\
		vec_append (dest, fun);
#define filterf(dest, src, fun)				\
	for (size_t i = 0; i < src.c; ++i)		\
		if (fun)				\
			vec_append (dest, src.v[i])
#define reducef(dest, src, fun)			\
	for (size_t i = 0; i < src.c; ++i)	\
		dest = fun;
#endif
