/* string.h -- generic String methods */

typedef struct String	String;

struct String {
	size_t c; /* length */
	char *v;  /* string */
	size_t m; /* allocated memory */
};

extern String	makestring	(size_t);
extern void	freestring	(String);
extern int	resizestring	(String *, size_t);
