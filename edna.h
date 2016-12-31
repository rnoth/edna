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
	char *mode;
	char *str;
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

/* defined in commands.c */
extern int	delete		(State *, Arg *, char *);
extern int	filename	(State *, Arg *, char *);
extern int	gotol		(State *, Arg *, char *);
extern int	help		(State *, Arg *, char *);
extern int	insert		(State *, Arg *, char *);
extern int	nop		(State *, Arg *, char *);
extern int	print		(State *, Arg *, char *);
extern int	quit		(State *, Arg *, char *);
extern int	write		(State *, Arg *, char *);

/* defined in file.c */
extern void	readfile	(State *);
extern void	writefile	(State *);

/* defined in input.c */
extern void	readline	(char **, size_t *, char *, ...);
extern void	parseline	(char *, char *, Arg *);

/* defined in line.c */
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
