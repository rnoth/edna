/* commands.h -- command-specific header file */
#ifndef _edna_cmds_
#define _edna_cmds_
/* TODO: organization */
extern int	delete		(State *, Buffer *, Arg *, char *);
extern int	edit		(State *, Buffer *, Arg *, char *);
extern int	filename	(State *, Buffer *, Arg *, char *);
extern int	gotol		(State *, Buffer *, Arg *, char *);
extern int	help		(State *, Buffer *, Arg *, char *);
extern int	insert		(State *, Buffer *, Arg *, char *);
extern int	nop		(State *, Buffer *, Arg *, char *);
extern int	openbuf		(State *, Buffer *, Arg *, char *);
extern int	print		(State *, Buffer *, Arg *, char *);
extern int	quit		(State *, Buffer *, Arg *, char *);
extern int	subst		(State *, Buffer *, Arg *, char *);
extern int	switchbuf	(State *, Buffer *, Arg *, char *);
extern int	write		(State *, Buffer *, Arg *, char *);
#endif
