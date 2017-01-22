/* edna.h -- header information */
#ifndef _edna_
#define _edna_

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

#include "util.h"
#include "vector.h"
#include "str.h"

#define LINESIZE (80)

#define FAIL (0)
#define SUCC (1)

typedef unsigned char	byte;
typedef size_t		linenum;

typedef struct Buffer	Buffer;
typedef struct Line	Line;
typedef struct Macro	Macro;
typedef struct Mark	Mark;
typedef struct Mode	Mode;
typedef struct Record	Record;
typedef struct Register	Register;
typedef struct State	State;
typedef VECTOR_TAG (Line*, Selection) Selection;

#include "cmd.h"

struct Buffer {
	/* file info */
	byte	dirty;
	FILE*	file;
	char*	filename;

	/* line info */
	size_t	len;
	Line*	top;
	Line*	bot;
	Line*	curline;
	linenum	lineno;

	/* misc. info */
	Mode*	mode;
	Mark*	marks;
};

struct Line {
	size_t	len;
	char*	str;
	Line*	next;
	Line*	prev;
};

struct Macro {
	char*	name;	/* macro name, not command name */
	Command	cmd;
	Arg	arg;
};

struct Mark {
	char*	name;
	Line*	line;
	linenum	lineno;
};

struct Mode {
	char*	name;
	int	(*prompt)  (State *, Buffer *);
	int	(*eval)    (State *, Buffer *, String *, char *);
	int	(*error)   (State *, Buffer *, char *);
};

struct Register {
	char*	name;
	char*	str;
};

struct State {
	VECTOR (Command, cmds);
	VECTOR (Buffer*, buffers);
	VECTOR (Mode,	 modes);
#if 0
	Macro*		 macros;
	Register*	 regs;
#endif
};

/* buffer.c */
extern int	addbuf		(State *, Buffer *);
extern int	checkoutbuf	(Buffer *, State *, size_t);
extern Buffer*	makebuf		(char *);
extern void	freebuf		(Buffer *);
extern int	rmbuf		(State *, size_t);
extern int	returnbuf	(Buffer *, State *);

/* command.c */
extern int	evalcmd		(State *, Buffer *, String *, char *);

/* file.c */
extern int	readbuf		(Buffer *, char *);
extern int	writebuf	(Buffer *, char *);

/* init.c */
extern void	freestate	(State *);
extern void	initst		(State *);
extern State*	makestate	(void);
extern int	parse_argv	(State *, char *, int, char **);

/* input.c */
extern int	readline	(String *, FILE *, char *);

/* insert.c */
extern int	inserror	(State *, Buffer *, char *);
extern int	insline		(State *, Buffer *, String *, char *);
extern void	inshandle	(int);

/* line.c */
extern size_t	getlineno	(Line *);
extern Line*	makeline	(void);
extern Line*	putline		(Line *, char *, size_t);
extern Line*	walk		(Line *, int, char *);

/* mode.c */
extern int	setmode		(State *, Buffer *, char *);

/* region.c */
extern void	freelines	(Line *, Line *);
extern void	linklines	(Line *, Line*);

/* util.c */
extern void	chomp		(String);

#endif
