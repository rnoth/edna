#ifndef _edna_state_
#define _edna_state_
#include "edna.h"
#include "vector.h"

struct State {
	VECTOR (Command, cmds);
	VECTOR (Buffer*, buffers);
	VECTOR (Mode,	 modes);
};

/* state.c */
extern int	addbuf		(State *, Buffer *);
extern int	checkoutbuf	(Buffer *, State *, size_t);
extern int	rmbuf		(State *, size_t);
extern int	returnbuf	(Buffer *, State *);

/* init.c */
extern void	freestate	(State *);
extern void	initst		(State *);
extern State*	makestate	(void);
extern int	parse_argv	(State *, char *, int, char **);
#endif
