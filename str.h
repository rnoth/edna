#ifndef _string_
#define _string_
#include <stdio.h>
#include <stdbool.h>

#ifndef isascii
# define isascii(C) (!((C) & (1<<7)))
#endif

typedef struct String	String;

struct String {
	size_t c; /* characters */
	size_t b; /* bytes */
	size_t m; /* allocated memory */
	char *v;  /* string */
};

/* string.c */
extern bool	appendstring	(String *, char const *);
extern bool	expandstring	(String *);
extern void	freestring	(String *);
extern bool 	copystring	(String *, String const *);
extern String *	makestring	(void);

/* str-char.c */
extern bool	appendchar	(String *, char const);
extern bool	appendchars	(String *, char const *);
extern String *	chartostr	(char const *);
extern String *	clonechars	(char const *);
extern String *	copychars	(String *, char const *);
extern char *	strtochar	(String const *);

/* str-io.c */
extern bool	readline	(String *, FILE *);
extern void	chomp		(char *);

/* str-misc.c */
extern void	chompstr	(String *);
extern bool	eol		(String const *, size_t);

/* str_utf8.c */
extern int 	get_uchar	(char *, const char *);
extern int	uchar_extent	(const unsigned char);
extern size_t	ustrlen		(const char *);
#endif
