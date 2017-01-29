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
	if (FAIL == readline (s, stdin)) {
		if (feof (stdin))
			strcpy (err, "quit");
		else
			strncpy (err, strerror (errno), 20);
		ret = FAIL;
		clearerr (stdin);
	}

	return (ret);

}

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
