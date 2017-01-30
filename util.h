/* util.h -- various useful macros */
#ifndef _util_
#define _util_

#include <stdio.h>
#include <stdlib.h>

#define FAIL (0)
#define SUCC (1)
#define ERR ((void *)-1)

#define MAX(X,Y)	((X) > (Y) ? (X) : (Y))
#define MIN(X,Y)	((X) < (Y) ? (X) : (Y))
#define LEN(X)		(sizeof X / sizeof *X)
#define BIT(X)		(1 << (X))

#define die(X) do { \
	perror (X); \
	abort (); \
} while (0);

#define warn(X) { \
	perror (X); \
};

#endif
