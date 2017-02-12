/* str_utf8.c -- utf8 conversion and parsing routines */
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "str.h"
#include "util.h"

int
get_uchar(char *dest, const char *src)
{
	size_t off, len;
	char tmp[5];
	int ext;

	ext = uchar_extent(*src);

	if (ext == -1)
		len = 1;
	else
		len = ext;

	memset(tmp, 0, len);
	memcpy(tmp, src, len);
	for (off = 0; off < len; ++off) {
		if (len > 1 && isascii(tmp[off]))
			goto invalid;
		else if (((unsigned char)tmp[off]) > 0xF4)
			goto invalid;

		continue;
	invalid:
			//ext = -ext;
			tmp[off] = 0;
			break;
	}

	tmp[off] = 0;

	memcpy(dest, tmp, off + 1);

	return off;
}

int
uchar_extent(const unsigned char ch)
{
	if (!ch)
		return 0;
	else if (isascii(ch))
		return 1;
	else if (ch >= 0xC2 && ch <= 0xDF)
		return 2;
	else if (ch >= 0xE0 && ch <= 0xEF)
		return 3;
	else if (ch >= 0xF0 && ch <= 0xF4)
		return 4;
	else
		return -1; /* error */
} 

size_t
ustrlen(const char *s)
{
	size_t ret = 0;

	while (*s && ++ret)
		s += uchar_extent(*s);

	return ret;
}
