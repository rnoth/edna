#ifndef _string_
#define _string_
#include <stdio.h>

#ifndef isascii
#	define isascii(C) (!((C) & (1<<7)))
#endif

#ifndef SUCC
#	define SUCC (1)
#	define FAIL (0)
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
extern String*	clonechars	(char *);
extern String*	copystring	(String *, String *);
extern String*	copychars	(String *, const char *);
extern String*	makestring	(size_t);
extern int	resizestring	(String *, size_t);
extern char*	strtochar	(String *);

/* str-io.c */
extern int	readline	(String *, FILE *);

/* str_utf8.c */
extern int 	get_uchar	(char *, const char *);
extern int	uchar_extent	(const unsigned char);
extern size_t	ustrlen		(const char *);
#endif
