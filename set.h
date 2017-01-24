/* set.h -- set operations */
#ifndef _set_
#define _set_

#define freeset(A) free (A);

typedef unsigned long  subset;
typedef unsigned long *Set;

extern Set makeset		(size_t len);
extern Set setaddmemb		(Set A, size_t len, size_t memb);
extern Set setcomplement	(Set A, size_t len);
extern Set setdifference	(Set A, Set B, size_t len);
extern Set setintersect		(Set A, Set B, size_t len);
extern int setoffset		(Set A, Set B, size_t len);
extern Set setunion		(Set A, Set B, size_t len);

#endif
