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
	} INST

#define _tagged_vector(TAG, TYPE, INST)			\
	struct TAG {					\
		TYPE *v;				\
		size_t c;				\
		size_t m;				\
	} INST

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
	}

#define _resize_vec(inst, size) {			\
		void *tmp;				\
							\
		tmp = inst->v;				\
		inst->v = realloc (inst->v,		\
				   size);		\
		if (!inst->v) {				\
			warn ("realloc");		\
			inst->v = tmp;			\
		} else {				\
			memset (inst->v + inst->m - 1,	\
			0, size - inst->m);		\
			inst->m = size;			\
		}					\
	}

#define _vec_insert(type, inst, loc, item) {		\
		if (inst->c >= inst->m) {		\
			_resize_vec (inst, inst->c + 1);\
		}					\
							\
		if (!memmove (inst->v + loc + 1,	\
			inst->v + loc, inst->m - loc))	\
			die ("memmove");		\
		if (!memcpy (inst->v + loc,		\
				&item, sizeof item))	\
			die ("memcpy");			\
		inst->c += 1;				\
	}


#define FREE_VECTOR(INST) {				\
		free (INST.v); 				\
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
		seg 	=(inst->c - loc)		\
			  * sizeof *inst->v;		\
							\
		if (loc <= inst->c) {			\
			memmove (inst->v + loc,		\
			  inst->v + loc + 1, seg);	\
			--inst->c;			\
		}					\
	}
			

#endif
