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

/* deprecated */
typedef struct Position Position;
struct Position {
	size_t lineno;
	Line *line;
};

typedef struct State State;
struct State {
	FILE *file;
	short dirty;
	Line *curline;
	size_t lineno;
};

typedef struct Arg Arg;
struct Arg {
	short rel;	/* is address relative? */
	int addr;
	char *mode;
};

typedef struct Command Command;
struct Command {
	char *name;
	int (*func)(State *, Arg *, char *);
	char *mode;
};

/* defined in commands.c */
extern int	delete		(State *, Arg *, char *arg);
extern int	gotol		(State *, Arg *, char *arg);
extern int	help		(State *, Arg *, char *arg);
extern int	insert		(State *, Arg *, char *arg);
extern int	nop		(State *, Arg *, char *arg);
extern int	print		(State *, Arg *, char *arg);
extern int	quit		(State *, Arg *, char *arg);
extern int	write		(State *, Arg *, char *);

/* defined in file.c */
extern void	readfile	(State *, char *);
extern void	writefile	(State *);

/* defined in input.c */
extern void	readline	(char **, size_t *, char *, ...);
extern void	parseline	(char *, char *, Arg *);

/* defined in line.c */
extern Line*	freelines	(Line *, Line *);
extern Line*	linklines	(Line *, Line*);
extern Line*	makeline	();
extern Line*	putline		(Line *, char *, size_t, int option);
extern Line*	walk		(Line *, int, char *);

/* defined in util.c */
extern void	chomp		(char *, size_t);
extern void	die		();
extern void	warn		();
