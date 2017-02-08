/* edna.h -- header information */
#ifndef _edna_
#define _edna_

#include "util.h"
#include "vector.h"
#include "str.h"

#define LINESIZE (80)

typedef unsigned char	byte;
typedef size_t		linenum;

typedef struct Buffer	Buffer;
typedef struct Line	Line;
typedef struct Mode	Mode;
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
};

struct Line {
	size_t	len;
	String*	str;
	Line*	next;
	Line*	prev;
};

struct Mode {
	char*	name;
	int	(*init)	   ();
	int	(*prompt)  ();
	int	(*input)   ();
	int	(*eval)    ();
	int	(*error)   ();
};

struct State {
	VECTOR (Command, cmds);
	VECTOR (Buffer*, buffers);
	VECTOR (Mode,	 modes);
};

/* buffer.c */
extern int	addbuf		(State *, Buffer *);
extern int	addline		(Buffer *, Line *, size_t);
extern int	checkoutbuf	(Buffer *, State *, size_t);
extern Buffer*	makebuf		(char *);
extern void	freebuf		(Buffer *);
extern void	setcurline	(Buffer *, Line *);
extern int	rmbuf		(State *, size_t);
extern int	returnbuf	(Buffer *, State *);

/* command.c */
extern int	cmdparse	(String *, void *, Buffer *, char *);
extern int	cmdeval		(State *, Buffer *, String *, char *);

/* file.c */
extern int	readbuf		(Buffer *, char *);
extern int	writebuf	(Buffer *, char *);

/* init.c */
extern void	freestate	(State *);
extern void	initst		(State *);
extern State*	makestate	(void);
extern int	parse_argv	(State *, char *, int, char **);

/* input.c */
extern int	grabline	(String *, char *);
extern int	readline	(String *, FILE *);

/* insert.c */
extern int	inserror	(State *, Buffer *, char *);
extern int	insparse	(String *, void *, Buffer *, char *);
extern int	insline		(State *, Buffer *, String *, char *);
extern void	inshandle	(int);

/* line.c */
extern size_t	getlineno	(const Line *);
extern Line*	getnext		(const Line *);
extern Line*	getprev		(const Line *);
extern void	freelines	(Line *, Line *);
extern void	linklines	(Line *, Line*);
extern Line*	makeline	(void);
extern int	changeline	(Line *, String *);
extern Line*	walk		(Line *, int);

/* mode.c */
extern int	setmode		(State *, Buffer *, char *);

/* util.c */
extern void	chomp		(String);

#endif
