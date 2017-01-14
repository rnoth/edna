/* input.c -- functions for getting input */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#include "edna.h"

extern int	readline	(String *, FILE *, char *);

int
readline (String *str, FILE *file, char *error)
{
#	define CHUNK 20
	size_t off = 0;

	for (;;) {
		if (!memset (str->v + (str->m - 1), '\n', 1)) die ("memset");

		if (!fgets (str->v + off, str->m - off, file)) {
			if (!feof (file)) { /* only report non-eof errors */
				strcpy (error, "fgets: ");
				strncpy (error + strlen (error), strerror (errno), LINESIZE);
			}
			return FAIL;
		}

		if (str->v[str->m - 1] == '\n') /* got all of line */
			break;

		if (!resizestring (str, str->m + CHUNK)) {
			strcpy (error, "error allocating buffer");
			return FAIL; /* error */
		}

		off = str->c - 1;
	}

	str->c = strlen (str->v);

	return SUCC;
}

