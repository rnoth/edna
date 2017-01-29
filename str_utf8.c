/* str_utf8.c -- utf8 conversion and parsing routines */
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "str.h"
#include "util.h"

char *
get_uchar (const char *s)
{
	char *ret;
	int ext;

	ext = uchar_extent (*s);
	if (ext == -1)
		return NULL;

	ret = malloc (ext + 1);
	if (!ret) die ("malloc");

	memcpy (ret, s, ext);
	ret[ext] = 0;

	return ret;
		
}

int
uchar_extent (const unsigned char ch)
{
	if (isascii (ch))
		return (1);
	else if (ch >= 0xC2 && ch <= 0xDF)
		return (2);
	else if (ch >= 0xE0 && ch <= 0xEF)
		return (3);
	else if (ch >= 0xF0 && ch <= 0xF4)
		return (4);
	else
		return (-1); /* error */
} 

size_t
ustrlen (const char *s)
{
	size_t ret = 0;

	while (*s && ++ret)
		s += uchar_extent (*s);

	return (ret);
}
