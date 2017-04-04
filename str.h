#ifndef _string_
#define _string_
#include <stdio.h>
#include <stdbool.h>

#include "vec.h"

#ifndef isascii
# define isascii(C) (!((C) & (1<<7)))
#endif

#define eol(str, pos) (pos >= len(str))

typedef Vector(char) String;

/* string.c */
void	chomp		(char *);
void	edna_str_free	(String *);
String *edna_str_alloc	(void);

/* utf8.c */
int 	get_uchar	(char *, const char *);
int	uchar_extent	(const unsigned char);
size_t	ustrlen		(const char *);
#endif
