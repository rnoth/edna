/* set.h -- set operations */
#include "edna.h"

typedef unsigned long long  subset;
typedef unsigned long long *Set;

extern Set makeset		(Buffer *buf);
extern Set setaddmemb		(Set A, size_t len, size_t memb);
extern Set setcomplement	(Set A, size_t len);
extern Set setdifference	(Set A, Set B, size_t len);
extern Set setintersect		(Set A, Set B, size_t len);
extern Set setunion		(Set A, Set B, size_t len);

