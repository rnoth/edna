/* commands.h -- command-specific header file */
#ifndef _edna_cmds_
#define _edna_cmds_

#include "edna.h"

struct Arg {
	Selection *sel;
	char	  *name;
	char	  *mode;

	Vector(char *) *param;
};

struct Command {
	char	*name;
	int	 (*func)(State *, Buffer *, Arg *, char *);
	char	*mode;
	char	*defaddr;
};

/* command.c */
int	cmderror	(State *, Buffer *, String *, char *);
int	cmdeval		(State *, Buffer *, String *, char *);
int	cmdprompt	(State *, Buffer *, String *, char *);
Command *findcmd	(State *, char *);
void 	 freearg	(Arg *);
Arg *	 makearg	(void);

/* insert.c */
int	inserror	(State *, Buffer *, String *, char *);
int	insline		(State *, Buffer *, String *, char *);
int	insprompt	(State *, Buffer *, String *, char *);

/* cmd-buffer.c */
int	cmd_listbuf	(State *, Buffer *, Arg *, char *);
int	cmd_openbuf	(State *, Buffer *, Arg *, char *);
int	cmd_switchbuf	(State *, Buffer *, Arg *, char *);

/* cmd-file.c */
int	cmd_edit	(State *, Buffer *, Arg *, char *);
int	cmd_filename	(State *, Buffer *, Arg *, char *);
int	cmd_quit	(State *, Buffer *, Arg *, char *);
int	cmd_write	(State *, Buffer *, Arg *, char *);

/* cmd-insert.c */
int	cmd_insert	(State *, Buffer *, Arg *, char *);

/* cmd-print.c */
int	cmd_help	(State *, Buffer *, Arg *, char *);
int	cmd_print	(State *, Buffer *, Arg *, char *);
int	cmd_eval	(State *, Buffer *, Arg *, char *);

/* cmd-register.c */
int	cmd_delete	(State *, Buffer *, Arg *, char *);

#endif
