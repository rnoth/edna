/* string.h -- generic String methods */
#ifndef _string_
#define _string_

typedef struct String	String;

struct String {
	size_t c; /* length */
	char *v;  /* string */
	size_t m; /* allocated memory */
};

extern void	freestring	(String);
extern int	copystring	(String *, String *);
extern String	makestring	(size_t);
extern int	resizestring	(String *, size_t);
#endif
