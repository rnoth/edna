/* str-io.c -- string input/outpu routines */
#include <stdio.h>
#include "str.h"
#include "util.h"

#define CHUNK 20

bool
readline(String *str, FILE *file)
{
	char ch, buf[5];
	int i, ext, ret = true;

	copychars(str, "");
	for (;;) {
		i = 0;
		clearerr(file);

		if (!fread (&ch, 1, 1, file)) goto fail;
		buf[i++] = ch;

		if (isascii(ch)) {
			appendchar(str, ch);
			if (ch == '\n') break;
			else continue;
		}

		/* multibyte character */
		ext = uchar_extent (ch);

		for (; i < ext; ++i) {
			fread (&ch, 1, 1, file);
			buf[i] = ch;
		}

		buf[i] = 0;
		appendchars(str, buf);

		continue;

	fail:

		if (!feof(file)) die("fread");
	}

	return ret;
}
