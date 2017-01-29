/* set.h -- set operations */
#ifndef _set_
#define _set_
#include <stdint.h>

#define freeset(A) free (A);
#define setshiftleft(A, len, off) setshift (A, len, off, 1)
#define setshiftright(A, len, off) setshift (A, len, off, 0)

typedef uint64_t  doubleset;
typedef uint32_t  subset;
typedef uint32_t *Set;

extern Set	cloneset	(Set A, size_t len);
extern size_t	offset		(uint32_t);
extern Set	makeset		(size_t len);
extern void*	set2vec		(Set A, size_t len);
extern Set	setaddmemb	(Set A, size_t len, size_t memb);
extern Set	setcomplement	(Set A, size_t len);
extern Set	setdifference	(Set A, Set B, size_t len);
extern Set	setintersect	(Set A, Set B, size_t len);
extern Set	setshift	(Set A, size_t len, size_t off, int left);
extern int	setrightmost	(Set A, size_t len);
extern Set	setunion	(Set A, Set B, size_t len);

#endif
