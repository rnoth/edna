/* string.h -- generic String methods */
#ifndef _string_
#define _string_

typedef struct String	String;
typedef unsigned long	rune;

struct String {
	size_t c; /* characters */
	size_t b; /* bytes */
	size_t m; /* allocated memory */
	char *v;  /* string */
};

/* string.c */
extern int	appendchar	(String *s, char);
extern void	freestring	(String *);
extern String*	chartostr	(char *);
extern int	copystring	(String *, String *);
extern String*	makestring	(size_t);
extern int	resizestring	(String *, size_t);

/* str_utf8.c */
extern char*	get_uchar	(char *);
extern int	uchar_extent	(unsigned char);
#endif
