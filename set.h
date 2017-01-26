/* set.h -- set operations */
#ifndef _set_
#define _set_

#define freeset(A) free (A);
#define setshiftleft(A, len, off) setshift (A, len, off, 1)
#define setshiftright(A, len, off) setshift (A, len, off, 0)

typedef unsigned long doubleset;
typedef unsigned int  subset;
typedef unsigned int *Set;

extern Set makeset		(size_t len);
extern Set setaddmemb		(Set A, size_t len, size_t memb);
extern Set setcomplement	(Set A, size_t len);
extern Set setdifference	(Set A, Set B, size_t len);
extern Set setintersect		(Set A, Set B, size_t len);
extern Set setshift		(Set A, size_t len, size_t off, int left);
extern int setrightmost		(Set A, size_t len);
extern Set setunion		(Set A, Set B, size_t len);

#endif
