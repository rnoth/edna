#ifndef _edna_state_
#define _edna_state_
#include "edna.h"
#include "vector.h"

struct State {
	VECTOR (Command, cmds);
	VECTOR (Buffer*, buffers);
	VECTOR (Mode,	 modes);
};

/* init.c */
extern void	freestate	(State *);
extern void	initst		(State *);
extern State*	makestate	(void);
extern int	parse_argv	(State *, char *, int, char **);
#endif
