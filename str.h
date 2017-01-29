/* string.h -- generic String methods */
#ifndef _string_
#define _string_

#ifndef isascii
#define isascii(C) (!((C) & (1<<7)))
#endif

typedef struct String	String;
typedef unsigned long	rune;

struct String {
	size_t c; /* characters */
	size_t b; /* bytes */
	size_t m; /* allocated memory */
	char *v;  /* string */
};

/* string.c */
extern int	appendstring	(String *, char *);
extern int	appendchar	(String *, char);
extern int	appendchars	(String *, char *);
extern void	freestring	(String *);
extern String*	chartostr	(char *);
extern int	copystring	(String *, String *);
extern int	copychars	(String *, const char *);
extern String*	makestring	(size_t);
extern int	resizestring	(String *, size_t);

/* str_utf8.c */
extern char*	get_uchar	(const char *);
extern int	uchar_extent	(const unsigned char);
extern size_t	ustrlen		(const char *);
#endif
