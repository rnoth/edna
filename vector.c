/* vector.c -- generic vector routines */
#include <stdlib.h>

extern int _vec_insert (void *inst, size_t loc, void *item);
#if 0
_vec_insert(inst, loc, item)
{
	if (inst->c >= inst->m) {
		_resize_vec (inst, inst->c + 1);
	}					

	memmove (inst->v + loc + 1,		
		 inst->v + loc,			
		 inst->z * (inst->c - loc));	
	*(inst->v + loc) = item;		
	inst->c += 1;				
}
#endif

