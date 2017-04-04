#ifndef _edna_state_
#define _edna_state_
#include "edna.h"

#include "str.h"

struct Mode {
	char*	name;
	int	(*init)	   (State *);
	int	(*prompt)  (State *, Buffer *, String *, char *);
	int	(*input)   (State *, Buffer *, String *, char *);
	int	(*eval)    (State *, Buffer *, String *, char *);
	int	(*error)   (State *, Buffer *, String *, char *);
};

struct State {
	Mode		 *mode;
	Vector(Command)	 *cmds;
	Vector(Buffer *) *buffers;
	Vector(Mode)	 *modes;
	Vector(Symbol)	 *syms;
	uint8_t		  running:1;
};

/* state.c */
extern int	addbuf		(State *, Buffer *);
extern int	checkoutbuf	(Buffer *, State *, size_t);
extern int	setmode		(State *, char *);
extern int	returnbuf	(State *, Buffer *);

/* init.c */
extern void	freestate	(State *);
extern int	initst		(State *);
extern State*	makestate	(void);
extern int	parse_argv	(State *, char **, char *);
extern int	quit		(State *);
#endif
