/* str_utf8.c -- utf8 conversion and parsing routines */
#include <stdlib.h>
#include <ctype.h>

#include "str.h"
#include "util.h"

int
uchar_extent (unsigned char ch)
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
	