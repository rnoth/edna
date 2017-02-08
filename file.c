/* file.c -- file manipulation functions */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edna.h"
#include "buffer.h"
#include "line.h"
#include "str.h"
#include "util.h"

extern int readbuf	(Buffer *, char *);
extern int writebuf	(Buffer *, char *);

int
readbuf (Buffer *buf, char *err)
{
	int ret = FAIL;
	String *s;
	Line *new;

	buf->file = fopen (buf->filename, "r+");
	if (!buf->file ) {
		warn ("fopen");
		return (FAIL);
	}

	s = makestring (LINESIZE);

	errno = 0;
	while (!feof (buf->file)) {
		if (readline (s, buf->file) == FAIL) {
			if (errno) {
				strncpy (err, strerror (errno), 20);
				goto finally;
			} else
				continue;
		}

		new = makeline ();
		if (changeline (new, s) == FAIL) {
			strcpy (err, "readbuf(): changeline() failed. memory errors?");
			freelines (new, new->next);
			goto finally;
		} else if (addline (buf, new) == FAIL) {
			strcpy (err, "readbuf(): changeline() failed. buffer inconsistency?");
			freelines (new, new->next);
			goto finally;
		}
	}
	clearerr (buf->file);

finally:
	freestring (s);
	buf->dirty = 0;
	return (ret);
}

int
writebuf (Buffer *buf, char *error)
{
	Line *tmp;

	if (!buf->filename[0]) {
		strcpy (error, "no filename");
		return FAIL;
	}

	if (!(buf->file = freopen (buf->filename, "w+", buf->file))) {
		strcpy (error, "fopen: ");
		strcpy (error + strlen (error), strerror (errno));
		return FAIL;
	}

	for (tmp = buf->top; tmp; tmp = getnext(tmp))
		fputs (tmp->str->v, buf->file);

	return SUCC;
}
