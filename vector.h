/* vector.h -- ugly macro-based vector implementation */
#ifndef _vector_
#define _vector_

#include <stdlib.h>
#include <string.h>

#include "util.h"

#define VECTOR(TYPE, INST)				\
	struct {					\
		TYPE *v;  /* vector (data) */		\
		size_t c; /* count (length) */		\
		size_t m; /* memory allocated */	\
		size_t z; /* size of unit */		\
	} INST

#define _tagged_vector(TAG, TYPE, INST)			\
	typedef struct TAG {					\
		TYPE *v;				\
		size_t c;				\
		size_t m;				\
		size_t z;				\
	} INST TAG

#define MAKE_VECTOR(TYPE, INST, SIZE) {			\
		_tagged_vector (vec, TYPE,		\
					*inst); 	\
		size_t size;				\
							\
		size = SIZE;				\
		inst = (struct vec *) &(INST);		\
							\
		inst->v = calloc (size, 		\
				sizeof (TYPE)); 	\
		if (!inst->v) die ("calloc");		\
		inst->c = 0;				\
		inst->m = size; 			\
		inst->z = sizeof (TYPE);		\
	}

#define _resize_vec(inst, size) {			\
		size_t diff;				\
		void *tmp;				\
							\
		diff = size - inst->m;			\
		tmp = realloc (inst->v, size);		\
		if (!tmp) {				\
			warn ("realloc");		\
			inst->v = tmp;			\
		} else {				\
			inst->v = tmp;			\
			if (diff > 0)			\
				bzero (inst->v+inst->m,	\
					diff);		\
			inst->m = size;			\
		}					\
	}

#define RESIZE_VEC(TYPE, INST, SIZE) {			\
		_tagged_vector (vec, TYPE, inst);	\
		size_t size;				\
							\
		inst = INST;				\
		size = SIZE;				\
							\
		_resize_vec (inst, size);		\
	}

#define _vec_insert(type, inst, loc, item) {		\
		if (inst->c >= inst->m) {		\
			_resize_vec (inst, inst->c + 1);\
		}					\
							\
		memmove (inst->v + loc + 1,		\
			 inst->v + loc,			\
			 inst->z * (inst->c - loc));	\
		*(inst->v + loc) = item;		\
		inst->c += 1;				\
	}


#define VEC_INSERT(TYPE, INST, LOC, ITEM) {		\
		_tagged_vector (vec, TYPE, *inst);	\
		size_t	loc;				\
		TYPE	item;				\
							\
		inst	= (struct vec *) &(INST);	\
		loc	= LOC;				\
		item	= ITEM;				\
							\
		_vec_insert (TYPE, inst, loc, item);	\
	}

#define VEC_APPEND(TYPE, INST, ITEM) {			\
		_tagged_vector (vec, TYPE, *inst);	\
		TYPE	item;				\
							\
		inst	= (struct vec *) &(INST);	\
		item	= ITEM;				\
							\
		_vec_insert (TYPE, inst, inst->c, item);\
	}

#define VEC_REMOVE(TYPE, INST, LOC) {			\
		_tagged_vector (vec, TYPE, *inst);	\
		size_t	loc, seg;			\
							\
		inst	= (struct vec *) &(INST);	\
		loc	= LOC;				\
		seg 	= (inst->c - loc)		\
			   * sizeof *inst->v;		\
							\
		if (loc <= inst->c) {			\
			memmove (inst->v + loc,		\
			      inst->v + loc + 1, seg);	\
			--inst->c;			\
		}					\
	}
			
#define FREE_VECTOR(INST) {				\
		free (INST.v); 				\
	}


#endif
