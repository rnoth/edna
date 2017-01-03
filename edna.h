/* edna.h -- header information */
#include <stdio.h>

#define LEN(X) (sizeof X / sizeof *X)

#define INSERT -1
#define CHANGE 0
#define APPEND 1
#define LINESIZE 80

typedef struct Line Line;
typedef struct Command Command;
typedef struct Arg Arg;
typedef struct Mark Mark;
typedef struct Buffer Buffer;
typedef struct Register Register;
typedef struct Macro Macro;
typedef struct State State;

struct Line {
	size_t len;
	char *str;
	Line *next;
	Line *prev;
};

struct Command {
	char *name;
	int (*func)(State *, Buffer *, Arg *, char *);
	char *mode;
};

struct Mark {
	char *name;
	Line *l;
	size_t lineno;
};

struct Buffer {
	short dirty;
	FILE *file;
	char *filename;
	Line *curline;
	size_t lineno;
	Mark *marks;
};

struct Register {
	char *name;
	char *str;
};

struct Arg {
	short rel;	/* is address relative? */
	int addr;
	size_t range;
	char *name;
	char *mode;
	size_t cnt; /* argc equivalent */
	char **vec; /* argv equivalent */
};

struct Macro {
	char *name; /* macro name, not command name */
	Command cmd;
	Arg arg;
};

struct State {
	Buffer **buffers;
	Buffer *curbuf;
	size_t buflen;
	Macro *macros;
	Register *regs;
};

/* defined in buffer.c */
extern int	addbuf		(State *, Buffer *);
extern Buffer*	makebuf		(char *);

/* defined in file.c */
extern void	readbuf		(Buffer *);
extern void	writebuf	(Buffer *);

/* defined in init.c */
extern State*	makestate	(void);
extern Arg*	makearg		(void);
extern void	cleanup		(State *, Arg *);

/* defined in input.c */
extern void	readline	(char **, size_t *, char *, ...);
extern void	parseline	(char *, size_t, Arg *);

/* defined in line.c */
extern Line*	getnext		(Line *);
extern Line*	getprev		(Line *);
extern Line*	makeline	();
extern Line*	putline		(Line *, char *, size_t, int option);
extern Line*	walk		(Line *, int, char *);

/* defined in region.c */
extern void	freelines	(Line *, Line *);
extern void	linklines	(Line *, Line*);

/* defined in util.c */
extern void	chomp		(char *, size_t);
extern void	die		(char *);
extern void	warn		(char *);
