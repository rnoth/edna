#ifndef _edna_mode_
#define _edna_mode_
#include "buffer.h"
#include "state.h"
#include "str.h"

struct Mode {
	char*	name;
	int	(*init)	   (State *, Buffer *, String *, char *);
	int	(*prompt)  (State *, Buffer *, String *, char *);
	int	(*input)   (State *, Buffer *, String *, char *);
	int	(*eval)    (State *, Buffer *, String *, char *);
	int	(*error)   (State *, Buffer *, String *, char *);
};

/* mode.c */
extern int	setmode		(State *, Buffer *, char *);

#endif
