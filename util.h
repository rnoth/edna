/* util.h -- various useful macros */
#ifndef _util_
#define _util_

#include <stdio.h>
#include <stdlib.h>

#define MAX(X,Y)	((X) > (Y) ? (X) : (Y))
#define MIN(X,Y)	((X) < (Y) ? (X) : (Y))
#define LEN(X)		(sizeof X / sizeof *X)
#define BIT(X)		(1 << X)

#define die(X) { \
	perror (X); \
	exit (1); \
};

#define warn(X) { \
	perror (X); \
};

#endif
