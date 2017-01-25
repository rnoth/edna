/* input.c -- functions for getting input */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#include "util.h"
#include "edna.h"

int
grabline (String *s, char *err)
{
	int ret = SUCC;

	errno = 0;
	if (!readline (s, stdin)) {
		strncpy (err, strerror (errno), 20);
		ret = FAIL;
	}

	return (ret);

}

int
readline (String *str, FILE *file)
{
#	define CHUNK 20
	char ch, *tmp, buf[6];
	int i, ext, ret = SUCC;
	size_t off = 0;

	for (;;) {
		i = 0;
		clearerr (file);

		if (off + 4 >= str->m) { /* TODO: move resize logic to str.h */
			tmp = realloc (str->v, str->m + CHUNK);
			if (!tmp) {
				perror ("realloc");
				ret = FAIL;
				break;
			}
			str->v = tmp;
			str->m += CHUNK;
		}

		if (!fread (&ch, 1, 1, file))
			goto fail;

		buf[i++] = ch;

		if (isascii(ch)) {
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
			buf[i] = ch;
		}

		buf[i] = 0;
		strcpy (str->v + off, buf);
		++str->c;
		off += i;

		continue;

	fail:

		if (!feof(file))
			perror ("fread");

		ret = FAIL;
		break;
		}

	str->b = off;
	str->v[off] = 0;

	return (ret);
}
