/* input.c -- functions for getting input */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#include "util.h"
#include "edna.h"

extern int	readline	(String *, FILE *, char *);

int
readline (String *str, FILE *file, char *error)
{
#	define CHUNK 20
	char ch, *tmp, buf[6];
	int i, ext, ret = SUCC;
	size_t off = 0;

	for (;;) {
		i = 0;
		clearerr (file);

		if (off + 4 >= str->m) {
			tmp = realloc (str->v, str->m + CHUNK);
			if (!tmp) {
				strncpy (error, strerror (errno), 20);
				perror ("realloc");
				return FAIL;
			}
			str->v = tmp;
			str->m += CHUNK;
		}

		if (!fread (&ch, 1, 1, file))
			goto finally;

		buf[i++] = ch;

		if (!(ch & BIT (7))) {
			str->v[off] = ch;
			++str->c;
			off += i;
			if (ch == '\n')
				break;
			continue;
		}


		/* multibyte character */
		ext = uchar_extent (ch);

		for (; i < ext; ++i) {
			fread (&ch, 1, 1, file);
			if (!(ch & BIT (7))) {
				strcpy (error, "invalid utf-8 sequence");
				ret = FAIL;
				i = -1;
				break;
			}
			buf[i] = ch;
		}
		if (i != -1) { /* valid utf-8 */
			buf[i] = 0;
			strcpy (str->v + off, buf);
			++str->c;
			off += i;
		}

		continue;

	finally:

		if (feof(file)) /* eof? */
			strcpy (error, "eof");
		else { /* actual error occurred */
			strncpy (error, strerror (errno), 20);
			perror ("fread");
		}
		ret = FAIL;
		break;
	}

	str->b = off;
	str->v[off] = 0;

	return (ret);
}
