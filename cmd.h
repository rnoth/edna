/* commands.h -- command-specific header file */
#ifndef _edna_cmds_
#define _edna_cmds_

#include "edna.h"
#include "vector.h"

typedef struct Arg	Arg;
typedef struct Command	Command;

struct Arg {
	Selection  sel;
	char*	   name;
	char*	   mode;

	VECTOR (char *, param);
	size_t	   cnt;	/* argc equivalent */
	char**	   vec;	/* argv equivalent */
};

struct Command {
	char*	name;
	int	(*func)	(State *, Buffer *, Arg *, char *);
	char*	mode;
	char*	defaddr;
};

/* command.c */
extern int	cmdchck (const void *a, const void *b);
extern int	cmdcmp (const void *a, const void *b);

/* parse.c */
extern int	parseline	(String *, Buffer *, Arg *, char *);

/* TODO: organization */
extern int	cmd_delete	(State *, Buffer *, Arg *, char *);
extern int	cmd_edit	(State *, Buffer *, Arg *, char *);
extern int	cmd_filename	(State *, Buffer *, Arg *, char *);
extern int	cmd_gotol	(State *, Buffer *, Arg *, char *);
extern int	cmd_help	(State *, Buffer *, Arg *, char *);
extern int	cmd_insert	(State *, Buffer *, Arg *, char *);
extern int	cmd_nop		(State *, Buffer *, Arg *, char *);
extern int	cmd_openbuf	(State *, Buffer *, Arg *, char *);
extern int	cmd_print	(State *, Buffer *, Arg *, char *);
extern int	cmd_quit	(State *, Buffer *, Arg *, char *);
extern int	cmd_subst	(State *, Buffer *, Arg *, char *);
extern int	cmd_switchbuf	(State *, Buffer *, Arg *, char *);
extern int	cmd_write	(State *, Buffer *, Arg *, char *);
#endif
