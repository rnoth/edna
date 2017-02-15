/* commands.h -- command-specific header file */
#ifndef _edna_cmds_
#define _edna_cmds_

#include "edna.h"
#include "line.h"
#include "vector.h"

struct Arg {
	Selection  sel;
	char*	   name;
	char*	   mode;

	VECTOR (char *, param);
};

struct Command {
	char*	name;
	int	(*func)	(State *, Buffer, Arg *, char *);
	char*	mode;
	char*	defaddr;
};

/* command.c */
extern int	grabline	(State *, Buffer, String *, char *);
extern int	cmdchck		(const void *a, const void *b);
extern int	cmdcmp		(const void *a, const void *b);
extern int	cmderror	(State *, Buffer, String *, char *);
extern int	cmdeval		(State *, Buffer, String *, char *);
extern int	cmdprompt	(State *, Buffer, String *, char *);

/* insert.c */
extern int	inserror	(State *, Buffer, String *, char *);
extern int	insline		(State *, Buffer, String *, char *);
extern int	insprompt	(State *, Buffer, String *, char *);

/* parse.c */
extern int	parseline	(String *, Buffer, Arg *, char *);

/* cmd-buffer.c */
extern int	cmd_listbuf	(State *, Buffer, Arg *, char *);
extern int	cmd_openbuf	(State *, Buffer, Arg *, char *);
extern int	cmd_switchbuf	(State *, Buffer, Arg *, char *);

/* cmd-file.c */
extern int	cmd_edit	(State *, Buffer, Arg *, char *);
extern int	cmd_filename	(State *, Buffer, Arg *, char *);
extern int	cmd_quit	(State *, Buffer, Arg *, char *);
extern int	cmd_write	(State *, Buffer, Arg *, char *);

/* cmd-insert.c */
extern int	cmd_insert	(State *, Buffer, Arg *, char *);

/* cmd-print.c */
extern int	cmd_help	(State *, Buffer, Arg *, char *);
extern int	cmd_print	(State *, Buffer, Arg *, char *);

/* cmd-register.c */
extern int	cmd_delete	(State *, Buffer, Arg *, char *);

#endif
