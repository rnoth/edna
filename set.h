/* set.h -- set operations */
#ifndef _set_
#define _set_
#include <stdint.h>

#include "vector.h"

typedef struct Set Set;

struct Set {
	size_t c;
	uint32_t *v;
};

extern Set *	cloneset	(Set *);
extern void	free_set	(Set *);
extern Set *	make_set	(void);
extern size_t	offset		(uint32_t);
extern void *	set2vec		(Set *);
extern Set *	setaddmemb	(Set *, size_t memb);
extern Set *	setcomplement	(Set *);
extern Set *	setdifference	(Set *, Set *);
extern Set *	setintersect	(Set *, Set *);
extern Set *	setshiftright	(Set *, size_t);
extern size_t	setrightmost	(Set *);
extern Set *	setunion	(Set *, Set *);

#endif
