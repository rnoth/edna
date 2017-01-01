/* edna.h -- header information */

#define LEN(X) (sizeof X / sizeof *X)

#define INSERT -1
#define CHANGE 0
#define APPEND 1
#define LINESIZE 80

typedef struct Line Line;
struct Line {
	size_t len;
	char *str;
	Line *next;
	Line *prev;
};

typedef struct State State;
struct State {
	FILE *file;
	char *filename;
	short dirty;
	Line *curline;
	size_t lineno;
};

typedef struct Arg Arg;
struct Arg {
	short rel;	/* is address relative? */
	int addr;
	char *name;
	char *mode;
	size_t cnt;
	char **vec;
};

typedef struct Command Command;
struct Command {
	char *name;
	int (*func)(State *, Arg *, char *);
	char *mode;
};

struct Mark {
	char c;
	Line *l;
};

struct Macro {
	Command cmd;
	Arg arg;
};

/* defined in file.c */
extern void	readfile	(State *);
extern void	writefile	(State *);

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
