/* edna.h -- header information */

#define MALLOC(X, Y) if (!(X = malloc (Y)))\
			warn ()
#define CALLOC(X, Y, Z) if (!(X = calloc (Y, Z)))\
			warn ()
#define REALLOC(X, Y) if (!(X = realloc (X, Y)))\
			warn ()
/* man 3 printf says ``If an output error is encountered, a negative value is
 * returned.'', not necessarily -1
 */
#define PRINTF(...) if (0 > printf (__VA_ARGS__))\
			warn ()
#define FPRINTF(...) if (!fprintf (__VA_ARGS__))\
			warn ()
#define MEMCPY(X, Y, Z) if (!memcpy(X, Y, Z))\
			warn ()
#define GETLINE(X, Y, Z) if (-1 == getline(&X, &Y, Z))\
			warn ()

#define LEN(X) (sizeof X / sizeof *X)

#define LINESIZE 80

typedef struct Line Line;
struct Line {
	size_t len;
	char *str;
	Line *next;
	Line *prev;
};

typedef struct Position Position;
struct Position {
	size_t lineno;
	Line *line;
};

typedef struct Arg Arg;
struct Arg {
	int addr;
};

typedef struct Command Command;
struct Command {
	char *handle;
	int (*func)(Position *, Arg *, char *);
};

/* defined in commands.c */
extern int	append	(Position *, Arg *, char *arg);
extern int	back	(Position *, Arg *, char *arg);
extern int	change	(Position *, Arg *, char *arg);
extern int	delete	(Position *, Arg *, char *arg);
extern int	forward	(Position *, Arg *, char *arg);
extern int	help	(Position *, Arg *, char *arg);
extern int	insert	(Position *, Arg *, char *arg);
extern int	nop	(Position *, Arg *, char *arg);
extern int	print	(Position *, Arg *, char *arg);
extern int	quit	(Position *, Arg *, char *arg);

/* defined in line.c */
extern Line*	freelines	(Line *, Line *);
extern Line*	linklines	(Line *, Line*);
extern Line*	makeline	();
extern Line*	putline		(Line *, char *, size_t, int option);
extern size_t	readline	(char **);
extern Line*	walk		(Line *, int, char *);

/* defined in util.c */
extern void	chomp		(char *, size_t);
extern void	warn		();
