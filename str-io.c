/* str-io.c -- string input/outpu routines */
#include <stdio.h>

#include "str.h"

int
readline (String *str, FILE *file)
{
#	define CHUNK 20
	char ch, buf[5];
	int i, ext, ret = SUCC;

	copychars (str, "");
	for (;;) {
		i = 0;
		clearerr (file);

		if (!fread (&ch, 1, 1, file))
			goto fail;

		buf[i++] = ch;

		if (isascii(ch)) {
			appendchar (str, ch);
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
		appendchars (str, buf);

		continue;

	fail:

		if (!feof(file))
			perror ("fread");

		ret = FAIL;
		break;
		}

	return (ret);
}
