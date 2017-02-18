/* util.h -- various useful macros */
#ifndef _util_
#define _util_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FAIL (0)
#define SUCC (1)

#define BIT(X)		(1UL << (X))
#define COPY(D,S)	(memcpy((D), (S), sizeof *(D)))
#define LEN(X)		(sizeof X / sizeof *X)
#define MAX(X,Y)	((X) > (Y) ? (X) : (Y))
#define MIN(X,Y)	((X) < (Y) ? (X) : (Y))

#define die(X) do { \
	perror (X); \
	abort (); \
} while (0);

#define warn(X) do { \
	perror (X); \
} while (0);

#endif
