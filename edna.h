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

struct Command {
	char *handle;
	int (*func)(Position *, char *);
};

/* defined in commands.c */
extern int	append	(Position *, char *);
extern int	delete	(Position *, char *);
extern int	help	(Position *, char *);
extern int	nop	(Position *, char *);
extern int	print	(Position *, char *);
extern int	quit	(Position *, char *);

/* defined in line.c */
extern Line*	appendline	(Line *, char *, size_t);
extern Line*	freelines	(Line *, Line *);
extern Line*	linklines	(Line *, Line*);
extern Line*	makeline	();
extern size_t	readline	(char **);

/* defined in util.c */
extern void	chomp		(char *, size_t);
extern void	warn		();
